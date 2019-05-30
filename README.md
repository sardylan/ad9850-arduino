# Arduino sketch for DDS AD9850

> Simple arduino sketch for driving DDS AD9850

[Analog Devices AD9850](https://www.analog.com/en/products/ad9850.html)

This little sketch is useful for driving the cheap Analog Device DDS AD9850.

This chip, equipped with a 125 MHz crystal oscillator, is capable to produce a sine wave up to 40 MHz.

All the experiments are made using an HC-SR08 PCB, which came with the DDS, the crystal and pin risers already mounted.

## Wiring

Connect the following pins between Arduino and the PCB:

Arduino | HC-SR08 PCB with AD9850
--------|------------------------
 **8**  | *RESET*
 **9**  | *DATA*
 **10** | *FU_UD*
 **11** | *W_CLK*

For serial programming, AD9850 requires pin **D0** to be connected to *VCC*, and pins **D1** and **D2** to ground.

## Usage

Open a serial terminal in Arduino serial port, at 115200 baud, configured as 8N1.

There are 2 commands:

- **F**: change frequency
- **R**: DDS reset

### Examples:

To set 10.0 MHz, input `F10000000` and press RETURN, you will read:

```
Frequency (Hz): 10000000 - Number: 343597383 - Sending to DDS: 11100010100001110101111000101000 - Completed!
```

To set 28.215 MHz, input `F28215000` and press RETURN, you will read:

```
Frequency (Hz): 28215000 - Number: 969460018 - Sending to DDS: 01001100100100110001001110011100 - Completed!
```

- First value is the frequency request.
- The second value is the unsigned 32-bit integer which will be sent to the DDS.
- The third value is the same 32-bit integer in binary form, written in big endian format.

To reset DDS, input `R` and press RETURN, you will read:

```
DDS reset...  - Completed!
```

## License

Arduino sketch for DDS AD9850
Copyright (C) 2019  Luca Cireddu (sardylan@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
