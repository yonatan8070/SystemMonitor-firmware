#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SerialCommand.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
SerialCommand sCmd;

void loop() {
  sCmd.readSerial();
}

void cpuTemp() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    lcd.setCursor(0, 0);
    lcd.print("CPU: ");
    lcd.print(arg);
  } else {
    Serial.println("Error");
  }
}

void gpuTemp() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    lcd.setCursor(9, 0);
    lcd.print("GPU: ");
    lcd.print(arg);
  } else {
    Serial.println("Error");
  }
}

void cpuUsage() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    lcd.setCursor(0, 1);
    lcd.print("CPU: ");
    lcd.print(arg);
  } else {
    Serial.println("Error");
  }
}

void gpuUsage() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    lcd.setCursor(9, 1);
    lcd.print("GPU: ");
    lcd.print(arg);
  } else {
    Serial.println("Error");
  }
}

void unrecognized(const char *command) {
  Serial.println("Bitch what?");
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  Serial.begin(9600);

  sCmd.addCommand("ct", cpuTemp);
  sCmd.addCommand("gt", gpuTemp);
  sCmd.addCommand("cu", cpuUsage);
  sCmd.addCommand("gu", gpuUsage);
  sCmd.setDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")
}
