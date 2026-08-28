/*
 * Actuator / relay bring-up test
 * -------------------------------------------------------------------------
 * Drives each 12 V linear actuator through its two relay channels, in both
 * directions, with a stop between moves. Use it to confirm wiring and which
 * way each actuator travels before running the tracker.
 *
 * Pins match the main sketch and docs/wiring.md:
 *   D10, D9 -> East / West actuator relays
 *   D8,  D7 -> North / South actuator relays
 *
 * For one actuator: the two channels in opposite states -> it moves;
 * both channels the same -> it holds still.
 */

const int EW_RELAY_1 = 10;
const int EW_RELAY_2 = 9;
const int NS_RELAY_1 = 8;
const int NS_RELAY_2 = 7;

const int MOVE_MS = 2000;
const int STOP_MS = 1000;

void setup() {
  pinMode(EW_RELAY_1, OUTPUT);
  pinMode(EW_RELAY_2, OUTPUT);
  pinMode(NS_RELAY_1, OUTPUT);
  pinMode(NS_RELAY_2, OUTPUT);
}

void exercise(int relayA, int relayB) {
  digitalWrite(relayA, HIGH);      // one direction
  digitalWrite(relayB, LOW);
  delay(MOVE_MS);

  digitalWrite(relayA, LOW);       // stop
  digitalWrite(relayB, LOW);
  delay(STOP_MS);

  digitalWrite(relayA, LOW);       // the other direction
  digitalWrite(relayB, HIGH);
  delay(MOVE_MS);

  digitalWrite(relayA, LOW);       // stop
  digitalWrite(relayB, LOW);
  delay(STOP_MS);
}

void loop() {
  exercise(EW_RELAY_1, EW_RELAY_2);    // East / West actuator
  exercise(NS_RELAY_1, NS_RELAY_2);    // North / South actuator
}
