# ⚠️ DANGER: High Voltage Warning ⚠️

This project involves working with mains voltage (e.g., 120V/240V AC), which is **extremely dangerous** and can cause severe electrical shock, fire, injury, or even death.

  - **Qualified Individuals Only**: You should only attempt to build or use this project if you are a qualified individual with a thorough understanding of electrical safety and high-voltage circuits.
  - **Your Safety is Your Responsibility**: The author assumes no liability for any damage, injury, or loss of life resulting from the use or misuse of this project, its information, or its code. You are solely responsible for your own safety.
  - **When in Doubt, Don't Proceed**: If you are a beginner or are unsure about any aspect of this project, **DO NOT PROCEED**. Always prioritize safety above all else.

# 💡 ArduinoUPS Project

ArduinoUPS is a cost-effective and efficient DC-DC Uninterruptible Power Supply (UPS) solution. It uses off-the-shelf components to create a reliable backup system for your critical devices.

  - ✅ **Ideal for small systems** under 280 watts.
  - ⚡ **Simple and efficient** DC-to-DC power conversion.
  - 🔋 **High-longevity LiFePO4 batteries** for reliable, long-term use.
  - 💸 **Affordable** with minimal components to assemble.
  - 📉 **USB HID compatible** monitor your UPS through your OS or [NUT](https://github.com/networkupstools/nut).


#### Part 1: DC Power System

The core of this project is a DC Uninterruptible Power Supply (UPS) built with a dedicated UPS/PSU module and a LiFePO4 battery. While this hardware can function on its own, it lacks monitoring capabilities.

#### Part 2: Monitoring and Reporting

An Arduino microcontroller monitors the UPS's operational status and reports the data back to a connected computer using the [HIDPowerDevice](https://github.com/abratchik/HIDPowerDevice) library. This monitoring system is designed to be compatible with the Network UPS Tools ([NUT](https://github.com/networkupstools/nut)) framework.

-----

## 🛒 Bill of Materials (BOM)

You will need the following parts to build this project:

### Core Components

  * 1x **Microcontroller**: [ATmega32U4-based board](https://s.click.aliexpress.com/e/_oCgz2n9) (e.g., Arduino Pro Micro or Arduino Leonardo).
  * 1x **DC Voltage Sensor**: [DC Voltage Sensor Module](https://s.click.aliexpress.com/e/_oDO9SRN).
  * 1x **AC Current Sensor**: [ZMCT103C Module](https://s.click.aliexpress.com/e/_oo3mSPH).
  * 1x **PSU/UPS Module**: [250W](https://s.click.aliexpress.com/e/_oEvcbEr) or [350W](https://s.click.aliexpress.com/e/_okWmb1V) model. *Remember to choose a voltage (12V or 24V) that matches your system.*
  * 1x **Battery**: [LiFePO4 Battery](https://amzn.to/4oprkak) with a built-in BMS rated for 15A or higher.

### Voltage Converters (Choose based on your needs)

To power your equipment, you may need to stabilize the output voltage.

#### For a \~12V Computer System (PicoPSU)

  * [400W PicoPSU (DC 5-34V Input)](https://s.click.aliexpress.com/e/_omeJSUf) - *Recommended for wider voltage compatibility.*
  * [250W PicoPSU (DC 12-25V Input)](https://s.click.aliexpress.com/e/_oDlRFaP) - *Note: This unit's limited input range may not utilize your battery's full capacity.*

#### For a \~24V Computer System

  * [300W Converter (DC 16-28V Input)](https://s.click.aliexpress.com/e/_op0HwJV)
  * [500W Converter (DC 16-60V Input)](https://s.click.aliexpress.com/e/_ooraGpZ)

#### For Modems, Switches & Other Low-Voltage Gear

  * [3A Buck-Boost (1.25-26V Output)](https://s.click.aliexpress.com/e/_opfwM1Z)
  * [3A Buck-Boost (0.5-30V Output)](https://s.click.aliexpress.com/e/_ooifDCx)
  * [Fixed-Voltage Buck-Boost (1-5A, 12-28V Input)](https://s.click.aliexpress.com/e/_oFBA1GF) - *I really like these\!*

-----

## 📝 Important Notes & Best Practices

Follow these tips for a safe and successful build:

  * **Measure Your Max Load**: Before buying parts, use a [Kill-A-Watt meter](https://amzn.to/4ftgFrc) (or similar device) to measure your system's peak power consumption. Ensure it's well within the capacity of your chosen battery and PSU/UPS module.
  * **Don't Skimp on the BMS**: Your battery's BMS determines the maximum power output. A 15A BMS can deliver \~192W (12.8V \* 15A), while a 20A BMS can deliver \~256W. When in doubt, oversize it\!
  * **Adjust the PSU/UPS Voltage**: You may need to tune the adjustable potentiometer on the PSU/UPS module. A setting of \~14.4V is often a good target for LiFePO4 charging.
  * **Use a Wide-Voltage PicoPSU**: Since the PSU/UPS output can be as high as 14.4V, choose a PicoPSU with a wide input voltage range to ensure stability and safety.
  * **Stay Under 80% Load**: For longevity, do not continuously exceed 80% of your PSU/UPS module's maximum rated load. For a 250W unit, stay under 200W. For a 350W unit, stay under 280W.
  * **Select the Correct Wire Gauge**: Use battery cables thick enough for your system's electrical load and cable length. Undersized wires can be a fire hazard.

-----

## 🔌 Wiring

*(This section is a work in progress\!)*

-----

## 🛠️ Software & Setup

### Dependencies

You will need [PlatformIO](https://platformio.org) to build and flash the firmware. You can install the [PlatformIO IDE extension for VSCode](https://platformio.org/install/ide?install=vscode) or use PlatformIO Core (CLI).

> **Alternative**: You can use the Arduino IDE, but you will need to:
>
> 1.  Rename `src/main.cpp` to `src/ArduinoUPS.ino`.
> 2.  Manually install the libraries listed in the `platformio.ini` file.

### Project Configuration

You **must edit `config.h`** to configure the project for your specific hardware and needs.

*(This section is a work in progress\!)*

-----

## 🚀 Building and Flashing

1.  Connect your Arduino board to your computer.
2.  Open a terminal in the project directory.
3.  Run the following PlatformIO command:
    ```bash
    pio run --target upload
    ```

-----

## 🖥️ Computer Configuration

1.  **Configure NUT:**
    * Use the provided `ups.conf` file as an example.
    * You **must** edit this file and replace the default Product ID with the one for your device.

2.  **Configure Linux udev Rules:**
    > **Note:** This step is only required for Linux-based systems to ensure correct device permissions.

    * Edit the `98-upower-hid.rules` file and set the Product ID to match your device.
    * Copy the modified file into the `/etc/udev/rules.d/` directory.

    ```bash
    sudo cp 98-upower-hid.rules /etc/udev/rules.d/
    ```

