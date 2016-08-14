# microbit-rtc_ds3231
MicroBit Clock using rtc DS3231

use MBED to import code from
https://developer.mbed.org/users/daw9000/code/microbit-rtc_ds3231/
or clone from github but you will REQUIRE as DEPENDENCIES
Libraries
MicroBit
DS3231

MicroBit-rtc_ds3231
##Clock

Copyright  David Wright 2016. DAW9000.
Feel free to copy, ammend , or do what you will but a shout out would be nice :)


This program uses the Maxim Integrated DS3231 library to create a BBC MicroBit
Clock. Requires a DS3231 Real Time clock connected to pin 19 and 20.

Recommended that an edge connector is used for DS3231 connection.

Program will display :-
    Date then Time then Temp (from DS3231 not MicroBit as it seems for
    accurate as to ambient temp.)
    Pressing Button B and keeping it pressed between rotations ie at the end
    of each display cycle will change mode cyclically.
    1. Date, Time, Temp
    2. Date only.
    3. Time only.
    4. Temp only.
I am sure better things can be done here! Improvements welcome.

Pressing Buttons A and B together and keeping pressed as above will take
you into clock setting mode.
Clock is set :-
Year, Month, Day, Hour, Minute, Second, Day of Week (1 to 7 eg Sun=7 but you can
make any day the start of week ie Sun=1).
Setting is done after DATE and again after TIME. So if only setting date
you can abort after the msg "Date Set".

If in setting mode abort changes by powering off at any time.

This again can be improved .... not the easiest to set date and time with
no keypad and limited screen!

DS3231 has 2 alarms, so alarm clock feature with speaker/music attached to
MicroBit is a good future mod.



developed by daw9000. no copywrite, free for whatever...

## Overview

The micro:bit runtime provides an easy to use environment for programming the BBC micro:bit in the C/C++ language, written by Lancaster University. It contains device drivers for all the hardware capabilities of the micro:bit, and also a suite of runtime mechanisms to make programming the micro:bit easier and more flexible. These range from control of the LED matrix display to peer-to-peer radio communication and secure Bluetooth Low Energy services. The micro:bit runtime is proudly built on the ARM mbed and Nordic nrf51 platforms.

In addition to supporting development in C/C++, the runtime is also designed specifically to support higher level languages provided by our partners that target the micro:bit. It is currently used as a support library for all the languages on the BBC www.microbit.co.uk website, including Microsoft Block, Microsoft TouchDevelop, Code Kingdoms JavaScript and Micropython languages.

## Links

[micro:bit runtime docs](http://lancaster-university.github.io/microbit-docs/) | [microbit-dal](https://github.com/lancaster-university/microbit-dal) |  [uBit](https://github.com/lancaster-university/microbit)

## Build Environments

| Build Environment | Documentation |
| ------------- |-------------|
| ARM mbed online | http://lancaster-university.github.io/microbit-docs/online-toolchains/#mbed |
| yotta  | http://lancaster-university.github.io/microbit-docs/offline-toolchains/#yotta |

## BBC Community Guidelines

[BBC Community Guidelines](https://www.microbit.co.uk/help#sect_cg)
