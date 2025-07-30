# **Arduino UNO OpenTherm Shield**

- [**Arduino UNO OpenTherm Shield**](#arduino-uno-opentherm-shield)
  - [**Overview**](#overview)
    - [**Features**](#features)
  - [**Schematic diagram**](#schematic-diagram)
  - [**Module visualisation**](#module-visualisation)
  - [**Assembly**](#assembly)
  - [**Production files**](#production-files)
  - [**Testing out the hardware**](#testing-out-the-hardware)
    - [Prerequisites](#prerequisites)
    - [Instructions](#instructions)
    - [Notes](#notes)
  - [**Software**](#software)
  - [**Reporting bugs**](#reporting-bugs)
  - [**License**](#license)
    - [**Hardware**](#hardware)
    - [**Software**](#software-1)
  - [**Support**](#support)

---

## **Overview**
The Arduino OpenTherm Shield is an open-source hardware interface designed to enable an Arduino microcontroller to communicate with heating systems using the OpenTherm protocol.
OpenTherm is a point-to-point communication protocol commonly used in modern heating systems for efficient control and monitoring.
This shield provides the necessary voltage conversion, electrical isolation, and signal conditioning to interface the Arduinos 5V or 3.3V logic
with the OpenTherm bus, which typically operates at higher voltages (24V).

  ### **Features**
  * OpenTherm slave interface to communicate with a boiler
  * OpenTherm master interface to communicate with a thermostat
  * can implement master, slave and gateway modes
  * built-in (3.3-5V) to 24V DC-DC step up converter
  * Arduino UNO compatible shield

This work is based on [otgw.tclcode.com](https://otgw.tclcode.com) project.

---

## **Schematic diagram**
<p align="center"><img src="./docs/schematic.png" alt="wiring diagram" width="80%"/></p>


## **Module visualisation**
(click on the image to see the 3D model)
<p align="center"><a href="https://3dviewer.net/#model=https://github.com/michpro/Arduino_OpenTherm_shield/blob/master/docs/Arduino_OpenTherm_shield.wrl"><img src="./docs/Arduino_OpenTherm_shield.png" alt="Arduino UNO OpenTherm Shield" width="50%"/></a></p>

## **Assembly**
[Interactive BOM and placement](https://michpro.github.io/Arduino_OpenTherm_shield/ibom.html)

For Arduino UNO:
* JP1 jumper – leave in the Auto position (V<sub>cc</sub> from IOREF).

For WeMos D1 UNO:
* JP1 jumper - this board is not fully compatible with Arduino specifications - the IOREF pin on this board should have a voltage of 3.3V, but it has 5V. Therefore, cut the default jumper between the Auto and V<sub>cc</sub> pads and use a drop of tin to connect the V<sub>cc</sub> pad to 3.3V.<br>*You can also modify the WeMos D1 UNO so that the IOREF pin has the correct voltage of 3.3V*.

Other boards:
* This shield should also be compatible with other boards. You only need to check the correct voltage on the IOREF pin (and possibly make a change as for WeMos D1 UNO) and, if necessary, adjust the software to the board definition.

## **Production files**
Production files can be found [**here**](./production/).

---

## **Testing out the hardware**
Below are the start-up instructions for the ["shield_test.ino"](./software/shield_test/shield_test.ino) Arduino program, a diagnostic tool designed to test and validate the electrical and communication integrity between a boiler and thermostat circuits on the OpenTherm shield. These instructions guide you through the process step-by-step, ensuring you can verify your hardware setup correctly.

### Prerequisites
- **Arduino Board**: Use an AVR family board (e.g., Arduino UNO).
- **OpenTherm Shield**: Ensure it is properly connected to the Arduino.
- **Computer**: Equipped with Arduino IDE or a serial monitor software.
- **Multimeter**: Required for measuring voltages and currents.
- **Two Wires**: Needed to connect the BOILER and THERM terminals during testing.

### Instructions

1. **Upload the Program**
   - Load the ["shield_test.ino"](./software/shield_test/shield_test.ino) program onto your Arduino board (Arduino UNO preffered) using the Arduino IDE.<br>
    *If necessary, adjust the pins used in the programme.*

2. **Open Serial Monitor**
   - Launch the serial monitor in the Arduino IDE or your preferred serial terminal software.
   - Set the baud rate to **115200**.

3. **Initial Setup**
   - Disconnect the 'THERM' and 'BOILER' terminals from each other if they are connected.
   - Check the LEDs on the shield:
     - **'PWR'** and **'RxT'** LEDs should be lit.
     - **'RxB'** LED should be off.
   - The serial monitor will display a prompt. If the LED statuses match, type **`y`** and press Enter to proceed.

4. **Voltage Measurement 1**
   - Use a multimeter to measure the voltage between the **+24V test point** and **GND**.
   - Expected range: **23.5V to 24.5V**.
   - If within range, type **`y`** in the serial monitor to proceed.

5. **Voltage Measurement 2**
   - Measure the voltage between the **T+ test point** and **GND**.
   - Expected value: Approximately **24V**.
   - If correct, type **`y`** to proceed.

6. **Current Measurement 1**
   - Measure the current flowing between **T+** and **GND**.
   - Expected range: **5mA to 9mA**.
   - Verify that the **RxT LED** is **not lit** during this measurement.
   - If correct, type **`y`** to proceed.

7. **Activate MASTER-OUT and Current Measurement 2**
   - The program will activate the 'MASTER-OUT' line (you’ll see a message in the serial monitor).
   - Measure the current between **T+** and **GND** again.
   - Expected range: **17mA to 23mA**.
   - Ensure the **RxT LED** remains **not lit**.
   - If correct, type **`y`** to proceed.

8. **Connect Terminals and Voltage Measurement 3**
   - Connect the **BOILER** and **THERM** terminals using two wires (polarity does not matter).
   - Measure the voltage between test points **B1** and **B2**.
   - Expected range: **15V to 18V**.
   - If within range, type **`y`** to proceed.

9. **Check RxB LED**
   - The program will activate 'MASTER-OUT' again.
   - Observe the **'RxB' LED**; it should **light up**.
   - If it does, type **`y`** to proceed.

10. **Activate SLAVE-OUT and Voltage Measurement 4**
    - The program will activate the 'SLAVE-OUT' line.
    - Check that the **RxT LED** **stops lighting up**.
    - Measure the voltage between **B1** and **B2** again.
    - Expected range: **5V to 7V**.
    - If correct, type **`y`** to proceed.

11. **Test Result**
    - If all steps are completed successfully, the serial monitor will display:
      ```
      *****************
      *  TEST PASSED  *
      *****************
      ```
    - If any step fails, it will show:
      ```
      *****************
      * TEST FAIL !!! *
      *****************
      ```

12. **Continuous Testing (if passed)**
    - If the test passes, the program will enter a continuous testing mode.
    - It will repeatedly check communication between the boiler and thermostat, printing:
      - **"Boiler inbound, thermostat outbound ... OK"** or **"Failed"** with a reason.
      - **"Boiler outbound, thermostat inbound ... OK"** or **"Failed"** with a reason.

### Notes
- **Test Points and LEDs**: Ensure you correctly identify the test points (+24V, T+, GND, B1, B2) and LEDs (PWR, RxT, RxB) on the OpenTherm shield.
- **Safety**: Exercise caution when measuring voltages and currents to avoid short circuits.
- **Troubleshooting**: If a measurement falls outside the expected range:
  - Double-check your connections and multimeter settings.
  - Retry the step. Persistent issues may indicate a problem with the shield or setup.

<br>

***Follow these instructions carefully to test and validate your OpenTherm shield setup. Each step requires confirmation via the serial monitor, so keep it open and active throughout the process. Successful completion ensures your boiler and thermostat circuits are functioning correctly.***

---

## **Software**
- Recommended library for use in your OpenTherm projects: 
  [https://github.com/ihormelnyk/opentherm_library](https://github.com/ihormelnyk/opentherm_library)

---

## **Reporting bugs**

[Create an issue on GitHub](https://github.com/michpro/Arduino_OpenTherm_shield/issues)

---

## **License**
Copyright © 2025 Michal Protasowicki

### **Hardware**
  * Hardware part of this project is released under CERN Open Hardware Licence Version 2 - Permissive.

    [![License: CERN-OHL-P-2.0](https://img.shields.io/badge/License-CERN--OHL--P--2.0-blue.svg)](./LICENSE)

### **Software**
  * Software part of this project is released under MIT Licence.

    [![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](./software/shield_test/LICENSE)

---

## **Support**
If You find my projects interesting and You wanted to support my work, You can give me a cup of coffee or a keg of beer :)

[![PayPal Direct](https://badgen.net/badge/icon/Support%20me%20by%20PayPal?icon=kofi&label&scale=1.5&color=blue)](https://www.paypal.me/michpro)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[![ko-fi](https://badgen.net/badge/icon/Support%20me%20on%20Ko-fi?icon=kofi&label&scale=1.5&color=red)](https://ko-fi.com/F1F24CEW1)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[![Coinbase](https://badgen.net/badge/icon/Support%20me%20with%20cryptocurrencies?icon=kofi&label&scale=1.5&color=blue)](https://commerce.coinbase.com/checkout/ec299320-cbed-475d-976e-fdf37c1ac3d0)
