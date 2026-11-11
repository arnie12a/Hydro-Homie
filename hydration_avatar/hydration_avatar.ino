
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// LCD connections - Arduino Nano ESP32
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

Adafruit_ST7789 tft =
  Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Colors
#define DARK_BLUE  0x10A3
#define LIGHT_BLUE 0x7D7C
#define CYAN_BLUE  0x2DDF
#define RED_COLOR  0xF800
#define YELLOW_COLOR 0xFFE0
#define GREEN_COLOR 0x07E0
#define FACE_COLOR 0x0841

enum HydrationState {
  PARCHED,
  NEUTRAL,
  WELL_HYDRATED
};

HydrationState currentState = PARCHED;

unsigned long lastChange = 0;
const unsigned long changeInterval = 4000;

// Draw a centered text string
void drawCenteredText(String text, int y,
                      uint16_t color, int size) {
  tft.setTextSize(size);
  tft.setTextColor(color);

  int16_t x1, y1;
  uint16_t w, h;

  tft.getTextBounds(
    text, 0, y, &x1, &y1, &w, &h
  );

  tft.setCursor((240 - w) / 2, y);
  tft.print(text);
}

// Draw water droplet
void drawWaterDrop(uint16_t color) {
  // Pointed top
  tft.fillTriangle(
    120, 65,
    65, 150,
    175, 150,
    color
  );

  // Rounded bottom
  tft.fillCircle(120, 150, 55, color);
}

// Draw parched expression
void drawParchedFace() {
  drawWaterDrop(LIGHT_BLUE);

  // Tired eyes
  tft.drawLine(88, 150, 105, 157, FACE_COLOR);
  tft.drawLine(135, 157, 152, 150, FACE_COLOR);

  // Sad mouth
  tft.drawLine(103, 185, 112, 178, FACE_COLOR);
  tft.drawLine(112, 178, 128, 178, FACE_COLOR);
  tft.drawLine(128, 178, 137, 185, FACE_COLOR);
}

// Draw neutral expression
void drawNeutralFace() {
  drawWaterDrop(CYAN_BLUE);

  tft.fillCircle(99, 150, 6, FACE_COLOR);
  tft.fillCircle(141, 150, 6, FACE_COLOR);

  tft.drawFastHLine(105, 184, 30, FACE_COLOR);
}

// Draw happy expression
void drawHappyFace() {
  drawWaterDrop(ST77XX_CYAN);

  // Happy eyes
  tft.drawLine(88, 150, 99, 143, FACE_COLOR);
  tft.drawLine(99, 143, 110, 150, FACE_COLOR);

  tft.drawLine(130, 150, 141, 143, FACE_COLOR);
  tft.drawLine(141, 143, 152, 150, FACE_COLOR);

  // Smile
  tft.drawLine(96, 178, 107, 190, FACE_COLOR);
  tft.drawLine(107, 190, 133, 190, FACE_COLOR);
  tft.drawLine(133, 190, 144, 178, FACE_COLOR);
}

void drawScreen(HydrationState state) {
  tft.fillScreen(DARK_BLUE);

  drawCenteredText(
    "HYDRATION HOMIE",
    20,
    ST77XX_WHITE,
    2
  );

  switch (state) {
    case PARCHED:
      drawParchedFace();
      drawCenteredText(
        "PARCHED", 225, RED_COLOR, 2
      );
      drawCenteredText(
        "Need water!", 255, ST77XX_WHITE, 1
      );
      break;

    case NEUTRAL:
      drawNeutralFace();
      drawCenteredText(
        "NEUTRAL", 225, YELLOW_COLOR, 2
      );
      drawCenteredText(
        "Keep drinking!", 255, ST77XX_WHITE, 1
      );
      break;

    case WELL_HYDRATED:
      drawHappyFace();
      drawCenteredText(
        "WELL HYDRATED", 225, GREEN_COLOR, 2
      );
      drawCenteredText(
        "Great job!", 255, ST77XX_WHITE, 1
      );
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  tft.init(240, 280);
  tft.setRotation(0);

  drawScreen(currentState);

  Serial.println("Hydration Homie Started!");
}

void loop() {
  if (millis() - lastChange >= changeInterval) {
    lastChange = millis();

    if (currentState == PARCHED) {
      currentState = NEUTRAL;
      Serial.println("State: NEUTRAL");
    }
    else if (currentState == NEUTRAL) {
      currentState = WELL_HYDRATED;
      Serial.println("State: WELL HYDRATED");
    }
    else {
      currentState = PARCHED;
      Serial.println("State: PARCHED");
    }

    drawScreen(currentState);
  }
}
