#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>
#include <Adafruit_Sensor.h>
#include <Math.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_RESET 8 //res 10
#define OLED_DC    9 //dc 8 D9
#define OLED_CS    10 //cs 9 D10
#define OLED_MOSI   11 //sck 6 D13
#define OLED_CLK   13 //sda 7 D12

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

MPU6050 mpu(Wire);

long timer = 0;

void setup() {

  Serial.begin(115200);
  Wire.begin();

  byte status = mpu.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println("SSD1306 Found!");

    Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050

  Serial.println("MPU6050 Found!");

  // Clear the buffer
  display.clearDisplay();

  // Set text color to white
  display.setTextColor(SSD1306_WHITE);
  
  // Set text size (1 = 6x8 font, 2 = 12x16 font, etc.)
  display.setTextSize(1);

  // Set cursor position to (0, 0)
  display.setCursor(0, 0);

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  display.println(F("Calculating offsets, do not move MPU6050"));
  display.display();
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");
  display.println(F("Done!"));
  display.display();
  delay(200);

}

void loop() {
  
  // Clear the buffer
  display.clearDisplay();
  display.setCursor(0, 0);

  updateSensors();

  demo();
  drawHorizonOnScreen();


  display.display();

  delay(10);

}


// Function to convert degrees to radians
float toRadians(float degrees) {
  return degrees * PI / 180.0;
}

void drawHorizonOnScreen() {

  float x = mpu.getAngleZ(); // x and z are reversed due to the orientation of the sensor.
  float y = mpu.getAngleY();
  float z = mpu.getAngleX(); // x and z are reversed due to the orientation of the sensor.

  // Convert angles to radians
  float radX = toRadians(x);
  float radY = toRadians(y);
  float radZ = toRadians(z);

  // Calculate the distance from the center to the edge of the screen
  float screenRadius = min(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.0;

  // Calculate the maximum line length based on the distance to the screen edge
  float maxLineLength = screenRadius / cos(radY);

  // Calculate the endpoint of the line on the positive side
  float endXPos = maxLineLength * cos(radY) * cos(radX);
  float endYPos = maxLineLength * cos(radY) * sin(radX);

  // Calculate the endpoint of the line on the negative side
  float endXNeg = maxLineLength * cos(radY + PI) * cos(radX);
  float endYNeg = maxLineLength * cos(radY + PI) * sin(radX);

  // Correct endpoint coordinates if angle is greater than 90 degrees or less than -90 degrees
  if (y > 90 || y < -90) {
    endXPos *= -1;
    endXNeg *= -1;
  }

  // Map endpoint coordinates to screen coordinates
  int screenXPos = (SCREEN_WIDTH / 2) + (endXPos * (SCREEN_WIDTH / 2) / screenRadius);
  int screenYPos = (SCREEN_HEIGHT / 2) - (endYPos * (SCREEN_HEIGHT / 2) / screenRadius);

  int screenXNeg = (SCREEN_WIDTH / 2) + (endXNeg * (SCREEN_WIDTH / 2) / screenRadius);
  int screenYNeg = (SCREEN_HEIGHT / 2) - (endYNeg * (SCREEN_HEIGHT / 2) / screenRadius);

  // Offset the Y coordinate based on the Y angle
  int yOffset = (y * (SCREEN_HEIGHT / 2) / 90) * -1;

  // Draw lines on both sides with offset
  display.drawLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + yOffset, screenXPos, screenYPos + yOffset, SSD1306_WHITE);
  display.drawLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + yOffset, screenXNeg, screenYNeg + yOffset, SSD1306_WHITE);
}

void updateSensors() {
  mpu.update();
}

void demo() {
  if(millis() - timer > 10){

    display.println(F("ANGLE"));
    display.print(F("X: "));display.println(mpu.getAngleX());
    display.print("Y: ");display.println(mpu.getAngleY());
    display.print("Z: ");display.println(mpu.getAngleZ());

    Serial.print(F("X:"));Serial.print(mpu.getAngleX());
    Serial.print(",Y:");Serial.print(mpu.getAngleY());
    Serial.print(",Z:");Serial.println(mpu.getAngleZ());

    //Serial.println(F("=====================================================\n"));
    timer = millis();
  }
}



