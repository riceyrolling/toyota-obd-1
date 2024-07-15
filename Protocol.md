### The Australian Version of the Protocol in PDF

In general, it should be clear how the protocol works. A "1" is +5V at the VF1 output, and a "0" is 0V at the same output (with TE2-E1 shorted).

Data is transmitted frame by frame. Each frame consists of:
- 16 bits of "1" indicating the start of the frame;
- The first "byte" of the protocol consists of 12 bits (the Australian sees this as `clock_test` (4 bits) + `word[00]` (another 8 bits) - I disagree with him because he just happened to have "0" as the first start bit of his `word[00]` byte, but it doesn't have to be "0");
- 12 normal 8-bit bytes.

That's it. This is the entire protocol, then again 16 bits of "1". And so on, in a loop, until we disconnect TE2-E1.

Each byte (including the first 12-bit one) consists of a start "0" bit, data bits (12 or 8), and two stop "1" bits.

In total, the frame consists of 16 + 13 * (1 + 2) + 12 * 8 + 12 = 163 bits.

The duration of one bit is about 8.2ms.
The frame duration is about 1.34 seconds.

So we have the following data bytes:
- The first one is 12 bits, its meaning is still unclear, but I named it `IDENTIFIER` in the program, and it is displayed simply from left to right bit by bit;
- 10 bytes with values;
- 2 bytes with flags, each byte has 8 bits, and respectively flags.

A designation like `1.A` means that it is the first byte, and it represents a number.
`12.1` means that it is the 12th byte, and the first bit is taken from it (flag bit designations).

Well, that's all there is.

---

#### Table of Parameters and Calculations

| Parameter                   | Description                                     | How to calculate                                      |
|-----------------------------|-------------------------------------------------|-------------------------------------------------------|
| Identifier                  | The meaning is not yet clear                    | Just displayed from left to right                     |
| 1.A                         | Injector pulse width                            | =1.A * 0.125 (ms)                                     |
| 2.A                         | Ignition timing angle                           | =2.A * 0.47 - 30 (degrees)                            |
| 3.A                         | Idle Air Control valve state                    | For different types of IAC valves, different formulas:<br> =3.A / 255 * 1000 (%)<br> =3.A (steps) |
| 4.A                         | Engine speed                                    | =4.A * 25 (RPM)                                       |
| 5.A                         | Mass Air Flow sensor (MAP/MAF)                  | For different types of sensors, different formulas:<br> =5.A * 0.6515 (kPa)<br> =5.A * 4.886 (mmHg)<br> =5.A * 0.97 (kPa) (for turbo engines)<br> =5.A * 7.732 (mmHg) (for turbo engines)<br> =5.A (g/sec) (this formula for MAF has not been found)<br> =5.A / 255 * 5 (Volts) (voltage on the MAF sensor) |
| 6.A                         | Engine Coolant Temperature                      | The temperature sensor can have a direct or reverse dependence. For direct, assume X=6.A, for reverse X=255-6.A<br> Depending on the value of X, different formulas:<br> 0..14: =(X-5) * 2 - 60<br> 15..38: =(X-15) * 0.83 - 40<br> 39..81: =(X-39) * 0.47 - 20<br> 82..134: =(X-82) * 0.38<br> 135..179: =(X-135) * 0.44 + 20<br> 180..209: =(X-180) * 0.67 + 40<br> 210..227: =(X-210) * 1.11 + 60<br> 228..236: =(X-228) * 2.11 + 80<br> 237..242: =(X-237) * 3.83 + 99<br> 243..255: =(X-243) * 9.8 + 122<br> Temperature in degrees Celsius. |
| 7.A                         | Throttle Position Sensor                        | =7.A / 2 (degrees)<br> =7.A / 1.8 (%)                 |
| 8.A                         | Vehicle speed                                   | = 8.A (km/h)                                          |
| 9.A                         | Correction for inline engines / correction for the first half | =9.A * 5 / 256 (volts)                                |
| 10.A                        | Correction for the second half                  | =10.A * 5 / 256 (volts)                               |
| 11.0                        | Over-enrichment after startup                   | 1-On                                                  |
| 11.1                        | Cold engine                                     | 1-Yes                                                 |
| 11.4                        | Detonation                                      | 1-Yes (no one has really checked it)                  |
| 11.5                        | Feedback on the lambda probe                    | 1-On                                                  |
| 11.6                        | Additional enrichment                           | 1-On                                                  |
| 12.0                        | Starter                                         | 1-On                                                  |
| 12.1                        | Idle indicator (Throttle)                       | 1-Yes (Closed)                                        |
| 12.2                        | Air conditioning                                | 1-On                                                  |
| 12.3                        | Neutral                                         | 1-On                                                  |
| 12.4                        | Mixture / mixture for the first half            | 1-Rich, 0-Lean                                        |
| 12.5                        | Mixture for the second half                     | 1-Rich, 0-Lean                                        |
| 12.7                        | Diagnostics                                     | Still not understood what it is.                      |

---
