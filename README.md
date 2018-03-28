# PeltierControl
Author: Janine Menne

## Description
C++ class with a small controller to control with a Peltier element, which enables the cooling of an SiPM.
The Peltier element is powered with a Keithley SourceMeter and read out by a Keithley MultiMeter.

The measurement devices are connected to the PC via gpib.

## Requirements to run terminal program

Availability of the following libraries (author: Janine Menne):  
* [SourceMeter][1]  
* [MultiMeter][2]  
* [LogFileDev][4]  
* [gpib][5] 





[1]: https://github.com/jamenne/Keithley_SourceMeter_Control
[2]: https://github.com/jamenne/Keithley_MultiMeter_Control
[3]: https://github.com/jamenne/PeltierElementControl
[4]: https://github.com/jamenne/Device_Logfiles  
[5]: https://github.com/jamenne/gpib  