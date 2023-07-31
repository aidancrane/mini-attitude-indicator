#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Math.h>

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


int COUNT = 0; //res 10

float ROLL = 0;
float PITCH = 0;
float YAW = 0;

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
  display.println(F("Hello World 123"));
  Serial.println(F("Hello World"));

}

void loop() {
  
  // Clear the buffer
  display.clearDisplay();

   // Generate random Euler angles and display them on the screen
  generateRandomEulerAngles();

  calculatePitch();

  displayXYZ();

  // Display the content on the screen
  display.display();

  delay(1000);

}

void displayXYZ() {

  display.setCursor(0, 0);
  // Print the Euler angles to the display
  display.print(F("Roll: "));
  display.println(ROLL);

  display.print(F("Pitch: "));
  display.println(PITCH);

  display.print(F("Yaw: "));
  display.println(YAW);

  display.print(F("Count: "));
  display.println(COUNT);
}

void calculatePitch() {

  float roll  = (-PITCH * M_PI) / 180.0;
  float cosroll = cos(roll);

  Serial.println(roll);

  display.drawLine(0, roll, SCREEN_WIDTH, roll, SSD1306_WHITE);

}


void generateRandomEulerAngles() {
  ROLL = random(-90, 91);  // Generate random roll angle between -90 and 90 degrees
  PITCH = random(-90, 91); // Generate random pitch angle between -90 and 90 degrees
  YAW = random(0, 361);    // Generate random yaw angle between 0 and 360 degrees
  COUNT++;

}



