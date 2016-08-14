
/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

DS3231 libs
* Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
**********************************************************************
Copyright  David Wright 2016. DAW9000.
Feel free to copy, ammend , or do what you will but a shout out would be nice :)
*/

/* This program uses the Maxim Integrated DS3231 library to create a BBC MicroBit
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

*/
#include "MicroBit.h"
#include "ds3231.h"
MicroBit uBit;
int iss,imm,ihh,idow,iDD,iMM,iYY;
ManagedString ss,mm,hh,dOw,DD,MM,YY, txtMonth, txtDay, tempOut;
ManagedString tsep = ":";
ManagedString dsep = "/";
Ds3231 mRTC(I2C_SDA0, I2C_SCL0);
uint16_t rtn;
uint16_t rval;
uint16_t rtn1;
ds3231_time_t mTime = {0,0,0,false,false};  // 24 hr mode -  seconds,minutes,hours,am_pm(pm=1,am=0),mode(0=24,1=12hr)
ds3231_calendar_t mCalendar; // day(day of week),date(day),month,year
ds3231_alrm_t mAlarm; // seconds,minutes,hours, day of week, day, rate1,rate2,rate3,rate4,am_pm,mode,day_date(use dayofweek=0,use date day=1)
//Seconds and am1 not used for alarm2
uint16_t mTemp; // raw temp data
uint16_t DOW; //day of week
char * weekdaya[7]= {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"} ;
char * montha[12]= {"January","February","March","April","May","June","July","August","September","October","November","December"};

int displayMode = 0;

// predeclare functions
void getTimeAndDate();
void setTimeAndDate();
void displayTimeAndDate();
void displayDate();
void displayTime();
void displayTemp();
void displaySeconds();

void getTimeAndDate()
{
    // get date and time and populate data vars
    mTime.mode=false; //24 hour
    rval = mRTC.get_time(&mTime); // get time
    mTemp = mRTC.get_temperature(); // get temperature
    rtn = mRTC.get_calendar(&mCalendar); //get date
    YY = (int)(mCalendar.year + 2000); // 4 digit
    txtMonth = montha[((int)mCalendar.month)-1];
    txtDay = weekdaya[((int)mCalendar.day)-1];
    iss = (int)mTime.seconds;
    imm = (int)mTime.minutes;
    ihh = (int)mTime.hours;
    idow = (int)mCalendar.day;
    iDD = (int)mCalendar.date;
    iMM = (int)mCalendar.month;
    iYY = (int)mCalendar.year;
    ManagedString s1 (iss / 10);
    ManagedString s2 (iss % 10);
    ss = s1 + s2;
    ManagedString m1 (imm / 10);
    ManagedString m2 (imm % 10);
    mm = m1 + m2;
    ManagedString h1 (ihh / 10);
    ManagedString h2 (ihh % 10);
    hh = h1 + h2;
    ManagedString D1 (iDD / 10);
    ManagedString D2 (iDD % 10);
    DD = D1 + D2;
    ManagedString M1 (iMM / 10);
    ManagedString M2 (iMM % 10);

    // lets setup the Temperature here too
    mTemp = mTemp / 256;
    ManagedString tmp(mTemp);
    tempOut=(tmp + "degC");


}
void setTimeAndDate()
{
    int rVal;
    int setYear, setMonth, setDay, setDOW, setHours, setMinutes, setSeconds;
    bool setMode, setAM_PM;
    getTimeAndDate(); // get latest

    uBit.display.clear();
    uBit.display.scroll(" Set Date & Time?");
    uBit.sleep(1000);
    uBit.display.clear();
    uBit.display.scroll(" Btn A & Btn B to change values");

    setYear = iYY;
    uBit.display.scroll("Year(yy):" + setYear);
    while (!(uBit.buttonAB.isPressed() )) {
        if (uBit.buttonA.isPressed()) {
            setYear--;
        }
        if (uBit.buttonB.isPressed()) {
            setYear++;
        }
        if (setYear < 0) setYear=99;
        if (setYear > 99) setYear=0;
        uBit.display.scroll(setYear);
    }

    setMonth = iMM;
    uBit.display.scroll("Month:" + setMonth);
    while (!(uBit.buttonAB.isPressed() )) {
        if (uBit.buttonA.isPressed()) {
            setMonth--;
        }
        if (uBit.buttonB.isPressed()) {
            setMonth++;
        }
        if (setMonth < 1) setMonth=12;
        if (setMonth > 12) setMonth=1;
        uBit.display.scroll(setMonth);
    }

    setDay = iDD;
    uBit.display.scroll("Day:" + setDay);
    while (!(uBit.buttonAB.isPressed())) {
        if (uBit.buttonA.isPressed()) {
            setDay--;
        }
        if (uBit.buttonB.isPressed()) {
            setDay++;
        }
        if (setDay < 1) setDay=31;
        if (setDay > 31) setDay=1; // make better using month?
        uBit.display.scroll(setDay);
    }

    setDOW = idow;
    uBit.display.scroll("Day of week(1-7):" + setDOW);
    while ( !(uBit.buttonAB).isPressed()) {
        if ( uBit.buttonA.isPressed() ) {
            setDOW--;
        }
        if (uBit.buttonB.isPressed()) {
            setDOW++;
        }
        if (setDOW < 1) setDOW=7;
        if (setDOW > 7) setDOW=1;
        uBit.display.scroll(setDOW);
    }

    uBit.display.scroll("Press AB to set Date");
    uBit.display.scroll("Press RESET to abort");
    while ( !(uBit.buttonAB.isPressed()) ) {
        uBit.display.scroll(setDay);
        uBit.display.scroll("/");
        uBit.display.scroll(setMonth);
        uBit.display.scroll("/");
        uBit.display.scroll(setYear);
    }

    if (uBit.buttonAB.isPressed()) {
        // set date
        mCalendar.day = setDOW;
        mCalendar.date = setDay;
        mCalendar.month = setMonth;
        mCalendar.year = setYear;
        rVal = mRTC.set_calendar(mCalendar);
    }
    if (rVal == 0) {
        uBit.display.scroll("Date set.");
    } else {
        uBit.display.scroll("Oops something went wrong!");
    }

    uBit.display.scroll("12 or 24 hour clock?");
    setMode = false; // default 24 hour
    setAM_PM = false; // default
    while ( !(uBit.buttonA.isPressed()) or !(uBit.buttonB.isPressed())) {
        uBit.display.scroll("Press A for 24 or B for 12");
    }
    if (uBit.buttonA.isPressed()) {
        setMode = true; //12 hr clock
        // am or pm?
        while ( !(uBit.buttonA.isPressed()) or !(uBit.buttonB.isPressed())) {
            uBit.display.scroll("OK 12hr - AM or PM?");
            uBit.display.scroll("A for AM or B for PM");
        }
        if (uBit.buttonA.isPressed()) setAM_PM = true;
        if (uBit.buttonB.isPressed()) setAM_PM = false;
    }
    if (uBit.buttonB.isPressed()) setMode = false;
    if ((setMode==true) and (ihh>12)) {
        setHours = ihh-12;
    } else {
        setHours = ihh;
    }
    uBit.display.scroll("Hours:" + setHours);
    while ( !(uBit.buttonAB).isPressed()) {
        if ( uBit.buttonA.isPressed() ) {
            setHours--;
        }
        if (uBit.buttonB.isPressed()) {
            setHours++;
        }
        if (setHours < 1) setHours=24;
        if (setHours > 24) setHours=1;
        if ((setMode==true) and (ihh>12)) {
            setHours = ihh-12;
        }

    }
    uBit.display.scroll(setHours);

    setMinutes = imm;
    uBit.display.scroll("Minutes:" + setMinutes);
    while ( !(uBit.buttonAB).isPressed()) {
        if ( uBit.buttonA.isPressed() ) {
            setMinutes--;
        }
        if (uBit.buttonB.isPressed()) {
            setMinutes++;
        }
        if (setMinutes < 0) setMinutes=59;
        if (setMinutes > 59) setMinutes=0;
        uBit.display.scroll(setMinutes);
    }
    setSeconds = iss;
    uBit.display.scroll("Seconds :" + setSeconds);
    while ( !(uBit.buttonAB).isPressed()) {
        if ( uBit.buttonA.isPressed() ) {
            setSeconds--;
        }
        if (uBit.buttonB.isPressed()) {
            setSeconds++;
        }
        if (setSeconds < 0) setSeconds=59;
        if (setSeconds > 59) setSeconds=0;
        uBit.display.scroll(setSeconds);
        while ( !(uBit.buttonAB).isPressed()) {
            uBit.display.scroll("Press AB to set the time");
        }
        if (uBit.buttonAB.isPressed()) {
            mTime.hours = setHours;
            mTime.minutes = setMinutes;
            mTime.seconds = setSeconds;
            mTime.mode = setMode;
            mTime.am_pm = setAM_PM;
            rVal = mRTC.set_time(mTime);
            if (rVal == 0) {
                uBit.display.scroll("Time is set.");
            } else {
                uBit.display.scroll("Oops it all gone pete tong");
            }
        }
    }
}

void displayTimeAndDate()
{

    if (displayMode == 0) {
        displayDate();
        displayTime();
        displayTemp();
    }
    if (displayMode == 1) {
        displayDate();
    }
    if (displayMode == 2) {
        displayTime();
    }
    if (displayMode == 3) {
        displayTemp();
    }
    if (displayMode == 4) {
        displaySeconds();
    }
}
void displayDate()
{
    ManagedString spc(" ");

    ManagedString dateOut(txtDay + spc + DD + spc +  txtMonth + spc + YY);

    uBit.display.scroll(dateOut);


    uBit.sleep(1000);
}
void displayTime()
{
    ManagedString timeOut(hh + tsep + mm);

    uBit.display.scroll(timeOut);

    uBit.sleep(1000);
}
void displayTemp()
{
    uBit.display.scroll(tempOut);


    uBit.sleep(1000);
}
void displaySeconds()
{
    uBit.display.scroll(iss);
}


int main(void)
{
    // Initialise the micro:bit runtime.
    uBit.init();

    while (true) {

        getTimeAndDate();
        displayTimeAndDate();
        if (uBit.buttonB.isPressed()) {
            displayMode ++;
            if(displayMode > 4 ) {
                displayMode=0;
            }
        }

        if (uBit.buttonAB.isPressed()) {
            setTimeAndDate();
        }



    }
// forever, in a power efficient sleep.
    release_fiber();

}