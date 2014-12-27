G19s Imperial Clock Applet - by Nic Holthaus
--------------------------------------------
The MIT License (MIT)

Copyright (c) 2014 Nic Holthaus

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--------------------------------------------
1. INSTALLING
--------------------------------------------
1. Double Click the included installer "G19s-Imperial-Clock-Installer.exe"
2. Follow the prompts
3. To make the clock available on computer start-up, add the "LCDImperialClock.exe" program to
your windows Startup folder in the start menu. The .exe can be found at "C:\Program Files\Logitech Gaming Software\Applets"
or wherever your logitech gaming software directory is located.
4. Now simply select the program on the applet chooser of your G19 keyboard!

--------------------------------------------
2. CHANGING SETTINGS
--------------------------------------------
There are several configurable settings which can be accessed by clicking the system tray icon
on the bottom right corner of your windows task bar while the program is running.

1. Smooth Movement - turning this on makes the clock's second hand swing smoothly. Disabling it gives the
clock a 'ticking' effect. Smooth motion uses slightly more CPU than ticking.
2. Show Date - Shows the current date atop the clock. This can also be enabled by pressing 'up' on the
G19s D-pad.
3. Show digital time - Shows the current digital time below the clock. This can also be enabled by 
pressing 'down' on the G19s D-pad.
4. 24-hour time - Changes digital time to a 24 hour clock (or vice versa) when enabled.
5. Personalized Message - Shows the message of your choice near the bottom of the clock. Maximum of
29 characters (which is the width of the G19's lcd)

To exit the program, right click the system tray icon and select 'exit'.

--------------------------------------------
3. BUILDING
--------------------------------------------
To build the software, you'll need Qt 5.4 or later (used for image processing/GUI),
CMAKE to build the makefile/visual studio project, and NSIS if you want to build the windows
installer package. As of writing this, the project is only really set up for windows, as I'm pretty
sure the logitech libraries only support windows. If that's not the case, the clock code itself should
be a trivial port since cmake/qt are natively cross-platform.
