Indoor climate measuring nodes running on a single coin cell battery. Measurements done every 5 minutes (not necessarily sent at same interval).
\n\n
Specs. for now:
*NRF24L01 - 2.4GHz module
*SAMD21G18 - MCU
*CR2450 - Coin cell battery ~ 500+mAh
*BME280 - Temperature, Humidity & Pressure.
*DS1302 - Ultra low power RTC for waking up node / Perhaps instead TLP5010.
\n\n
Ideas to improve battery life:
Latch/flippydifloppy to control power to MCU + SIP32431.

