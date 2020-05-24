# RFID-transceiver
Tool for reading RFID tags and (soon to be) transmitting them to a remote server with the help of ST Microelectronics CR95HF transceiver board. Ideal for usage in various IOT implementations.

## Features
- Read real active or passive RFID tags through ISO 15693 protocol.
  - Currently only 1 protocol at a time; no protocol rotation.
- Simulate the aforementioned process of reading of RFID tags.
- HTTP transmission of identified tag to remote server.

## Dependencies
External dependencies involve:
- libusb.
- CR95HF library.
- POCO libraries.

**Do note:** the latest libusb API changes breaks the functionality, 1.0.22 or earlier required (only tested with 1.0.22).

## Installation
Path assumptions are made that the CR95HF shared library is in the library path, and POCO shared libraries are assumed to be installed in /usr/local/lib/poco.

To install and run do (root privileges required for usb interface):

    make
    sudo ./rfid

If alternative shared library directories are used, to override one or the other, or both, default path, do:

    make CLIBS=/path/to/libCR95HF.so POCOH=/path/to/poco
    sudo ./rfid

To remove:

    make clean
    
## Troubleshooting
If errors regarding functionality implemented in the external libraries; CR95HF and POCO, occur during runtime. Remember to rebuild cache:

    sudo ldconfig -v

And verify that symlinks to said libraries are in order:

    ldconfig -p

If libraries are placed in a custom folder that isn't already in the search path, it may be required to add the path manually:

    export LD_LIBRARY_PATH=/path/to/lib:$LD_LIBRARY_PATH

or, for a more persistent solution:

    cd /etc/ld.so.conf.d
    echo "/path/to/lib" >> libname.conf
    sudo ldconfig -v

## Todo
- Implement more sophisticated solutions for rescan and protocol rotation.
- Expand current configuration possibilities.
- More documentation, tests and comments.
