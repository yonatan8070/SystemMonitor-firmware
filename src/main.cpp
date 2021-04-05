#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SerialCommand.h>
#include <U8g2lib.h>
#include <Wire.h>

#include "config.h"

U8G2_ST7920_128X64_F_HW_SPI display(U8G2_R2, 12);
SerialCommand sCmd;

int cpuTemp = -1;
int cpuUsage = -1;
int gpuTemp = -1;
int gpuUsage = -1;
int memUsage = -1;

int coreUsage[CORE_COUNT] = {100, 50, 25, 0};

void drawProgressBar(int x, int y, int w, int h, int filled) {
  filled = 100 - filled;

  display.drawFrame(x, y, w, h);

  int fillOffset = (h * (filled / 100.0F));
  int fillHeight = h - fillOffset;

  display.drawBox(x + 1, y + fillOffset, w - 2, fillHeight);
}

void loop() {
  sCmd.readSerial();
}

void drawCores() {
  for (int i = 0; i < CORE_COUNT; i++) {
    drawProgressBar((128 - (10 * CORE_COUNT)) + (i * 10), 28, 8, 36, coreUsage[i]);
  }
}

// Draw everything
void draw() {
  display.clearBuffer();
  display.setCursor(0, 8);

  display.print("CPU: ");
  display.print(cpuUsage);
  display.print("% @ ");
  display.print(cpuTemp);
  display.print("°");  // Extra spaces to overwrite old characters that might still be there

  display.setCursor(0, 16);

  display.print("GPU: ");
  display.print(gpuTemp);
  display.print("°");

  display.setCursor(0, 24);

  display.print("RAM: ");
  display.print(memUsage);
  display.print("MB of ");
  display.print(MEMORY_MAX);
  display.print("MB");

  drawCores();

  display.sendBuffer();
}

void cpuTempCmd() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    cpuTemp = atoi(arg);
  } else {
    Serial.println("Error");
  }
}

void gpuTempCmd() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    gpuTemp = atoi(arg);
  } else {
    Serial.println("Error");
  }
}

void cpuUsageCmd() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    cpuUsage = atoi(arg);
  } else {
    Serial.println("Error");
  }
}

void gpuUsageCmd() {
  char *arg = sCmd.next();
  if (arg != NULL) {
    Serial.println(arg);
    gpuUsage = atoi(arg);
  } else {
    Serial.println("Error");
  }
}

void cpuCoreCmd() {
  for (int i = 0; i < CORE_COUNT; i++) {
    coreUsage[i] = atoi(sCmd.next());
  }
}

void memoryUsageCmd() {
  memUsage = atoi(sCmd.next());
}

void unrecognized(const char *command) {
  Serial.println("Bitch what?");
}

void setup() {
  display.begin();
  display.clear();
  display.setFont(u8g2_font_5x7_tf);
  display.enableUTF8Print();

  Serial.begin(9600);

  sCmd.addCommand("ct", cpuTempCmd);
  sCmd.addCommand("gt", gpuTempCmd);
  sCmd.addCommand("cu", cpuUsageCmd);
  sCmd.addCommand("gu", gpuUsageCmd);
  sCmd.addCommand("cc", cpuCoreCmd);
  sCmd.addCommand("mu", memoryUsageCmd);
  sCmd.addCommand("draw", draw);
  sCmd.setDefaultHandler(unrecognized);
}
