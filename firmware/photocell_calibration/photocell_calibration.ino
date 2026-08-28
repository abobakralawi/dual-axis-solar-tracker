/*
 * Photocell calibration / light-level readout
 * -------------------------------------------------------------------------
 * Reads one photocell voltage divider on A0, auto-learns its running
 * min/max, and prints a normalised 0..100 light level to serial. Handy for
 * checking sensor response and picking the tracker's dead-band thresholds.
 *
 * Wiring: photocell and a fixed resistor form a divider between 5 V and GND;
 * the midpoint goes to A0.
 *
 * Note: here a brighter light gives a HIGHER reading. The main tracker
 * sketch uses the opposite convention (1023 - analogRead).
 */

const int PHOTOCELL_PIN = A0;

int minLight;
int maxLight;

void setup() {
  Serial.begin(9600);

  int level = analogRead(PHOTOCELL_PIN);
  minLight = level - 20;
  maxLight = level;
}

void loop() {
  int level = analogRead(PHOTOCELL_PIN);

  if (level < minLight) minLight = level;
  if (level > maxLight) maxLight = level;

  int normalised = map(level, minLight, maxLight, 0, 100);
  Serial.println(normalised);

  delay(1000);
}
