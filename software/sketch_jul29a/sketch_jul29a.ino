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

  demo();

  display.display();

  delay(10);

}


void demo() {
  mpu.update();


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



