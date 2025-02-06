#include "SerialAdapter.h"


SerialAdapter::SerialAdapter() {
  m_filedescriptor = -1;
  m_status = -1;

  time_t sec = SERIALADAPTER_DEFAULT_TIMEOUT / 1000;
  __suseconds_t usec = (SERIALADAPTER_DEFAULT_TIMEOUT - sec * 1000) * 1000;
  m_timeout = {.tv_sec = sec, .tv_usec = usec};

  m_poll_timeout_msecs = 10;
  m_secured = false;
};

SerialAdapter::~SerialAdapter() {
  this->Close();
};

void SerialAdapter::Close() {
  if( m_filedescriptor != -1 ) {
    // Try to turn the adapter off
    this->Reset();
  }
  this->CloseWhenUnestablishedConnection();
};

void SerialAdapter::CloseWhenUnestablishedConnection() {
  if( m_filedescriptor != -1 ) {
	  close( m_filedescriptor );
  }
  this->CloseWhenUnopenedFile();
}

void SerialAdapter::CloseWhenUnopenedFile() {
	m_filedescriptor = -1;
	m_status = -1;
}

int SerialAdapter::PayloadType() {
  return m_header[ 0 ];
};

unsigned char* SerialAdapter::Payload() {
  return m_payload;
};

int SerialAdapter::PayloadLength() {
  return m_payload_length;
};

bool SerialAdapter::Init( const char* path, bool surpress_warnings ) {
  if( m_filedescriptor != -1 ) {
    MSG_SERIALADAPTER_DEBUG("Called Init on serial adapter with an already open file: " + std::to_string(m_filedescriptor));
    this->Close(); // ensure we aren't in a funky state before attempting to initialize
  }

  m_filedescriptor = open( path, O_RDWR | O_NOCTTY | O_NONBLOCK );
  if( m_filedescriptor < 0 ) {
    MSG_SERIALADAPTER_DEBUG("Could not open file " + std::string(path));
    this->CloseWhenUnopenedFile();
    return false;
  }

  struct termios options;

  if( tcgetattr( m_filedescriptor, &options ) < 0 ) {
	MSG_SERIALADAPTER_DEBUG("Could not get serial terminal attributes.");
	this->CloseWhenUnestablishedConnection();
    return false;
  }

  speed_t baudrate = SERIALADAPTER_DEFAULT_BAUDRATE;

  cfsetispeed( &options, baudrate );
  cfsetospeed( &options, baudrate );
  cfmakeraw( &options );

  if( tcsetattr( m_filedescriptor, TCSANOW, &options ) < 0) {
    MSG_SERIALADAPTER_DEBUG("Could not set serial terminal attributes such as i/o baudrate.");
    this->CloseWhenUnestablishedConnection();
    return false;
  }

  // Attempt to flush the input buffer
  if (tcflush(m_filedescriptor, TCIFLUSH) == -1) {
      MSG_SERIALADAPTER_ERROR("tcflush() failed with m_filedescriptor " + std::to_string(m_filedescriptor) + " during serial adapter initialization: " + std::to_string(errno));
      this->CloseWhenUnestablishedConnection();
      return false;
  }

//  TODO:  I was thinking this MAY be necessary for some extreme situations where the adapter's firmware
//         gets stuck in a non-ready state, so perhaps its best to do this to reset the connnection.
//         Leaving out for now.
//  if( !this->Reset() ) {
//	MSG_SERIALADAPTER_ERROR( "Failed to ensure serial adapter is reset before initializing." );
//  }

  if( !this->GetType() ) {
    MSG_SERIALADAPTER_ERROR( "Failed to get adapter type." );
    this->Close();
    return false;
  }

  if( m_type == ADAPTER_TYPE_X360SK ) {
    MSG_SERIALADAPTER_INFO( "Correct adapter type found = X360SK" );

    if (!m_secured) {
      // We now know we have the correct serial adapter.  Time to get possessive and ensure nobody else
      // tries to get access to it (I'm looking at you QLC+!).
      int result = chmod(path, S_IRUSR | S_IWUSR);
      if (result == 0) {
    	  MSG_SERIALADAPTER_DEBUG("Set read/write permissions of " + std::string(path) + " to only me.");
      } else {
    	  MSG_SERIALADAPTER_INFO("Problem setting read/write permissions of " + std::string(path) + " to only me.");
      }

      result = this->RemoveAcl(path);
      if (result == 0) {
    	  MSG_SERIALADAPTER_DEBUG("Cleared ACL of " + std::string(path) + ".");
      } else {
    	  MSG_SERIALADAPTER_INFO("Problem clearing ACL of " + std::string(path) + ".");
      }

      m_secured = true;
    }

  } else {
    if( m_type == ADAPTER_TYPE_X360 ) {
      MSG_SERIALADAPTER_ERROR( "Unsupported adapter type found = X360" );
      this->Close();
    } else if( m_type == ADAPTER_TYPE_XONE ) {
      MSG_SERIALADAPTER_ERROR( "Unsupported adapter type found = XONE" );
      this->Close();
    } else {
      MSG_SERIALADAPTER_ERROR( "Unknown adapter type found." );
      this->Close();
    }

    this->DumpData( true );

    return false;
  }

  // Get version
  if( !this->GetVersion() ) {
    MSG_SERIALADAPTER_ERROR( "Failed to get adapter version." );
    this->Close();

    this->DumpData( true );

    return false;
  }
  MSG_SERIALADAPTER_INFO( "Adapter version = " << m_version_major << "." << m_version_minor );

  if( !this->Start() ) {
    MSG_SERIALADAPTER_ERROR( "Serial Adapter : ERROR : Failed to start." );
    this->Close();
    return false;
  }

  // Setup polling options
  m_poll_fds[ 0 ].fd = m_filedescriptor;
  m_poll_fds[ 0 ].events = POLLIN | POLLOUT | POLLERR | POLLHUP | POLLNVAL;  // input/output/error/disconnect/fd invalid.  

  // Save warnings surpress
  m_surpress_warnings = surpress_warnings;

  return true;
};

int SerialAdapter::Write( unsigned char* buffer, unsigned int count )
{
  unsigned int bytes_written = 0;
  int write_amount;

  fd_set writefds;

  while( bytes_written != count ) {
    FD_ZERO( &writefds );
    FD_SET( m_filedescriptor, &writefds );
    int status = select( m_filedescriptor + 1, NULL, &writefds, NULL, &m_timeout );
    if( status > 0 ) {
      if( FD_ISSET( m_filedescriptor, &writefds ) ) {
        write_amount = write( m_filedescriptor, buffer + bytes_written, count - bytes_written );
        if( write_amount > 0 ) {
          bytes_written += write_amount;
        }
      }
    } else if( errno == EINTR ) {
      continue;
    } else {
      if( !m_surpress_warnings ) {
        MSG_SERIALADAPTER_INFO( "*WARNING* Select error failed." );
      }
      break;
    }
  }

  return bytes_written;
};

int SerialAdapter::Read( unsigned char* buffer, unsigned int count ) {

  unsigned int bytes_read = 0;
  int read_amount;

  fd_set readfds;

  while( bytes_read != count ) {
    FD_ZERO( &readfds );
    FD_SET( m_filedescriptor, &readfds );
    int status = select( m_filedescriptor + 1, &readfds, NULL, NULL, &m_timeout );
    if( status > 0 ) {
      if( FD_ISSET( m_filedescriptor, &readfds ) ) {
        read_amount = read( m_filedescriptor, buffer + bytes_read, count - bytes_read );
        if( read_amount > 0 ) {
          bytes_read += read_amount;
        }
      }
    } else if( status < 0 ) {
      if( errno == EINTR ) {
        continue;
      }
      if( !m_surpress_warnings ) {
        MSG_SERIALADAPTER_INFO( "*WARNING* Select error failed." );
      }
    } else {
      if( !m_surpress_warnings ) {
        MSG_SERIALADAPTER_INFO( "*WARNING* Select timeout." );
      }
      break; // timeout
    }
  }

  return bytes_read;
};

bool SerialAdapter::WaitForReply( int header_value, int payload_length ) {
  int read_amount;
  int timeout = 10000;

  while( timeout-- > 0 ) {
    // Find header
    read_amount = this->Read( m_header, 2 );
    if( read_amount != 2 ) {
      if( !m_surpress_warnings ) {
        MSG_SERIALADAPTER_INFO( "*WARNING* Header size miss-match.  Received " << read_amount );
      }
      return false;
    }

    if( m_header[ 1 ] > 0 ) {
      read_amount = this->Read( m_payload, m_header[ 1 ] );
      if( read_amount != payload_length ) {
        if( !m_surpress_warnings ) {
          MSG_SERIALADAPTER_INFO( "*WARNING* Malformed payload length : Expected " << payload_length << " : Received " << read_amount );
        }
        return false;
      }
    }

    // Check this packet is the command response.
    if( m_header[ 0 ] == header_value ) {
      return true;
    }
  }

  if( !m_surpress_warnings ) {
    MSG_SERIALADAPTER_INFO( "*WARNING* WaitForReply Timeout." );
  }

  return false;
};

// Gets the type of adapter.
bool SerialAdapter::GetType() {
  m_header[ 0 ] = HEADER_GET_TYPE;
  m_header[ 1 ] = 0x00;

  if( this->Write( m_header, 2 ) != 2 ) {
    return false;
  }

  // Wait for the reply : Reply contains packet size 1
  if( !this->WaitForReply( HEADER_GET_TYPE, 1 ) ) {
    return false;
  }

  m_type = m_payload[ 0 ];

  return true;
};

bool SerialAdapter::GetVersion() {
  m_header[ 0 ] = HEADER_VERSION;
  m_header[ 1 ] = 0x00;

  if( this->Write( m_header, 2 ) != 2 ) {
    return false;
  }

  // Wait for the reply : Reply contains packet size 2
  if( !this->WaitForReply( HEADER_VERSION, 2 ) ) {
    return false;
  }

  m_version_major = m_payload[ 0 ];
  m_version_minor = m_payload[ 1 ];

  return true;
};

bool SerialAdapter::GetBaudrate() {
  m_header[ 0 ] = HEADER_BAUDRATE;
  m_header[ 1 ] = 0x00;

  if( this->Write( m_header, 2 ) != 2 ) {
    return false;
  }

    // Wait for the reply : Reply contains packet size 1
  if( !this->WaitForReply( HEADER_BAUDRATE, 1 ) ) {
    return false;
  }

  m_baudrate = m_payload[ 0 ] * 1000;

  return true;
};

int SerialAdapter::GetStatus() {
  m_header[ 0 ] = HEADER_STATUS;
  m_header[ 1 ] = 0x00;

  if( this->Write( m_header, 2 ) != 2 ) {
    return -1;
  }

  // Wait for the reply : Reply contains packet size 1
  if( !this->WaitForReply( HEADER_STATUS, 1 ) ) {
    return -1;
  }

  return m_payload[ 0 ];
};

bool SerialAdapter::Reset() {
  m_header[ 0 ] = HEADER_RESET;
  m_header[ 1 ] = 0x00;

  return ( this->Write( m_header, 2 ) == 2 );
};

bool SerialAdapter::Start() {
  m_header[ 0 ] = HEADER_START;
  m_header[ 1 ] = 0x00;


  if( this->Write( m_header, 2 ) != 2 ) {
    m_status = -1;
    return false;
  }

  m_status = 1;
  return true;
};

bool SerialAdapter::IsRunning() {
  return ( m_status == 1 );
};

void SerialAdapter::DumpData( bool showpayload ) {

  MSG_SERIALADAPTER_INFO( "Header..." );
  std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0');
  std::cout << m_header[ 0 ] << " : " << m_header[ 1 ] << std::endl;

  if( showpayload ) {
    MSG_SERIALADAPTER_INFO( "Payload..." );

    int i, j, k;

    for( i = 0; i < m_header[ 1 ]; i += 16 ) {
      std::cout << std::endl << "0x" << std::hex << std::setw(8) << std::setfill('0') << i;

      for( j = 0, k = 0; k < 16; j++, k++ ) {
        if( i+j < m_header[ 1 ] ) {
          std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << m_payload[ i+j ];
        } else {
          std::cout << "  ";
        }
        std::cout << " ";
      }
      std::cout << " ";
      for( j = 0, k = 0; k < 16; j++, k++ ) {
        if( i+j < m_header[ 1 ] ) {
          if( ( m_payload[ i+j ] < 32 ) || ( m_payload[ i+j ] > 126 ) ) {
            std::cout << ".";
          } else {
            std::cout << std::dec << m_payload[ i+j ] << std::hex;
          }
        }
      }
    }
    std::cout << std::endl;
  }

  std::cout << std::nouppercase << std::dec;
};

bool SerialAdapter::SendControlReply( unsigned char* ptr_control_reply, unsigned char control_reply_size ) {
  m_payload_out[ 0 ] = HEADER_CONTROL_DATA;
  m_payload_out[ 1 ] = control_reply_size;

  std::memcpy( &m_payload_out[ 2 ], ptr_control_reply, control_reply_size );

  if( this->Write( m_payload_out, control_reply_size + 2 ) == control_reply_size + 2 ) {
    return true;
  }

  return false;
};

bool SerialAdapter::SendInterruptReply( unsigned char* ptr_interrupt_reply, unsigned char interrupt_reply_size ) {
  m_payload_out[ 0 ] = HEADER_IN_REPORT;
  m_payload_out[ 1 ] = interrupt_reply_size;

  std::memcpy( &m_payload_out[ 2 ], ptr_interrupt_reply, interrupt_reply_size );

  if( this->Write( m_payload_out, interrupt_reply_size + 2 ) == interrupt_reply_size + 2 ) {
    return true;
  }

  return false;
};

int SerialAdapter::Poll() {
  if( m_filedescriptor == -1 ) {
	MSG_SERIALADAPTER_DEBUG("WARNING* Polling adapter but it's not been established.");
	m_status = -1;
	return -1;
  }

  int poll_result = poll( m_poll_fds, 1, m_poll_timeout_msecs );
  int header_size;

  if( poll_result > 0 )  {
    if( m_poll_fds[ 0 ].revents & POLLIN ) {
      header_size = this->Read( m_header, 2 );
      if( header_size == 2 ) {
        if( m_header[ 1 ] > 0 ) {
          m_payload_length = m_header[ 1 ];
          this->Read( m_payload, m_payload_length );
          if( m_header[ 0 ] == HEADER_START ) {
            MSG_SERIALADAPTER_INFO( "Adapter replied as started." );
          } else if( m_header[ 0 ] == HEADER_CONTROL_DATA || m_header[ 0 ] == HEADER_OUT_REPORT ) {
            return 1;
          }
        } else {
          if( !m_surpress_warnings ) {
            MSG_SERIALADAPTER_INFO( "*WARNING* Header received without payload..." );
            std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0');
            std::cout << m_header[ 0 ] << " : " << m_header[ 1 ] << std::endl;
            std::cout << std::dec;
          }
        }
      } else {
        if( !m_surpress_warnings ) {
          MSG_SERIALADAPTER_INFO( "*WARNING* Header size error! - No payload? : Size = " << header_size );
        }
      }
    }

    if( m_poll_fds[ 0 ].revents & (POLLERR | POLLHUP | POLLNVAL) ) {
      // Error noted on file descriptor
      MSG_SERIALADAPTER_ERROR( "Polling issue.  Check adapter." );

      this->Close();

      return 0;
    }
  }

  return 0;
};

int SerialAdapter::RemoveAcl(const char* path) {
    // Get the current ACL for the file
    acl_t acl = acl_get_file(path, ACL_TYPE_ACCESS);
    if (acl == NULL) {
        MSG_SERIALADAPTER_INFO("Warning:  Problem getting ACL.");
        return -1;
    }

    // Create an empty ACL object
    acl_t empty_acl = acl_init(1); // Initialize with a single empty ACL entry
    if (empty_acl == NULL) {
        MSG_SERIALADAPTER_INFO("Warning: Problem initializing empty ACL");
        acl_free(acl);
        return -1;
    }

    // Set the empty ACL back to the file (removes ACL)
    int result = acl_set_file(path, ACL_TYPE_ACCESS, empty_acl);
    if (result == -1) {
        MSG_SERIALADAPTER_INFO("Warning: Problem setting empty ACL.");
        acl_free(acl);
        acl_free(empty_acl);
        return -1;
    }

    // Free the ACL object after operation
    acl_free(acl);
    acl_free(empty_acl);
    return 0;
}
