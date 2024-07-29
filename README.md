# simon

This is an implementation of the [Simon Game](https://en.wikipedia.org/wiki/Simon_(game)) on an AVR microcontroller.

## PCP

The PCB design in KiCad was made by [Jan Hermes](mailto:jan.hermes@rub.de) (MicroCRT) from the [electronic workshop (EWS)](https://etit.ruhr-uni-bochum.de/fakultaet/zentrale-einrichtungen/electronic-workshop/) at the [Ruhr-University of Bochum](www.ruhr-uni-bochum.de). Thanks!

### main components

The SMD components are all given in the KiCad files, but the main parts are listed below.

It should be noted, that one is free on the choice of LEDs, but in order to match the perceived brightness, they have to be matched with appropriate resistors.

- [ATmega168PB-MNR](https://www.mouser.de/ProductDetail/579-ATMEGA168PBMNR)
- [battery holder](https://www.mouser.de/ProductDetail/712-BAT-HLD-001-THM)
- [5 switches](https://www.mouser.de/ProductDetail/179-TS03665260LCRD67)
- [main switch](https://www.mouser.de/ProductDetail/179-SLW-108645-4ARAD)
- [2x10 pin header](https://www.mouser.de/ProductDetail/200-TSW10507TD)
- [10 kOhm pull up resistor for RESET](https://www.mouser.de/ProductDetail/71-RCA040210K0JNED)
