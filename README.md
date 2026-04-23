# Kingst_6502_Analyser
WDC65C134 and WDC65C256 Analyser for the Kingst LA2016  

Library and includes downloaded from the Kingst website (https://www.qdkingst.com/en)

Currently allows the decoding of WDC65C134 and WDC65C256 opcodes. This uses the following signals:

1) PHI2   -  Main 6502 clock signal.
2) R/W    -  Read/Write line.
3) RUN    -  This is the equivelent of the SYNC signal on these microcontrollers.
4) D0-D7  -  8 bit data bus.


# Test system
This has been tested using:
1)  Ubuntu 24.04.4 LTS.
2)  GCC 13.3.0
3)  Kingst LA2016
4)  WDC65C134SXB and WDC65C256SXB

# Limitations
Does not take in to account the setting of the X and M bits, it assumes that the access is always 8-bit.
