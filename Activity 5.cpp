const int trigPin = 3;
const int echoPin = 4;
const int lightPin = 5;

const int buttonPin = 15;
volatile bool ledState = false;
volatile int count = 0;

void toggleLED() {
    ledState = !ledState; 
    digitalWrite(lightPin, ledState); 
    count++;
}

void setup() {
    serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(lightPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(lightPin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(buttonPin), toggleLED, FALLING);
}

void loop() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    digitalWrite(lightPin, HIGH);
    delay(500);
    digitalWrite(lightPin, HIGH);

    Serial.print("Button Press Count: ")
    Serial.println(count);
}