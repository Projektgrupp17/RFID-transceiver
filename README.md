# RFID-transceiver
Tool for reading RFID tags and (soon to be) transmitting them to a remote server with the help of ST Microelectronics CH95HF transceiver board.

## Supports
- Read real active or passive RFID tags through ISO 15693 protocol.
  - Currently only 1 protocol at a time with manually triggered scans.
- Simulate the aforementioned process of reading of RFID tags.

## Installation
To install and run do (root privileges required for usb interface):

    make
    sudo ./rfid

To remove:

    make clean
    
## Todo
- Add http support.
- Implement more sophisticated solutions for rescan and protocol rotation.
- Implement solution recognizing RFID tags leaving and entering the environment.
- Expand current configuration possibilities.
- More tests and comments.
