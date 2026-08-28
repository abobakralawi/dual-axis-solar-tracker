/*
 * Dual-axis solar tracker : main firmware
 * -------------------------------------------------------------------------
 * Target board : Arduino Uno
 * Sensors      : 4x light sensors (East / West / South / North) on A0..A3
 *                wired as voltage dividers; a brighter cell pulls the
 *                analog reading DOWN, so the code uses (1023 - analogRead).
 * Drivers      : 2x 12 V linear actuators via a 4-channel relay board
 *                  - East / West  axis : relays on D10, D9
 *                  - North / South axis: relays on D8,  D7
 *                Each axis uses two relays for polarity reversal (H-bridge
 *                style). Both relays LOW = actuator stopped.
 * Behaviour    : compare opposing sensor pairs; if the difference exceeds a
 *                dead-band, nudge the matching actuator toward the brighter
 *                side. When all four sensors read below NIGHT_LEVEL the E/W
 *                actuator is run to its east end stop for ~160 s straight,
 *                so the actuators MUST have built-in limit switches. The
 *                loop then waits for morning.
 *
 * Cleaned up for readability; the pin map, thresholds and control logic are
 * unchanged from the prototype that ran in Sana'a, 2018.
 * See ../../docs/ for wiring and the tracking algorithm.
 */

// ---- Pin map -------------------------------------------------------------
const int EAST_SENSOR_PIN  = A0;
const int WEST_SENSOR_PIN   = A1;
const int SOUTH_SENSOR_PIN  = A2;
const int NORTH_SENSOR_PIN  = A3;

const int EW_RELAY_1_PIN = 10;   // East / West actuator, direction A
const int EW_RELAY_2_PIN = 9;    // East / West actuator, direction B
const int NS_RELAY_1_PIN = 8;    // North / South actuator, direction A
const int NS_RELAY_2_PIN = 7;    // North / South actuator, direction B

// ---- Tuning -------------------------------------------------------------
const int EW_DEADBAND   = 10;   // ignore E/W differences smaller than this
const int NS_DEADBAND   = 20;   // ignore N/S differences smaller than this
const int NIGHT_LEVEL   = 30;   // all sensors below this  => night / park
const int STEP_MS       = 800;  // relay-on time for a one-"step" nudge (ms)
const int READ_PERIOD_MS = 5000;

// ---- Actuator identifiers --------------------------------------------------
enum Axis      { AXIS_EW = 1, AXIS_NS = 2 };
enum Direction { DIR_A  = 1, DIR_B  = 2 };

int eastReading  = 0;
int westReading  = 0;
int southReading = 0;
int northReading = 0;

void moveActuator(Axis axis, Direction dir, int steps);
void stopAllActuators();
void readSensors();

void setup() {
  Serial.begin(1200);
  pinMode(EW_RELAY_1_PIN, OUTPUT);
  pinMode(EW_RELAY_2_PIN, OUTPUT);
  pinMode(NS_RELAY_1_PIN, OUTPUT);
  pinMode(NS_RELAY_2_PIN, OUTPUT);
  stopAllActuators();
  Serial.println("Running...");
}

void loop() {
  readSensors();

  // --- East / West axis (azimuth) ---
  int ewDiff = eastReading - westReading;
  Serial.print("EastWestDifference: ");
  Serial.println(ewDiff);

  if (abs(ewDiff) <= EW_DEADBAND) {
    Serial.println("E/W within dead-band, holding");
  } else if (ewDiff < 0) {
    Serial.println("West is brighter -> move West");
    moveActuator(AXIS_EW, DIR_A, 1);
  } else {
    Serial.println("East is brighter -> move East");
    moveActuator(AXIS_EW, DIR_B, 1);
  }

  // --- North / South axis (elevation) ---
  int nsDiff = northReading - southReading;
  Serial.print("NorthSouthDifference: ");
  Serial.println(nsDiff);

  if (abs(nsDiff) <= NS_DEADBAND) {
    Serial.println("N/S within dead-band, holding");
  } else if (nsDiff < 0) {
    Serial.println("South is brighter -> move South");
    moveActuator(AXIS_NS, DIR_A, 1);
  } else {
    Serial.println("North is brighter -> move North");
    moveActuator(AXIS_NS, DIR_B, 1);
  }

  // --- Night: park the panel and wait for daylight ---
  if (eastReading  < NIGHT_LEVEL && westReading  < NIGHT_LEVEL &&
      northReading < NIGHT_LEVEL && southReading < NIGHT_LEVEL) {
    Serial.println("Night time -> parking panel");
    moveActuator(AXIS_EW, DIR_B, 200);   // return toward the East / home stop

    while (eastReading  < NIGHT_LEVEL && westReading  < NIGHT_LEVEL &&
           northReading < NIGHT_LEVEL && southReading < NIGHT_LEVEL) {
      Serial.println("Waiting for the sun...");
      delay(60000);
      readSensors();
    }
  }
}

// -------------------------------------------------------------------------
void readSensors() {
  eastReading  = 1023 - analogRead(EAST_SENSOR_PIN);
  westReading  = 1023 - analogRead(WEST_SENSOR_PIN);
  southReading = 1023 - analogRead(SOUTH_SENSOR_PIN);
  northReading = 1023 - analogRead(NORTH_SENSOR_PIN);

  Serial.print("E="); Serial.print(eastReading);
  Serial.print(" W="); Serial.print(westReading);
  Serial.print(" S="); Serial.print(southReading);
  Serial.print(" N="); Serial.println(northReading);
  delay(READ_PERIOD_MS);
}

void moveActuator(Axis axis, Direction dir, int steps) {
  int r1 = (axis == AXIS_EW) ? EW_RELAY_1_PIN : NS_RELAY_1_PIN;
  int r2 = (axis == AXIS_EW) ? EW_RELAY_2_PIN : NS_RELAY_2_PIN;

  if (dir == DIR_A) {
    digitalWrite(r1, HIGH);
    digitalWrite(r2, LOW);
  } else {
    digitalWrite(r1, LOW);
    digitalWrite(r2, HIGH);
  }

  delay((long)steps * STEP_MS);
  stopAllActuators();
}

void stopAllActuators() {
  digitalWrite(EW_RELAY_1_PIN, LOW);
  digitalWrite(EW_RELAY_2_PIN, LOW);
  digitalWrite(NS_RELAY_1_PIN, LOW);
  digitalWrite(NS_RELAY_2_PIN, LOW);
}
