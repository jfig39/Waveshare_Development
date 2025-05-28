// GxEPD2_HelloWorldLogo.ino - ESP32 with Waveshare 1.54" V2 e-Paper (SSD1681)

#define ENABLE_GxEPD2_GDEH0154D67

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "logo.h"  // Bitmap logo converted from PNG

#include <BH1750.h>
#include <Wire.h>

// Pin mapping for ESP32-DevKitM-1
#define EPD_CS   5   // Chip Select
#define EPD_DC   10  // Data/Command
#define EPD_RST  9   // Reset
#define EPD_BUSY 4   // Busy


// Define display object using SSD1681 controller (200x200)
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));



BH1750 lightMeter;

const char HelloWorld[] = "Hello World!";

void helloWorld() {
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;

  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(HelloWorld);
  } while (display.nextPage());
}

void showLogo() {
  display.setRotation(1);
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(
      (display.width() - logo_width) / 2,
      (display.height() - logo_height) / 2,
      logo_bitmaps,
      logo_width,
      logo_height,
      GxEPD_BLACK
    );
  } while (display.nextPage());
}

void setup() {
  Serial.begin(115200);
   Wire.begin(21, 22); // SDA, SCL (set according to your wiring)

    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  } else {
    Serial.println(F("Error initialising BH1750"));
  }


  display.init(115200, true, 2, false);  // 2ms reset pulse

  helloWorld();     // Display Hello World
  delay(2000);      // Wait for 2 seconds
  showLogo();       // Display the logo
  display.hibernate();
}

void loop() {
 if (lightMeter.measurementReady()) {
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
}

}
