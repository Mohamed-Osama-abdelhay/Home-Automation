#include <LiquidCrystal_I2C.h>
//#include <Adafruit_Sensor.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include "addons/TokenHelper.h"
#include <addons/RTDBHelper.h>
#include <Wire.h>

#include "DHT.h"
#include <Keypad.h>
#include <ESP32Servo.h>



#define WIFI_SSID "TEdataE596901"
#define WIFI_PASSWORD "10900587$Mm"


#define DATABASE_URL "loginregister-ea880-default-rtdb.firebaseio.com"
#define DATABASE_SECRET "VgWGlm29kumi8U34JhhtVOcxCS69dWlerU0gyN60"


FirebaseData firebaseData;
FirebaseAuth firebaseAuth;
FirebaseConfig config;


#define DHT_TYPE DHT11
#define inpt1 23
#define inpt2 18
#define enable 4
#define LED_PIN 5
#define DHT_PIN 14
#define TRIG_PIN 12
#define ECHO_PIN 13
#define SERVO_PIN 15
const int motorSpeed = 255;   

#define mq2_pin 2
#define lm35_pin 36
#define buzzer 19

#define ldr_pin 34
#define PIR_pin 35



DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;

// Keypad
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' }
};

byte rowPins[ROWS] = { 32, 33, 25, 26 };
byte colPins[COLS] = { 27, 16, 17 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


String storedPassword = "";   // Password from Firebase
String enteredPassword = "";  // Password entered via keypad
bool isSystemLocked = true;   // Lock flag

float temperature = 0.0;
long duration;
float distance = 0.0;

void setup() {
  Serial.begin(115200);


  pinMode(inpt1, OUTPUT);
  pinMode(inpt2, OUTPUT);
  pinMode(enable, OUTPUT);






  pinMode(LED_PIN, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);

  pinMode(ECHO_PIN, INPUT);





  pinMode(mq2_pin, INPUT);

  pinMode(lm35_pin, INPUT);

  pinMode(ldr_pin, INPUT);

  pinMode(buzzer, OUTPUT);

  pinMode(PIR_pin, INPUT);


  servo.attach(SERVO_PIN);

  servo.write(0);

  Wire.begin(21, 22);

   lcd.begin();

   lcd.backlight();

  dht.begin();


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");


  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &firebaseAuth);


  if (Firebase.RTDB.getString(&firebaseData, "/actions/pass")) {
    storedPassword = firebaseData.stringData();
    Serial.println("Stored Password: " + storedPassword);
  } else {
    Serial.println("Failed to get password from Firebase");
  }
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    if (isSystemLocked) {
      handlePasswordEntry();
    } else {
      runFunctionalities();
      checkForReLock();
    }
  } else {

    Serial.println("Wi-Fi disconnected, turning off components to save power");

    digitalWrite(inpt1, LOW);
    digitalWrite(inpt2, LOW);

    digitalWrite(LED_PIN, LOW);
    servo.write(0);
    

    // Disable DHT and ultrasonic sensor readings
    temperature = 0.0;  // Do not read temperature
    distance = 0.0;     // Do not read distance
  }

  delay(100);
}



void handlePasswordEntry() {
  
  lcd_show_string("Enter password" );

  char key = keypad.getKey();
  if (key) {
    delay(200);
    Serial.println("Key Pressed: " + String(key));
    if (key == '#') {  // Submit password
      Serial.println("Entered Password: " + enteredPassword);
      if (enteredPassword == storedPassword) {
        unlockSystem();
      } else {
        
        lcd_show_string("Wrong Password!");
        Serial.println("Incorrect Password!");
        delay(1000);
        
      }
      enteredPassword = "";   // Reset entered password
    } else if (key == '*') {  // Clear password
      enteredPassword = "";
      
    } else {  // Append digit to password
      enteredPassword += key;
    }
  }
}

void unlockSystem() {
  isSystemLocked = false;
 lcd_show_string("Access Granted");
  Serial.println("Password Correct! Unlocking...");
  servo.write(90);  // Unlock position
  delay(2000);
  servo.write(0);  // Lock position after a moment
  
}

void runFunctionalities() {

  readAndSendTemperature();

  controlFan();

  controlLED();

  measureDistance();

  Read_Temp();

  light_func();

  smoke_alarm();
}



void checkForReLock() {
  char key = keypad.getKey();
  if (key == '*') {  // Press '*' to relock the system
    isSystemLocked = true;

    enteredPassword = "";
    if (Firebase.RTDB.getString(&firebaseData, "/actions/pass")) {
      storedPassword = firebaseData.stringData();
      Serial.println("Stored Password: " + storedPassword);
    } else {
      Serial.println("Failed to get password from Firebase");
    }
    
    lcd_show_string("System Locked");
    Serial.println("System Relocked.");
  }
}

void readAndSendTemperature() {  //function measurment 2l rtoobaa
  float humidity = dht.readHumidity();
  if (!isnan(humidity)) {
    Firebase.RTDB.setFloat(&firebaseData, "/actions/humidity", humidity);
    Serial.println("Humidity now :" + String(humidity));
  }
}

void controlFan() {
  if (Firebase.RTDB.getInt(&firebaseData, "/actions/fan")) {



    int fanState = firebaseData.intData();
    digitalWrite(inpt1, fanState);
    digitalWrite(inpt2, LOW);
    
    analogWrite(enable, motorSpeed);  // Control fan speed (0-255)
    Serial.println(fanState ? "Fan ON" : "Fan OFF");
  }
}

void controlLED() {
  if (Firebase.RTDB.getInt(&firebaseData, "/actions/light")) {
    int lightState = firebaseData.intData();
    digitalWrite(LED_PIN, lightState);
    // servo.write(90);
    Serial.println(lightState ? "LED ON" : "LED OFF");
  }
}

void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.034) / 2;
  if (distance < 5) {
    Firebase.RTDB.setInt(&firebaseData, "/actions/ultra", 1);  // Send 1 if distance is less than 5cm
  } else {
    Firebase.RTDB.setInt(&firebaseData, "/actions/ultra", 0);  // Send 0 if distance is more than 5cm
  }
  Serial.println("Distance: " + String(distance) + " cm");
}




//light
void light_func() {

  bool motion = motion_detection();

  int light_value = analogRead(ldr_pin);

  lcd_show(light_value, "light now:");

  if (motion == true) {

    //light
    if (analogRead(ldr_pin) >= 35 /* light value */) {

      //turn off lights
      digitalWrite(LED_PIN, LOW);


    } else if (analogRead(ldr_pin) < 35 /* light value */) {
      //turn on lights
      digitalWrite(LED_PIN, HIGH);
    }
  }
}



//gas check

void smoke_alarm() {

  const int smoke_trigger = 500;  //gas value change 7sb 2l ra8ba

  int smoke_reading = analogRead(mq2_pin);

  lcd_show(smoke_reading, "smoke value now:");

  delay(2000);

  if (smoke_reading >= smoke_trigger) {


    Serial.println("danger there is gas leak ");

    Firebase.RTDB.setString(&firebaseData, "/alerts/Smoke", "Smoke Detected Alert!");

    digitalWrite(buzzer, HIGH);
    delay(2000);
    digitalWrite(buzzer, LOW);
    delay(2000);

  } else {

    //safe
    lcd_show_string("2l wad3 safe");
  }
}


//lcd functions for printing
void lcd_show(int sensor_reading, String show) {

  lcd.setCursor(0, 0);
  lcd.print("                ");  // Clear 16 characters


  lcd.setCursor(0, 0);

  lcd.print(show);

  lcd.setCursor(0, 1); 
  lcd.print("                ");  // Clear the second row
  
  lcd.setCursor(0, 1);  
  lcd.print(sensor_reading , 1);  
  
  delay(1000); 
}


void lcd_show_string(String show) {

  lcd.setCursor(0, 0);
  lcd.print("                ");  // Clear 16 characters
  lcd.setCursor(0, 0);
  lcd.print(show);
  delay(1000);
}


//motion
bool motion_detection() {

  int pir_reading;
  pir_reading = digitalRead(PIR_pin);

  if (pir_reading == 1) {


    return true;

  } else {

    return false;
  }
}





void Read_Temp() {

  float temp_ = (analogRead(lm35_pin) * 5.0 / 1024.0) * 100.0;


  lcd_show((int)temp_, "the temp now is :");
  delay(1000);


  Firebase.RTDB.setFloat(&firebaseData, "/actions/temp", temp_);


  float threshold = 35;

  bool motion = motion_detection();

  if (motion == true) {


    if (temp_ >= threshold) {

      //turn on fan

      digitalWrite(inpt1, HIGH);
      digitalWrite(inpt2, LOW);

      // Set the fan speed using PWM
      analogWrite(enable, motorSpeed);  // Control fan speed (0-255)

    } else {
      //turn off fan

      digitalWrite(inpt1, LOW);
      digitalWrite(inpt2, LOW);
    }
  }
}
