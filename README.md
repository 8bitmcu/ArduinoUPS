⚠️ DANGER: High Voltage Warning
===============================

This project involves working with mains voltage (e.g., 120V/240V AC), which is extremely dangerous and can cause severe electrical shock, fire, injury, or death.

You should only attempt to build or use this project if you are a qualified individual with a thorough understanding of electrical safety and high-voltage circuits.

The author assumes no liability for any damage, injury, or loss of life resulting from the use or misuse of this project, its information, or its code. You are solely responsible for your own safety.

If you are a beginner or are unsure about any aspect of this project, DO NOT PROCEED. Always prioritize safety above all else.

ArduinoUPS
==========

**WIP**

ArduinoUPS is an implementation of the [HIDPowerDevice](https://github.com/abratchik/HIDPowerDevice) using off the shelf components to create a low power, efficient and affordable DC UPS computer backup solution.

- Ideal for systems under 280 watts
- Simple and efficient DC to DC conversion


BOM
---

- 1x [ATmega32U4](https://s.click.aliexpress.com/e/_oCgz2n9) microcontroller. (Arduino Pro Micro or Arduino Leonardo)
- 1x [DC Voltage Sensor](https://s.click.aliexpress.com/e/_oDO9SRN) module
- 1x [ZMCT103C](https://s.click.aliexpress.com/e/_oo3mSPH) AC current sensor
- 1x [PicoPSU](https://s.click.aliexpress.com/e/_oDlRFaP) (12~25v wide-voltage range)
- 1x [250w](https://s.click.aliexpress.com/e/_oEvcbEr) or [350w](https://s.click.aliexpress.com/e/_okWmb1V) PSU/UPS module
- 1x [LiFePO4 battery](https://amzn.to/4oprkak) (with 15A BMS or higher)

notes:
- **Measure your system maximum load**: Using a [P3 kill-a-watt](https://amzn.to/4ftgFrc) (or similar device) to confirm your system's maximum load is within the battery and UPS/PSU module's capacity.
- **Choose a battery with a high Amp BMS**: For example, a 15A BMS can deliver at most ~192w of power while a 20A BMS will do ~256w.
- **Adjust the voltage of the UPS/PSU module to ~14.4v**: with my UPS/PSU unit, it seems that both the charging and output voltages are driven by the adjustable potentiometer on the board.
- **Use a wide-voltage range PicoPSU unit**: With a output as high as 14.4v from the UPS/PSU unit, using a picoPSU that can accept voltage from 12-25v is much safer choice.
- **Do not exceed 80% of the UPS/PSU maximum load**: for a 250w unit, that would be 200w and for a 350w unit that would be 280w.

Wiring
------
![Wiring Diagram](assets/wiring.png)

Dependencies
------------
- [PlatformIO](https://platformio.org): You can set up either PlatformIO Core or the IDE. This is needed to build and flash the project.

Building and Installing
-----------------------
1. Compile and flash the project to the arduino: `pio run --target upload`

License
-------

Copyright (c) Alex Bratchik 2020. All right reserved.

Copyright (c) 8bitmcu 2025. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
