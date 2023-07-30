#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   11 //sck 6
#define OLED_CLK   13 //sda 7
#define OLED_DC    9 //dc 8
#define OLED_CS    10 //cs 9
#define OLED_RESET 8 //res 10
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup() {

  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Set text color to white
  display.setTextColor(SSD1306_WHITE);
  
  // Set text size (1 = 6x8 font, 2 = 12x16 font, etc.)
  display.setTextSize(1);

  // Set cursor position to (0, 0)
  display.setCursor(0, 0);

  // Print "Hello World" to the display
  display.println(F("Hello World"));
  Serial.println(F("Hello World"));

  // Display the content on the screen
  display.display();
}

void loop() {
  // Nothing to do here for this example
}
