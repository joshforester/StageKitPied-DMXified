# QLC Demo 

## Configuration

### Interactive Qlcplus Websocket Demo (iqwd)

#### IQWD

None required.  It is an interactive shell.  To begin, type `help`.

#### QLCPlus (QLC+) Server Configuration

Ensure QLCPlus is launched with the `--web` parameter.  For details, see the
[QLC+ Command Line Parameters Page](https://www.qlcplus.org/docs/html_en_EN/commandlineparameters.html).

#### Logging

TODO.

## Deploying

Programs included in the repo should run on an AARCH64 architecture, as can be deployed on Raspberry Pi 4B, for example.

## Building

Install the library dependencies:

```
apt-get install libusb-1.0.0 libwebsocketpp-dev libboost-dev-all libssl-dev
```

After that, should be able to build it with:

```
make
```

## Running

### Interactive Qlcplus Websocket Demo (iqwd)

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

Note that the API isn't explicitly documented there per se, but can be reverse engineered.  From it, one can glean messages to `send <connectionId> <message>`, for example:

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
