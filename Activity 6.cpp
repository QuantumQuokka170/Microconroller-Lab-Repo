#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_tockn.h>
#include <Adafruit_AHTX0.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MPU6050 mpu6050(Wire);
Adafruit_AHTX0 aht10;
void setup() {
    Serial.begin(115200);
    Wire.setSDA(20);
    Wire.setSCL(21);
    Wire.begin();

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("SSD1303 not found!");
        while (true);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    display.print("Initializing...");
    display.display();

    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);

    if (!aht10.begin()) {
        Serial.println("AHT10 not found!");
        while (true);
    }
    
    delay(1000);
    display.clearDisplay();
}

void loop() {
    mpu6050.update();

    float gyroX = mpu6050.getGyroX();
    float gyroY = mpu6050.getGyroY();
    float gyroZ = mpu6050.getGyroZ();

    sensors_event_t humidity, temp;
    aht10.getEvent(&humidity, &temp);

    Serial.print("Gyro X: "); Serial.print(gyroX);
    Serial.print(" | Gyro Y: "); Serial.print(gyroY);
    Serial.print(" | Gyro Z: "); Serial.print(gyroZ);
    Serial.print(" || Temp: "); Serial.print(temp.temperature);
    Serial.print(" | Humidity: "); Serial.println(humidity.relative_humidity);
    Serial.println(" %");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Gyro X: "); display.print(gyroX);
    display.setCursor(0, 10);
    display.print("Gyro Y: "); display.print(gyroY);
    display.setCursor(0, 20);
    display.print("Gyro Z: "); display.print(gyroZ);
    display.setCursor(0, 35);
    display.print("Temp: "); display.print(temp.temperature); display.print("C");
    display.setCursor(0, 45);
    display.print("Humidity: "); display.println(humidity.relative_humidity); display.print("%");
    display.display();

    delay(500);
}
