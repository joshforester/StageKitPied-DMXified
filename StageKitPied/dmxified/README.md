# Introduction

_**StageKitPied-DMXified**_ is an expansion of capability of StageKitPied that allows one to map the Xbox 360 lighting,
strobe, and fogger instructions sent to the Rock Band StageKit to arbitrary DMX device instructions.  Additionally, one
can map the presence/absence of files on the file system to arbitrary DMX device instructions.  

Since the most intuitive use of this latter feature is a stomp/button box attached to the GPIO pins of the Raspberry Pi
running _**StageKitPied-DMXified**_, system service scripts that create and delete files based on GPIO-wired button
presses are provided in this code for your convenience.  This feature is, however, not limited to button boxes.  Since
the input is simply the absence/presence of a file, any input (i.e. a web service, home automation device, someone
interactively creating/removing the file at a command prompt, etc.) can be used to trigger a mapped DMX output.

Because both Xbox Rock Band Stage Kit instructions and file absence/presence can be competing inputs for lighting
instructions sent to DMX devices, the mapping configuration for _**StageKitPied-DMXified**_ also supports the overriding
of conflicting instructions via a prioritization configuration.

## Getting Started

### Documentation

Because this configuration is powerful, it can be complicated to get it exactly how you want it.  My personal
mapping file, QLC Workspace, and QLC Fixture files are in the codebase as a working example.  Additionally, I do my best
to document the format _**StageKitPied-DMXified**_ expects of the mapping file expects here.

### Testing/Demo Applications

There are standalone demo applications in the `demo` folder whose usage is documented in
[StageKitPied/dmxified/demo/README.md](demo/README.md).

These applications can assist to:
* ensure _**StageKitPied-DMXified**_ can connect to your QLC+ instance
* vet your mapping file's syntax
* perform a "dry-run" to show how _**StageKitPied-DMXified**_ would process a specified series of input events given
your mapping file
* perform an actual run with your lighting setup given a specified series of input events

The standalone applications' usage is documented in the aforementioned README, and each of the standalone demo
applications come with their own event series files and mapping files, which you can tweak to suit your learning/testing
needs.

# DMXified Mapping File

## Overview

Distilled down to its simplest form, the mapping file is simply a list of mappings of inputs to outputs.  Each input
can have one or more outputs, and outputs can either be an Xbox Stage Kit instruction or a QLC+ instruction.  Stage Kit
Instruction types are `Handle_LEDUpdate`, `Handle_StrobeUpdate`, and `Handle_FogUpdate`.  QLC+ instructions are
`basicWidgetValueSet`, `simpleDeskChannelSet`, `setFunctionStatus`.  These QLC+ instructions refer to setting widget
values in a QLC Workspace virtual console, channel values in the QLC+ simple desk, or using functions within a QLC+
Workspace respectively.

_**Note:**  For DMX device outputs, **StageKitPied-DMXified** simply interfaces with a containerized QLC+ instance running
on the Raspberry Pi set up during install.  Because QLC+ is robust software built to manage and control DMX devices, it
is expected that you create a workspace there, then reference controls within it for devices, scenes, animations, etc.
via a QLC+ instruction in the **StageKitPied-DMXified** mapping file._

## Examples

The mapping file that I personally use and that is installed by default can be found at
[StageKitPied/dmxified_mapping.xml](../dmxified_mapping.xml).  Below are a few examples from it alongside an
explanation.

### Example 1 - Standard Stage Kit Input

#### Configuration

Below is an example of a standard Stage Kit Input, in which _**StageKitPied-DMXified**_ is receiving an `SK_LED_BLUE`
instruction:

```xml
<mapping>
    <input type="stageKitInput" id="SK_LED_BLUE" priority="1" />
    <outputs>
        <output type="stageKitOutput" subtype="Handle_LEDUpdate">
            <value>SK_LED_BLUE</value>
        </output>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>2</widgetId>
            <value>1</value>
        </output>
    </outputs>
</mapping>
```

#### Input

Because `priority=1`, this is considered a lowest-priority event, and will easily be overridden by other `mapping`
elements with the same/higher `priority` value in the event of an `output` conflict.

#### Outputs

##### Stage Kit Instruction

For the `stageKitOutput`, a `subtype` of `Handle_LEDUpdate` is used for
PDP StageKit Pod LED updates, and _**StageKitPied-DMXified**_ does the correct thing when it comes to processing the
exact LED updates that are contained within the Xbox Stage Kit instruction input (which specific LEDs to update on the
pod and so forth).

For a full list of the potential `subtype` for the `Handle_LEDUpdate` instruction refer to Example 3 Outputs section
below.

##### QLC+ Instruction

For the `qlcplusOutput`, a `subtype` of `basicWidgetValueSet` is used with a `widgetId` of `2` and a `value` of `1`.
This instructs _**StageKitPied-DMXified**_ to use its Websocket connection to QLC+ to send a command set the value of
widget `2` in the QLC Workspace Virtual Console to `1`.  Since in my QLC Workspace (files in
[installer/qlc](../../installer/qlc)), the widget with an `id` of `2` is a button, this Websocket call is simply setting
the button state to 'on'.

Without going into the details of the QLC+ Workspace file since it is beyond the scope of this documentation, the
`widgetId` this `qlcplusOutput` is referencing can be found in the workspace file (also XML).  For example, here is the
corresponding section in [installer/qlc/qlcplusSkpDmx.qxw](../../installer/qlc/qlcplusSkpDmx.qxw):

```
<Button Caption="SK_LED_BLUE" ID="2" Icon="">
    <WindowState Visible="False" X="30" Y="100" Width="130" Height="35"/>
    <Appearance>
        <FrameStyle>None</FrameStyle>
        <ForegroundColor>Default</ForegroundColor>
        <BackgroundColor>Default</BackgroundColor>
        <BackgroundImage>None</BackgroundImage>
        <Font>Default</Font>
    </Appearance>
    <Function ID="1"/>
    <Action>Toggle</Action>
    <Intensity Adjust="False">100</Intensity>
</Button>
```
Note it is the value of the `ID` attribute on the `Button` element, since this widget is a button.

In my QLC+ Workspace setup, this button toggles a function (`<Function ID="1">` above) that turns my PAR lights blue, as
well as my bar lights.  It additionally turns off all other light colors.

I find that using a simple button widget, and attaching it to a more function in the QLC+ Workspace allows for a simple
_**StageKitPied-DMXified**_ mapping file, while allowing for a robust set of DMX commands via the function and still
allows manually testing the effect via the QLC+ Virtual Console since I can interactively press the button as well.
Further, I can VNC into my QLC+ container and see the status of the widgets live (the buttons will flicker on and off as
_**StageKitPied-DMXified**_ is sending QLC+ commands over its Websocket API).  That said, _**StageKitPied-DMXified**_
allows for mapping to controlling functions or basic channel values as well, so feel free to design your setup however
you desire.

### Example 2 - File Input

Below I show an example stemming from my use of a button box hard-wired to GPIO on my Raspberry Pi, whereby button
presses are received by a service
([installer/buttons/switch_touch_file_buttons.service](../../installer/buttons/switch_touch_file_buttons.service)) that
creates/deletes a file.  That file's creation/deletion is configured by the mapping file (shown below) to turn all of my
DMX devices into "Sound Mode" (a common setting on DMX devices that listens for beats in the music and illuminates in a
pre-configured way set by the DMX device's manufacturer).  

_Sound Mode (generic response to music) is not as cool as the Xbox 360 Stage Kit instructions (specifically coded to
songs), but some custom songs do not have StageKit instructions programmed in, so I have this button because it is better than
idling lights._

#### Configuration

Below is an example of a File Input, in which _**StageKitPied-DMXified**_ is monitoring the creation and deletion of a
file on the Raspberry Pi's file system.  Take note that for `input` elements of `type=fileExistsInput`, there must
always be two `mapping` elements:  one for the file creation and one for its deletion.  

The creation `mapping` may have any `id` you specify, whereas the deletion `mapping` must have the same name as the
corresponding creation `mapping`, but with the string "Off" appended to it.  For example, below you will see the `id` of
the creation `mapping` is "soundModeButton" and, consequently, the `id` for the corresponding deletion `mapping` is
`soundModeButtonOff`.

```xml
<mapping>
    <input type="fileExistsInput" id="soundModeButton" priority="100">
        <file>/opt/StageKitPied/input/button12.tmp</file>
        <outputOverrideList>
            <output type="qlcplusOutput" subtype="basicWidgetValueSet" />
            <output type="qlcplusOutput" subtype="setFunctionStatus" />
            <output type="qlcplusOutput" subtype="simpleDeskChannelSet" />
        </outputOverrideList>
    </input>
    <outputs>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>20</widgetId>
            <value>1</value>
        </output>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>5</widgetId>
            <value>1</value>
        </output>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>8</widgetId>
            <value>1</value>
        </output>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>14</widgetId>
            <value>1</value>
        </output>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>22</widgetId>
            <value>1</value>
        </output>
    </outputs>
</mapping>
<mapping>
    <input type="fileExistsInput" id="soundModeButtonOff" priority="100" />
    <outputs>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>23</widgetId>
            <value>1</value>
        </output>
    </outputs>
</mapping>
```

#### Input

Note `priority=100` here.  This means that where there is a conflict between an `output` in the `outputOverrideList`
here and the `output` of another `mapping` whose `input` has a lower `priority`, the `output` element of this `input`
will supersede.  A conflict is defined as `output` elements with the same `type`/`subtype` combination.

##### File

This specifies the path of the file that is being monitored for creation by this `mapping` and for deletion by the
corresponding `mapping` whose `input` `id` value appends the string "Off".

That is, in this case, the `input` with `id` of "soundModeButton" is monitoring `/opt/StageKitPied/input/button12.tmp`
for creation, and the `input` with `id` of "soundModeButtonOff" is monitoring `/opt/StageKitPied/input/button12.tmp` for
deletion.

In my case, this file is tied to a file created/deleted when I press (for creation) and press again (for deletion) a
button on my button box (via the configured
[installer/buttons/switch_touch_file_buttons.service](../../installer/buttons/switch_touch_file_buttons.service)).

##### Output Override List

These are all the `output` elements that will be overridden as described above in the Input section.  What this means
for this example is for every other `mapping` with an `input` whose `priority` value is less than 100, its `output`
configuration will be overridden for the following `type`/`subtype` combinations:
* "qlcplusOutput"/"basicWidgetValueSet"
* "qlcplusOutput"/"setFunctionStatus"
* "qlcplusOutput"/"simpleDeskChannelSet"

I'm effectively clearing out QLC+ configuration here, then setting it via the `outputs` described below.

#### Outputs

For the QLC+ Workspace that ships with _**StageKitPied-DMXified**_ (and the one I personally used), the Virtual Console
simply contains multiple groups of buttons.  These groups control DMX lights, fog, and strobe pretty much one-to-one
with Xbox Stage Kit instructions.  Separately on the Virtual Console, I have special button groups that turn on/off
lighting functions that I have specially configured for buttons on my button box and also for idling.  So in my setup,
when I press the button for "Sound Mode", I first want to turn off all lights, fog, strobe, and idle functions in QLC+.
Then, I want to turn on my "Sound Mode" programming.

Given the explanation of purpose above, you will see from the mapping that there are a lot of `output` elements of with
a `subtype` value of "basicWidgetValueSet" whose `value` subelement is set to "1".  That is because I have buttons in
each button group that when pressed turn _off_ the lights, fog, or strobe DMX functions turned on by the other buttons
in the group.  Though a little counterintuitive from a `mapping` configuration perspective, it allows me to simply press
one button (have one `output`) to reset the settings established from all the buttons of the button group.  Thus, there
are only four `output` elements in the `outputs` list above versus ten that would otherwise be required.

In summary, after the `outputOverrideList` in the `input` overrides QLC+ instructions to set lights, fog, and strobe
events, the first four `output` elements essentially press the buttons to reset DMX lights, fog, strobe, and idle state
to off.  These both prevent future settings and current setup from screwing up "Sound Mode" turned on by the last
`output`, which pressed the "Sound Mode On" button on the Virtual Console.

##### QLC+ Instruction

As explained in Example 1.

### Example 3 - Nullified Stage Kit Input

#### Configuration

Below is an example of a nullified Stage Kit Input, in which _**StageKitPied-DMXified**_ is receiving an `SK_FOG_ON`
instruction:

```xml
<mapping>
    <input type="stageKitInput" id="SK_FOG_ON" priority="1" />
    <outputs>
        <!-- I personally am using a fileExistsInput to control fog, so this is no longer necessary. -->
        <!--
                    <output type="stageKitOutput" subtype="Handle_FogUpdate">
                        <value>true</value>
                    </output>
        -->
    </outputs>
</mapping>
```

_**Note:** The Xbox will at times send out 'SK_FOG_ON' events during gameplay when it thinks you need more fog._

#### Input

As with Example 1, because `priority=1`, this is considered a lowest-priority event, and will easily be overridden by
other `mapping` elements with the same/higher `priority` value in the event of an `output` conflict.  However, this is a
moot point, since no `outputs` are configured for this `mapping`.

#### Outputs

As seen above, the outputs are commented out.  Normally, a `stagetKitInput` with a `SK_FOG_ON` subtype would be mapped
to a `Handle_FogUpdate`.  I have disabled any action here because I did not want the Xbox Stage Kit to blow fog unless I
explicitly ask it to via the button box I created for _**StageKitPied-DMXified**_.  As such, fogger control is
configured via a `fileExistsInput` tied to a file created/deleted when I press or release a button on my button box (via
the configured
[installer/buttons/active_touch_file_buttons.service](../../installer/buttons/active_touch_file_buttons.service)).  Any
instructions from the Xbox for the fogger are nullified by the configuration here.

_**Note:**  It is mandatory to include a configuration for **ALL** instructions that the Xbox may send to
**StageKitPied-DMXified**, even if they are nullifying by the `mapping`, as seen here.  Below is a complete list of
instructions that the Xbox will send to **StageKitPied-DMXified**:_

* SK_FOG_ON
* SK_FOG_OFF
* SK_STROBE_SPEED_1
* SK_STROBE_SPEED_2
* SK_STROBE_SPEED_3
* SK_STROBE_SPEED_4
* SK_STROBE_OFF
* SK_LED_BLUE
* SK_LED_GREEN
* SK_LED_YELLOW
* SK_LED_RED
* SK_LED_MASK
* SK_ALL_OFF

And 3 more custom instructions that _**StageKitPied-DMXified**_ creates and will need to be configured in the mapping
file:

* SERIAL_RESTART
* IDLE_ON
* IDLE_OFF

### Example 4 - Idle On

The following events are made up by _**StageKitPied-DMXified**_ and never actually sent by the Xbox.  That said, it is
useful to have lighting feedback for when they occur and/or can improve the gameplay experience:
* IDLE_ON
* IDLE_OFF
* SERIAL_RESTART

#### Configuration

Below is an example of a _**StageKitPied-DMXified**_-generated Stage Kit Input instruction for `IDLE_ON` (for more info
on the conditions in which this event occurs, please see the comments in [StageKitPied/lights.ini](../lights.ini),
`[NO_EVENT_DATA]` section, above the `IDLE_SECONDS` setting):

```xml
<mapping>
    <input type="stageKitInput" id="IDLE_ON" priority="1" />
    <outputs>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>5</widgetId>
            <value>1</value>
        </output>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>8</widgetId>
            <value>1</value>
        </output>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>14</widgetId>
            <value>1</value>
        </output>
        <output type="qlcplusOutput" subtype="basicWidgetValueSet">
            <widgetId>19</widgetId>
            <value>1</value>
        </output>
        <output type="stageKitOutput" subtype="Handle_LEDUpdate">
            <value>SK_ALL_OFF</value>
        </output>
    </outputs>
</mapping>
```

#### Input

As with Example 1, because `priority=1`, this is considered a lowest-priority event, and will easily be overridden by
other `mapping` elements with the same/higher `priority` value in the event of an `output` conflict.  Because we are
idling, and breaking that idle event relies on any non-FOG_OFF event, this is the correct setting.

#### Outputs

In this setup, after 20 seconds of no non-FOG_OFF events being sent to _**StageKitPied-DMXified**_ (again, set in the
[StageKitPied/lights.ini](../lights.ini) file referenced in the Example 4 intro), we are assuming the game is idling
between songs or otherwise in a menu screen or something.

As a result, I clear the light, fog, and strobe DMX settings and turn on the UV/black lights via buttons in my QLC+
virtual console.  Since the Rock Band room is pretty dark, I find the UV/black lights are a good transition without
spoiling the mood.

## Mapping File Backus-Naur Form

The format of the configuration file, as expressed in
[Backus-Naur Form](https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form) is as follows (if you are not familiar with
this notation, don't sweat it; just know that it is another way of specifying the mapping file configuration format that
makes things understandable to some people).

### Format

```bnf
<document>            ::= <root-element>

<root-element>        ::= "<root>" <mapping-list> "</root>"

<mapping-list>        ::= <mapping> <mapping-list>
                        |  ε                       // empty list

<mapping>             ::= "<mapping>" <input-element> <outputs-element> "</mapping>"

-----------------------------------------------------------------
-- INPUT ELEMENT -------------------------------------------------
-----------------------------------------------------------------
<input-element>       ::= <self-closing-input>
                        |  <container-input>

<self-closing-input>  ::= "<input" <input-attributes> "/>"

<container-input>     ::= "<input" <input-attributes> ">"
                         <input-children>
                         "</input>"

<input-attributes>    ::= <attr-type> <attr-id> <attr-priority>
                        |  <attr-type> <attr-id>                // priority optional
                        |  <attr-id>   <attr-type> <attr-priority>
                        |  ... (any permutation of the three)

<attr-type>           ::= "type=" <quoted-string>          // "stageKitInput" | "fileExistsInput"
<attr-id>             ::= "id="   <quoted-string>
<attr-priority>       ::= "priority=" <integer>

<input-children>      ::= <file-element> <output-override-list>?
                        |  <output-override-list> <file-element>?
                        |  <output-override-list>
                        |  <file-element>
                        |  ε

<file-element>        ::= "<file>" <character-data> "</file>"

<output-override-list> ::= "<outputOverrideList>" <override-output-list> "</outputOverrideList>"
<override-output-list> ::= <override-output> <override-output-list>
                        |  ε
<override-output>      ::= "<output" <override-attributes> "/>"
<override-attributes> ::= "type=" <quoted-string> "subtype=" <quoted-string>

-----------------------------------------------------------------
-- OUTPUTS CONTAINER ---------------------------------------------
-----------------------------------------------------------------
<outputs-element>     ::= "<outputs>" <output-list> "</outputs>"
<output-list>         ::= <output> <output-list>
                        |  ε

<output>              ::= "<output" <output-attributes> ">"
                         <output-children>
                         "</output>"

<output-attributes>   ::= "type=" <quoted-string> "subtype=" <quoted-string>
                        // ── type → allowed subtypes (see “Constraints” section below)

-----------------------------------------------------------------
-- INDIVIDUAL OUTPUT ---------------------------------------------
-----------------------------------------------------------------
<output-children>     ::= <parameter-list> | ε
<parameter-list>      ::= <parameter> <parameter-list> | ε
<parameter>           ::= <widgetId-elem>
                        |  <value-elem>
                        |  <functionId-elem>
                        |  <status-elem>
                        |  <absoluteDmxAddress-elem>

<widgetId-elem>          ::= "<widgetId>"            <integer> "</widgetId>"
<value-elem>             ::= "<value>"               <value-content> "</value>"
<functionId-elem>        ::= "<functionId>"          <integer> "</functionId>"
<status-elem>            ::= "<status>"              <integer> "</status>"
<absoluteDmxAddress-elem>::= "<absoluteDmxAddress>"  <integer> "</absoluteDmxAddress>"

<value-content>          ::= <quoted-string>                   // e.g. "1", "true", "SK_LED_RED"
<integer>                ::= <digit> { <digit> }*
<quoted-string>          ::= '"' { any-char‑except-'"' } '"'
<character-data>         ::= { any-char‑except-'<', '&' }
<digit>                  ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
```

### Constraints

If `type` = "stageKitInput", then `id` must be one of:
```
{
    "SK_FOG_ON",
    "SK_FOG_OFF",
    "SK_STROBE_SPEED_1",
    "SK_STROBE_SPEED_2",
    "SK_STROBE_SPEED_3",
    "SK_STROBE_SPEED_4",
    "SK_STROBE_OFF",
    "SK_LED_BLUE",
    "SK_LED_GREEN",
    "SK_LED_YELLOW",
    "SK_LED_RED",
    "SK_LED_MASK",
    "SK_ALL_OFF",
    "SERIAL_RESTART",
    "IDLE_ON",
    "IDLE_OFF"
}
```

If `type` = "stageKitOutput", then `subtype` must be one of:
```
{ 
    "Handle_LEDUpdate",
    "Handle_StrobeUpdate",
    "Handle_FogUpdate"
}
```

If `type` = "qlcplusOutput", then `subtype` must be one of
```
{
    "basicWidgetValueSet",
    "simpleDeskChannelSet",
    "setFunctionStatus"
}
```

If `type` = "qlcplusOutput" and `subtype` = "basicWidgetValueSet", then the following subelements must be configured:
* `<widgetId>`
* `<value>`

If `type` = "qlcplusOutput" and `subtype` = "simpleDeskChannelSet", then the following subelements must be configured:
* `<absoluteDmxAddress>`
* `<value>`

If `type` = "qlcplusOutput" and `subtype` = "setFunctionStatus", then the following subelements must be configured:
* `<functionId>`
* `<status>`
