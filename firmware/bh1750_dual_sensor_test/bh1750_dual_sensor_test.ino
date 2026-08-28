/*
 * BH1750 dual light-sensor test
 * -------------------------------------------------------------------------
 * Bench test used while evaluating the BH1750FVI digital lux sensor as an
 * alternative to the analog light sensors. Two sensors share the I2C bus at
 * the two possible addresses (ADDR low = 0x23, ADDR high = 0x5C) and their
 * lux readings are printed to the serial monitor.
 *
 * Wiring (Arduino Uno):
 *   VCC  -> 3.3 V        SDA -> A4
 *   GND  -> GND          SCL -> A5
 *   East sensor  ADDR -> HIGH (0x5C)
 *   West sensor  ADDR -> LOW  (0x23)
 *
 * On the Uno, I2C uses A4/A5, so this test cannot run at the same time as
 * the four analog sensors on A0..A3. That is fine: the build uses one set
 * or the other, not both.
 *
 * Dependency: "BH1750FVI" library by Mohannad Rawashdeh
 *   https://github.com/Genotronex/BH1750FVI_Master
 *   (Arduino IDE: Sketch > Include Library > Add .ZIP Library, then restart.)
 *
 * The final tracker used the analog sensors; this stayed a test sketch.
 */

#include <Wire.h>
#include "BH1750FVI.h"

BH1750FVI lightSensorEast;
BH1750FVI lightSensorWest;

void setup() {
  Serial.begin(9600);

  lightSensorEast.begin();
  lightSensorWest.begin();

  lightSensorEast.SetAddress(Device_Address_H);   // 0x5C, ADDR pin HIGH
  lightSensorWest.SetAddress(Device_Address_L);   // 0x23, ADDR pin LOW

  lightSensorEast.SetMode(Continuous_H_resolution_Mode);
  lightSensorWest.SetMode(Continuous_H_resolution_Mode);

  Serial.println("Running...");
}

void loop() {
  uint16_t east = lightSensorEast.GetLightIntensity();
  uint16_t west = lightSensorWest.GetLightIntensity();

  Serial.print("East: ");
  Serial.print(east);
  Serial.print(" lux    West: ");
  Serial.print(west);
  Serial.println(" lux");

  delay(8000);
}
