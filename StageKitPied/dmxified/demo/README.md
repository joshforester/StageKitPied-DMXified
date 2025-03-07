# StageKitPied-DMXified Demo Programs 

This folder contains programs for verifying your StageKitPied-DMXified setup.  The following demo programs are helpful for ensuring your setup is working and configured properly:
* __Interactive Qlcplus Websocket Demo (__ `iqwd` __)__  - This is a program to simply test your connection to Qlcplus Websocket port.  When successfuly, you'll have verified that Qlcplus's Websocket API is enabled and that your Raspberry Pi can connect to it over the network.
* __XML Config Reader Demo (__ `xcrd` __)__  - This is a program that reads in a mapping file XML and prints it out.  This validates the XML, and somewhat validates that the file has the expected format.  There  _may_  be attribute value problems in the file that won't be necessarily caught by this program, but it's a great place to start.
* __Event Engine Simulator Demo (__ `eesd` __)__  - This is a program that simulates what decisions StageKitPied-DMXified will make given your mapping file XML and a sequence of events.  This program will also find problems with the mapping file XML file that the XML Config Reader Demo did not.
* __Qlcplus Output Simulator Demo (__ `qosd` __)__  - This is a program that simulates what decisions StageKitPied-DMXified will make given your mapping file XML and a sequence of events.  However, this program will actually process <qlcOutput> outputs using Qlcplus's Websocket API.  Additionally, it respects <fileExistsInput> file watchers in the mapping file XML, responding to file system events during execution.

## Configuration

### Interactive Qlcplus Websocket Demo (`iqwd`)

#### IQWD

None required.  It is an interactive shell.  To begin, type `help`.

#### QLCPlus (QLC+) Server Configuration

Ensure QLCPlus is launched with the `--web` parameter.  For details, see the
[QLC+ Command Line Parameters Page](https://www.qlcplus.org/docs/html_en_EN/commandlineparameters.html).

#### Logging

TODO.

### XML Config Reader Demo (`xcrd`)

#### XCRD

None required.  If you are passing in a mapping file to test, that will need to be correct mapping file XML (please see `xcrd_demo_mapping.xml` for an example of what is possible).

#### Logging

TODO.

### Event Engine Simulator Demo (`eesd`)

#### EESD

None required.  If you are passing in a mapping file to test, that will need to be correct mapping file XML (please see `eesd_demo_mapping.xml` for an example of what is possible).  

Further, if you are passing in an event file to test, that will need to be the correct format (please see `eesd_demo_events.txt` for an example of what is possible).  Note that the event file is simply an ordered list of ids.  Each id should reference a unique id attribute from an input element in the mapping file XML used.  Please review `eesd_demo_events.txt`, as it references id attributes from input elements in `eesd_demo_mapping.xml`.

### Qlcplus Output Simulator Demo (`qosd`)

#### QOSD

None required.  If you are passing in a mapping file to test, that will need to be correct mapping file XML (please see `qosd_demo_mapping.xml` for an example of what is possible).  

Further, if you are passing in an event file to test, that will need to be the correct format (please see `qosd_demo_events.txt` for an example of what is possible).  Note that the event file is simply an ordered list of ids.  Each id should reference a unique id attribute from an input element in the mapping file XML used.  Please review `qosd_demo_events.txt`, as it references id attributes from input elements in `qosd_demo_mapping.xml`.

#### QLCPlus (QLC+) Server Configuration

If your QLC+ instance is ready to accept the Websocket API commands, you'll see lighting updates as the simulator runs. 

TODO:  explain strategies for QLC configuration

#### Logging

Output for this program is done via the DEBUG build macro, which is already configured for the `qosd` target and dependent code during this application's build.  Thus, there's no additional logging configuration necessary here--you should be able to read along as to what the EventEngine is deciding (based on the provided mapping file XML and event file) upon running QOSD on the command line.

## Deploying

Programs included in the repo should run on an AARCH64 architecture, as can be deployed on Raspberry Pi 4B, for example.

## Building

Install the library dependencies:

```
apt-get install libusb-1.0.0-dev libacl1-dev libwebsocketpp-dev libboost-dev-all libssl-dev librapidxml-dev
```

After that, should be able to build it with:

```
make
```

## Running

### Interactive Qlcplus Websocket Demo (`iqwd`)

```
#> ./iqwd
```

From within the program, it may be helpful to know what messages need to be sent to QLC+ once fixtures and inputs/outputs are configured, and scenes and widgets are setup.

By default, QLC+ will be listening for Websocket clients on the following (which can be used with the iqwd `connect` command):

```
ws://127.0.0.1:9999/qlcplusWS
```

Once connected (and confirmed via the iqwd `show` command), different QLC+ websocket API commands can be issued via the iqwd `send` command.

For QLC+ websocket API, see the [QLC+ Web API Test Page](https://www.qlcplus.org/Test_Web_API.html).

Note that the API isn't explicitly documented there per se, but can be reverse engineered.  From it, one can glean messages to `send <message>`, for example:

getWidgetType:

```
QLC+API|getWidgetType|9
```

setFunctionStatus(functionId, status):

```
QLC+API|setFunctionStatus|0|1
```

Basic widget value set:

```
4|1
```

setSimpleDeskChannel(absoluteDmxAddress, value):

```
CH|2|255
```

### XML Config Reader Demo (`xcrd`)

```
#> ./xcrd
```

This will run the demo using the included `xcrd_demo_mapping.xml` file.  If you'd like to test out your own file, execute it as follows:

```
#> ./xcrd path/to/your/mapping_file.xml
```

### Event Engine Simulator Demo (`eesd`)

```
#> ./eesd
```

This will run the demo using the included `eesd_demo_mapping.xml` and `eesd_demo_events.txt` files.  If you'd like to test out your own file[s], execute it as follows:

```
#> ./eesd path/to/your/mapping_file.xml path/to/your/event_file.txt
```

### Qlcplus Output Simulator Demo (`qosd`)

```
#> ./qosd
```

This will run the demo using the included `qosd_demo_mapping.xml` and `qosd_demo_events.txt` files.  If you'd like to test out your own file[s], or have it run against a QLC+ instance running at a URL different from ws://127.0.0.1:9999/qlcplusWS, or have it run slower/faster, execute it as follows:

```
#> ./qosd path/to/your/mapping_file.xml path/to/your/event_file.txt your_qlcplus_url sleep_time
```
where:
* `your_qlcplus_url` is the websocket URL where your instance is running (default: `ws://127.0.0.1:9999/qlcplusWS`)
* `sleep_time` is the time in seconds `qosd` will sleep between processing each event in the event file