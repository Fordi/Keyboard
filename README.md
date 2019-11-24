# Keyboard

Keyboard matrix decoder sketch for Teensy 3.2 / Teensy LC

Based on https://github.com/thedalles77/USB_Laptop_Keyboard_Controller

## How to use

1. Wire up the board as seen here:
https://www.instructables.com/id/How-to-Make-a-USB-Laptop-Keyboard-Controller/

2. Configuration

    1. Make sure [keymap.h](./keymap.h) is effectively empty (but exists)
    2. Open [config.h](./config.h) in a text editor, and comment / uncomment
      `#define DIODES` based on what you know about your matrix.  If you're
      unsure, leave it on.

3. Scan

  1. Build and install this sketch.  Unplug the USB cable once it's uploaded.
  2. Copy `scancodes.txt` to a new file named for your keyboard model (e.g.,
    `Keyboards/scancodes_MY-KEYBOARD.txt`), and open it in a text editor.
  3. Place your cursor on the first empty line, and plug in your keyboard.  After
    5 seconds (or whatever is set in `INITIAL_DELAY` in [config.h](./config.h)),
    either `Teensy32` or `TeensyLC` will appear, depending on which board you're
    using.  If this value doesn't match your board, something fishy's going on.
  4. For each line, tap the key that corresponds on the keyboard.  Use your
    system's keyboard to navigate around if a key doesn't work.
      - If some keys don't react, you probably have a poorly soldered line in
        there. Scan all the keys that work, then run
          `node mapper.js Keyboards/scancodes_MY-KEYBOARD.txt`
        it'll tell you which pins never come up; those are probably not
        connected; check your soldering work and start again.
      - If you're not pressing any keys, and you see an unlikely short (e.g.,
        `1 26 Shorted`) appear, that's probably OK, as they're likely to be a
        common ground.  The scanner will exclude those pins from further
        consideration.  Delete the inserted text, uncomment the line, and
        start scanning.
      - If you're not pressing any keys and you see a likely short appear (e.g.,
        `7 8 Shorted`), check your soldering work on those two FPC pins.

4. Install

  1. Run `node mapper.js Keyboards/scancodes_MY-KEYBOARD.txt`
      - This will generate `keymap.h`
  2. Install this sketch again.

5. Troubleshoot

  - If you didn't know what to set DIODES to, it's likely that you have some
    out-of-order matrix coordinates.  Toggle it, and repeat from step 3 (scan)

  - If you need further help, open an issue on this github project, and I'll see
    what I can help with.
