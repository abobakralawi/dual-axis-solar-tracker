# Tracking algorithm

The tracker compares opposing light sensors and nudges each axis until the
two readings are close. It does not use time or the sun's position.

## Each cycle

1. Read the four sensors as `1023 - analogRead(pin)`, so a bigger number
   means brighter.
2. East/West: `ewDiff = east - west`.
   - `abs(ewDiff) <= 10`: hold.
   - `ewDiff < 0`: west brighter, pulse the E/W actuator toward west.
   - `ewDiff > 0`: east brighter, pulse it toward east.
3. North/South: same, with `north - south` and a dead-band of 20.
4. A pulse energises the actuator's relay pair for 800 ms, then stops.
5. Wait, then repeat.

## Night

If all four sensors read below 30, the firmware runs the E/W actuator
toward the east end stop (a long run, stopped by the actuator's own limit
switch), then checks the sensors once a minute until the light returns.
The N/S tilt axis is left where it is.

## Notes

- Serial runs at 1200 baud.
- Moves are timed pulses. The actuators' own limit switches stop them at
  the ends of travel.
- The analog reads are not averaged.
