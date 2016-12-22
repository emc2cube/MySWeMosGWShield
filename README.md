MySWeMosGWShield
==========

![NRF24L01 GW on the WeMos](https://raw.githubusercontent.com/emc2cube/MySWeMosGWShield/master/img/MySWeMosGWShield_nrf.jpg)
![All adapters](https://raw.githubusercontent.com/emc2cube/MySWeMosGWShield/master/img/all_adapters.jpg)
![PCB back view](https://raw.githubusercontent.com/emc2cube/MySWeMosGWShield/master/img/back.jpg)
![KiCad view](https://raw.githubusercontent.com/emc2cube/MySWeMosGWShield/master/img/kicad-pcb.png)


Description
-----------

Simple [MySensors](http://www.mysensors.org) [WeMos Mini](https://www.wemos.cc/product/d1-mini-pro.html) gateway shield with support for 3 radio modules: SMD NRF24L01+, SMD PA+LNA NRF24L01+ or SMD RFM69HW.


Ordering
--------

Gerber files are included so you can order these PCB at your favorite PCB fab house and solder the few components yourself.
For an easy ordering process you can directly order these PCB without having to do anything else:
- [PCBs.io](https://PCBs.io/share/4QAjD) 4 for $5.52, black 1.2mm PCB, ENIG finish.
- [OSHPark.com](https://oshpark.com/shared_projects/59hasWWz) 3 for $6.90, purple 1.6mm PCB, ENIG finish.


Assembly
--------

Simply solder your favorite radio module to the corresponding trace and a 4.7 to 47µF SMD 0805 capacitor on the top side of the PCB.
It is also recommended to solder the 0.1µF and 10µF SMD 0805 capacitors but not required unless you add some of the optional components.

Radio module layout:
- NRF24L01 PA+LNA
![NRF24L01 PA+LNA shield](https://raw.githubusercontent.com/emc2cube/MySWeMosGWShield/master/img/nrf24palna.jpg)
- NRF24L01
![NRF24L01 shield](https://raw.githubusercontent.com/emc2cube/MySWeMosGWShield/master/img/nrf24.jpg)
- RFM69HW
![RFM69 shield](https://raw.githubusercontent.com/emc2cube/MySWeMosGWShield/master/img/rfm69.jpg)


Optional components
-------------------

### 3.3V linear voltage regulator
You *should not* need to add this, but if you are using an amplified radio module (such as a PA+LNA NRF24L01+) you may experience Tx errors.
If the WeMos power regulator do not provide enough current for your radio you can add an additional AMS117 regulator between the 5V and 3.3V circuits and a 10µF SMD 0805 capacitor between 3.3V and GND.

### ATSHA204 module (unsupported?)
For security reason you can add a CryptAuthEE SHA256 chip. This will allow you to sign messages and will secure communications between the gateway and other nodes (this is not encryption, just signing).
If you decide to add the ATSHA204 chip you will need to add a 0.1µF SMD 0805 decoupling capacitor and 4.7kΩ SMD 0805 pull-up resistor.
Signing can also be done at the software level, without the chip if you decide to add this function later.
Currently ESP8266 MySensors gateway do not seem to support hardware encryption but it may work in a later release or if used as a node or repeater.
For complete reference see the detailled [MySensors Security & Signing guide](https://www.mysensors.org/about/signing)

### Inclusion mode button
If you have a compatible controller you can add an [inclusion mode button](https://www.mysensors.org/build/advanced_gateway#inclusion-mode-button).


Installation
-------

Plug the shield on top of your [WeMos Mini](https://www.wemos.cc/product/d1-mini-pro.html).
Do not forget to edit the various options of the [Arduino example code](https://github.com/emc2cube/MySWeMosGWShield/tree/master/Arduino/) before uploading it.


Arduino
-------

Included programs

### MySWeMosGWShield_SecurityPersonalizer
Adapted from [MySensors Security & Signing](https://www.mysensors.org/about/signing) SecurityPersonalizer sketch with pin assigned for this shield.

### MySWemosGatewayESP8266OTA
Gateway sketch exemple (NRF24L01, ESP8266 OTA and inclusion mode enabled)


Notes
-----

Shields are stackable and additional ones can be added on top / below the WeMos to introduce new functions / sensors to your gateway.
As a note, keep in mind that an ESP8266 tends to be warm while in use, a temperature probe won't be accurate if too close. 
For best radio performances it is recommended to have the radio shield on the topmost position.

Only a few pins are left to be used, depending on your configuration and the additional components added on the board:
- A0 (Analog input, max 3.3V input) is available if you don't use software signing.
- D0 (GPIO16, IO) is available if you don't add/use an ATSHA chip for hardware signing.
- D1 (GPIO5, IO, SCL) is always available.
- D2 (GPIO4, IO, SDA) is available if you use a RFM69HW radio module. 
- D3 (GPIO0, IO, 10k Pull-up) is available if you don't use an inclusion mode button.
- D4 (GPIO2, IO, 10k Pull-up, BUILTIN_LED) is available if you don't use flash leds on rx/tx/err.
- TX (GPIO1, TXD) is always available but used for serial communication.
- RX (GPIO3, RXD) is always available but used for serial communication.


Revision history
----------------

Version 1.0: Initial release.


Special Kicad libraries
---------------

This project use the [Kicad footrpints and templates for wemos D1 mini board](https://github.com/jerome-labidurie/d1_mini_kicad) library.
