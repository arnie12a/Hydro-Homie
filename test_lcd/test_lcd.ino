
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// Nano ESP32 pins (Arduino numbering)
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

Adafruit_ST7789 tft =
  Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void drawAvatar() {
  tft.fillScreen(ST77XX_BLACK);

  // Header
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(25, 20);
  tft.println("HYDRATION");

  tft.setCursor(65, 45);
  tft.println("HOMIE");

  // Avatar face
  tft.fillCircle(120, 145, 65, ST77XX_CYAN);

  // Eyes
  tft.fillCircle(95, 130, 8, ST77XX_BLACK);
  tft.fillCircle(145, 130, 8, ST77XX_BLACK);

  // Smile
  tft.drawLine(95, 165, 110, 175, ST77XX_BLACK);
  tft.drawLine(110, 175, 130, 175, ST77XX_BLACK);
  tft.drawLine(130, 175, 145, 165, ST77XX_BLACK);

  // Footer
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.setCursor(25, 245);
  tft.println("Stay Hydrated!");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starting Waveshare LCD Test...");

  // Initialize 240x280 ST7789 display
  tft.init(240, 280);
  tft.setRotation(0);

  // Color test
  tft.fillScreen(ST77XX_RED);
  delay(1000);

  tft.fillScreen(ST77XX_GREEN);
  delay(1000);

  tft.fillScreen(ST77XX_BLUE);
  delay(1000);

  // Draw Hydration Homie
  drawAvatar();

  Serial.println("LCD Test Complete!");
}

void loop() {
  // Nothing needed yet
}
