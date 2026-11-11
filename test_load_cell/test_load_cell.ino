
#include "HX711.h"

#define DOUT 2
#define CLK  3

HX711 scale;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Starting HX711 Load Cell Test...");

  scale.begin(DOUT, CLK);

  if (scale.wait_ready_timeout(2000)) {
    Serial.println("HX711 connected successfully!");

    Serial.println("Remove all weight from the sensor.");
    delay(3000);

    scale.tare(10);

    Serial.println("Scale zeroed!");
  } else {
    Serial.println("ERROR: HX711 not detected!");
  }
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.read_average(10);

    Serial.print("Raw Weight Reading: ");
    Serial.println(reading);
  } else {
    Serial.println("HX711 not responding!");
  }

  delay(500);
}
