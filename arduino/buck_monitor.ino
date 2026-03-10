#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ----------------------------
// LCD Setup
// ----------------------------
LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Address 0x27, 16x2 LCD

// ----------------------------
// Pin Configuration
// ----------------------------
#define VIN_PIN   A0  // Input voltage
#define VOUT_PIN  A1   // Output voltage
#define IOUT_PIN  A2   // Output current (ACS712)
#define POT_PIN   A3  // Potentiometer (47kΩ)

// ----------------------------
// Calibration & Variables
// ----------------------------
float prevPotValue = 0;
String potStatus = "Stable";

const float VREF = 5.0;            // Arduino reference voltage
const float VOLT_DIV_RATIO = 5.1; // Adjust for your voltage divider
const float ACS_OFFSET = 2.49;      // ACS712 output offset (V)
const float ACS_SENSITIVITY = 0.185; // 185 mV/A for 5A sensor

// ----------------------------
// Function to read voltage
// ----------------------------
float readVoltage(int pin) {
  int adc = analogRead(pin);
  float voltage = (adc * VREF) / 1023.0;
  return voltage * VOLT_DIV_RATIO;
}

// ----------------------------
// Function to read current
// ----------------------------
float readCurrent(int pin) {
  int adc = analogRead(pin);
  float voltage = (adc * VREF) / 1023.0;
  float current = (voltage - ACS_OFFSET) / ACS_SENSITIVITY;
  return current;
}

// ----------------------------
// Detect potentiometer rotation
// ----------------------------
String detectPotRotation(float currentValue) {
  String result;
  if (currentValue > prevPotValue + 5) {
    result = "Increased";
  } else if (currentValue < prevPotValue - 5) {
    result = "Decreased";
  } else {
    result = "Stable";
  }
  prevPotValue = currentValue;
  return result;
}

// ----------------------------
// Setup
// ----------------------------
void setup() {
  Serial.begin(9600);

  lcd.begin();       // Use library-specific begin() with no arguments
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voltage Monitor");
  delay(1500);
}

// ----------------------------
// Main Loop
// ----------------------------
void loop() {
  float vin = readVoltage(VIN_PIN);
  float vout = readVoltage(VOUT_PIN);
  float iout = readCurrent(IOUT_PIN);
  float potValue = analogRead(POT_PIN);

  potStatus = detectPotRotation(potValue);

  // ---- LCD Display ----
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IN:");
  lcd.print(vin, 1);
  lcd.print("V OUT:");
  lcd.print(vout, 1);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("I:");
  lcd.print(iout, 2);
  lcd.print("A ");
  lcd.print(potStatus);

  // ---- Serial Monitor ----
  Serial.print("Vin: "); Serial.print(vin, 2); Serial.print("V  ");
  Serial.print("Vout: "); Serial.print(vout, 2); Serial.print("V  ");
  Serial.print("Iout: "); Serial.print(iout, 2); Serial.print("A  ");
  Serial.print("Pot: "); Serial.print(potValue);
  Serial.print(" -> "); Serial.println(potStatus);

  delay(1000);  // Update every second
}