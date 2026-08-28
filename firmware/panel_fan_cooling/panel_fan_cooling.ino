/*
 * Panel / enclosure fan cooling
 * -------------------------------------------------------------------------
 * Simple thermostat: read an analog temperature sensor (e.g. LM35 on A0),
 * convert to degrees Celsius, and switch a fan on above a set point.
 *
 * The fan pin drives a relay / transistor. The HIGH/LOW sense below assumes
 * an active-LOW relay board (LOW = fan on); flip it for active-HIGH.
 *
 * conversion: LM35 gives 10 mV/degC; analogRead step = 5000/1024 mV
 *             => degC = analogRead * (5000 / 1024) / 10 = analogRead * 0.488
 */

const int FAN_PIN      = 12;
const int TEMP_PIN     = A0;
const float SETPOINT_C = 23.0;

void setup() {
  pinMode(FAN_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  float tempC = analogRead(TEMP_PIN) * 0.48828125f;

  Serial.print("Temperature = ");
  Serial.print(tempC);
  Serial.println(" C");

  if (tempC > SETPOINT_C) {
    digitalWrite(FAN_PIN, LOW);    // fan ON  (active-LOW relay)
  } else {
    digitalWrite(FAN_PIN, HIGH);   // fan OFF
  }

  delay(1000);
}
