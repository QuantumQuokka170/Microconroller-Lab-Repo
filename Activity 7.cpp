#include <Wire.h> //I2C Communication
#include <Adafruit_GFX.h> //Graphics library for OLED Display
#include <Adafruit_SSD1306.h> //OLED Display library
#include <MPU6050_tockn.h> //MPU6050 library
#include <Adafruit_AHTX0.h> //AHT10 library
#define SCREEN_WIDTH 128    //OLED Display width
#define SCREEN_HEIGHT 64    //OLED Display height
#define OLED_RESET -1      //OLED Display reset pin
#define SCREEN_ADDRESS 0x3C  //OLED Display address


const int trigPin = 3; //Ultrasonic sensor trigger pin
const int echoPin = 4; //Ultrasonic sensor echo pin
const int lightPin= 5; //LED pin
const int buttonPin = 15; //Button pin

volatile bool ledState = false; //LED state
volatile int count = 0; //Button press count

//Initializing sensor and display objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //OLED Display object
MPU6050 mpu6050(Wire); //MPU6050 object
Adafruit_AHTX0 aht10; //AHT10 object

//Toggle LED function to toggle the LED state and increment the button press count
void toggleLED(){ 
  ledState = !ledState; //Toggle the LED state
  digitalWrite(lightPin, ledState); //Set the LED state
  count++; //Increment the button press count
}

//Setup function to initialize the sensors and display
void setup() {
    Serial.begin(115200); //Initialize serial communication
    Serial.begin(115200); //Initialize serial communication

    pinMode(trigPin, OUTPUT); //Set the trigger pin as output
    pinMode(lightPin, OUTPUT); //Set the LED pin as output
    pinMode(echoPin, INPUT); //Set the echo pin as input
    pinMode(buttonPin, INPUT_PULLUP); //Set the button pin as input with pull-up resistor
    pinMode(lightPin, OUTPUT); //Set the LED pin as output
    attachInterrupt(digitalPinToInterrupt(buttonPin), toggleLED, FALLING); //Attach the toggleLED function to the button pin interrupt
 
    Wire.setSDA(20); //Set the SDA pin
    Wire.setSCL(21); //Set the SCL pin
    Wire.begin(); //Initialize the I2C bus
 

    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { //Check if the OLED Display is found
        Serial.println("SSD1303 not found!"); //Print an error message if the OLED Display is not found
        while (true);
    }
 
    display.clearDisplay(); //Clear the display
    display.setTextSize(1); //Set the text size to 1
    display.setTextColor(SSD1306_WHITE); //Set the text color to white
    display.setCursor(10, 10); //Set the cursor position to the center of the display
    display.print("Initializing..."); //Print the initializing message  
    display.display(); //Display the message
 

    mpu6050.begin(); //Initialize the MPU6050
    mpu6050.calcGyroOffsets(true); //Calculate the gyroscope offsets
 
    if (!aht10.begin()) { //Check if the AHT10 is found
        Serial.println("AHT10 sensor not found!"); //Print an error message if the AHT10 is not found
        while (true);
    }
 
    delay(1000); //Delay for 1 second
    display.clearDisplay(); //Clear the display
}
 
void loop() { //Main loop
    mpu6050.update(); //Update the MPU6050
 
    float gyroX = mpu6050.getGyroX(); //Get the gyroscope X axis value
    float gyroY = mpu6050.getGyroY(); //Get the gyroscope Y axis value
    float gyroZ = mpu6050.getGyroZ(); //Get the gyroscope Z axis value

    sensors_event_t humidity, temp; //Get the humidity and temperature values
    aht10.getEvent(&humidity, &temp); //Get the humidity and temperature values

    Serial.print("Gyro X: "); Serial.print(gyroX); //Print the gyroscope X axis value
    Serial.print(" | Gyro Y: "); Serial.print(gyroY); //Print the gyroscope Y axis value
    Serial.print(" | Gyro Z: "); Serial.print(gyroZ); //Print the gyroscope Z axis value
    Serial.print(" || Temp: "); Serial.print(temp.temperature); //Print the temperature value
    Serial.print(" C | Humidity: "); Serial.print(humidity.relative_humidity); //Print the humidity value
    Serial.println(" %"); //Print the humidity value
 
    display.clearDisplay(); //Clear the display
    display.setCursor(0, 0); //Set the cursor position 
    display.print("Gyro X: "); display.print(gyroX); //Print the gyroscope X axis value
    display.setCursor(0, 10); //Set the cursor position 
    display.print("Gyro Y: "); display.print(gyroY); //Print the gyroscope Y axis value
    display.setCursor(0, 20); //Set the cursor position 
    display.print("Gyro Z: "); display.print(gyroZ); //Print the gyroscope Z axis value
    display.setCursor(0, 35); //Set the cursor position 
    display.print("Temp: "); display.print(temp.temperature); display.print("C");
    display.setCursor(0, 45); //Set the cursor position 
    display.print("Humidity: "); display.print(humidity.relative_humidity); //Print the humidity value
display.print(" %"); //Print the humidity value
    display.display(); //Display the message
 
 digitalWrite(trigPin, LOW); //Set the trigger pin low
    delayMicroseconds(2); //Delay for 2 microseconds
    digitalWrite(trigPin, HIGH); //Set the trigger pin high
    delayMicroseconds(10); //Delay for 10 microseconds
    digitalWrite(trigPin, LOW); //Set the trigger pin low
     

    long duration = pulseIn(echoPin, HIGH); //Measure the duration of the pulse
    float distance = (duration * 0.0343) / 2; //Calculate the distance in cm
     
    Serial.print("Distance: "); //Print the distance value
    Serial.print(distance); //Print the distance value
    Serial.println(" cm"); //Print the distance value

    digitalWrite(lightPin, HIGH); //Set the LED pin high
    delay(500); //Delay for 500 milliseconds
    digitalWrite(lightPin, HIGH); //Set the LED pin high

    Serial.print("Button Press Count: "); //Print the button press count
    Serial.println(count); //Print the button press count
    delay(500); //Delay for 500 milliseconds
}

