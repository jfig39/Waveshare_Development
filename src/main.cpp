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

const unsigned long updateInterval = 30000; // 30 seconds

void displayLux(float lux, uint16_t x, uint16_t y) {
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);

  // Define region to update
  uint16_t w = 120;
  uint16_t h = 30;
  display.setPartialWindow(x, y - 20, w, h);  // Adjust y for text baseline

  display.firstPage();
  do {
    display.fillRect(x, y - 20, w, h, GxEPD_WHITE);  // Clear the target area only
    display.setCursor(x, y);
    display.print("Lux: ");
    display.print(lux, 1); // One decimal point
  } while (display.nextPage());
}

void showLogo() {
  display.setRotation(1);

  // Define the top half of the screen as the draw region
  int16_t win_x = 0;
  int16_t win_y = 0;
  int16_t win_w = display.width();
  int16_t win_h = display.height() / 2;

  display.setPartialWindow(win_x, win_y, win_w, win_h);

  display.firstPage();
  do {
    // Clear only the top half
    display.fillRect(win_x, win_y, win_w, win_h, GxEPD_WHITE);

    // Center the logo within that top half
    int16_t x = (display.width() - logo_width) / 2;
    int16_t y = (win_h - logo_height) / 2;

    display.drawBitmap(
      x,
      y,
      logo_bitmaps,
      logo_width,
      logo_height,
      GxEPD_BLACK
    );
  } while (display.nextPage());
}


void setup() {
  Wire.begin(21, 22); // SDA, SCL (custom pins for ESP32)

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    // Successfully initialized
  } else {
    // Failed to initialize sensor
  }

  display.init(115200, true, 2, false);  // 2ms reset pulse
  showLogo();  // Show the logo once
  delay(2000);
  display.hibernate();
}

void loop() {
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();
    display.init(
  115200,   // serial debug (can be ignored)
  false,    // NOT full refresh (this is key!)
  2,        // reset pulse in ms
  false     // don't use "busy" wait fallback
);                     // Wake up display
    displayLux(lux, 30, 180);           // Show lux value near bottom
    display.hibernate();                // Save power
  }

  delay(updateInterval);  // Wait 30s before next reading
}
