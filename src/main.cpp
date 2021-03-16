#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SerialCommand.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_ST7920_128X64_F_HW_SPI display(U8G2_R2, 12);
SerialCommand sCmd;

const char *cpuTemp;
const char *cpuUsage;
const char *gpuTemp;
const char *gpuUsage;

void loop() {
  sCmd.readSerial();
}

void writeToDisplay() {
  display.setCursor(0, 11);

  display.print("CPU: ");
  display.print(cpuUsage);
  display.print("% @ ");
  display.print(cpuTemp);
  display.print("°");

  
  
  
  display.sendBuffer();
}

void cpuTempCmd() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    cpuTemp = arg;
    writeToDisplay();
  } else {
    Serial.println("Error");
  }
}

void gpuTempCmd() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    display.drawStr(0, 11, arg);
    display.sendBuffer();
  } else {
    Serial.println("Error");
  }
}

void cpuUsageCmd() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    cpuUsage = arg;
  } else {
    Serial.println("Error");
  }
}

void gpuUsageCmd() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    display.drawStr(0, 11, arg);
    display.sendBuffer();
  } else {
    Serial.println("Error");
  }
}

void unrecognized(const char *command) {
  Serial.println("Bitch what?");
}

void setup() {
  display.begin();
  display.clear();
  display.setFont(u8g2_font_t0_11_mf);
  display.enableUTF8Print();

  Serial.begin(9600);

  sCmd.addCommand("ct", cpuTempCmd);
  sCmd.addCommand("gt", gpuTempCmd);
  sCmd.addCommand("cu", cpuUsageCmd);
  sCmd.addCommand("gu", gpuUsageCmd);
  sCmd.setDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")
}
