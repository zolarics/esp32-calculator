# Wiring Guide

**Step 0**. Ensure you have the following materials:
> ESP32
> OLED Display
> Membrane Switch Module (Keypad)
> 12 Male to Male jumper cables
> 2 Breadboards

**Step 1**. Connect the two breadboards together in parellel. This will grant you enough room to wire everything.

**Step 2**. Place the ESP32 in-between the two boards. You may have to bend one of the breadboards for this to work. See Figures 2.0 and 2.1 for a visual example.

**Step 3**. Place the OLED display on one of the breadboards. Make sure you have enough room to connect at-least 1 full row of wires.

**Step 4**. Connect the following wires from the ESP32 to the OLED display:
[3V3] (ESP32) -> [VCC] (OLED)
[GND] (ESP32) -> [GND] (OLED)
[D21] (ESP32) -> [SDA] (OLED)
[D22] (ESP32) -> [SCL] (OLED)
See Figure 4.0 for a visual example
- Note: The respective pins for the keypad can be modified in the "calculator.ino" code on lines 35 and 36.

**Step 5**. Connect the following wires *starting from the "*" button and going right* on the keypad to the ESP32:
[Connector 1] (Keypad) -> [D5] (ESP32)
[Connector 2] (Keypad) -> [D18] (ESP32)
[Connector 3] (Keypad) -> [D19] (ESP32)
[Connector 4] (Keypad) -> [D23] (ESP32)
[Connector 5] (Keypad) -> [D27] (ESP32)
[Connector 6] (Keypad) -> [D26] (ESP32)
[Connector 7] (Keypad) -> [D25] (ESP32)
[Connector 8] (Keypad) -> [D33] (ESP32)
See Figure 5.0 for a visual example of just the keypad pins, and see Figure 5.1 for a visual example with everything attached.
- Note: The respective pins for the keypad can be modified in the "calculator.ino" code on lines 46 and 47.
