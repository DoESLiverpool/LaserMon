laserproject
============
I've built a temperature and current monitoring system for the two laser cutters. The project is based on an Arduino board, two current sensors, one temperature sensor and an LCD display. Through an electronic circuit which the sensors are connected to, the Arduino board calculates the values of current and temperature (which is shown on the LCD) and eventually the data is sent to Xively. The purposes of this project are basically two:

1 - Keeping the water cooling temperature within certain values, in particular under 35C, otherwise the laser cutters reliability might be seriously compromised. The highest efficiency is obtained with a temperature of 15C. The display shows the temperature and changes backlit color due to temperature change. Specifically when the temp is lower than 25C it remains blue, when it's between 25C and 35C it gradually fades from blue to red and when it's higher than 35C the red backlit flashes showing "Alarm!" on the LCD (just press the unique button on the circuit to silence the Alarm message).

2 - Remotely checking (via Xively website) the current values of both laser cutters , measured in Ampere. There are not particular indication about it, except for some current peaks measured by the Arduino input when the board is powered on/off. They are nothing to worry about and they don't affect the board safety at all. Lasers current values usually goes from 0.10A when idle up to 6A when the big laser is running at maximum power.

In order to work properly, the current sensor needs to be connected to the line wire 

The board and the Arduino must be powered on with the dedicated USB cable and connected to the network via ethernet

