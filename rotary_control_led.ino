#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Encoder.h>

const int PWM_CHANNEL = 0;
const int PWM_FREQUENCY = 5000;
const int PWM_RESOLUTION = 8;
const int LED_PIN = 2;

const char str[] = "LED intensity: ";
char valueStr[25]; 

const char strB[] = "Interval (ms): ";
char valueStrB[25]; 



unsigned long blinkInterval = 1000;
unsigned long lastBlinkTime = 0;
unsigned long maxblinkInterval = 10000;
unsigned long ledPower;
bool ledState = false;


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };


// SSD1306 Display Pins
#define PIN_SDA 18
#define PIN_SCL 19

// Rotary Encoder Pins
#define PIN_ENC_A 16
#define PIN_ENC_B 17
#define PIN_ENC_BTN 5

// Variables
int variable1 = 0;
int variable2 = 0;
int currentVariable = 1;    // 1 = variable1, 2 = variable2
const int MAX_VARIABLE1 = 255; // Maximum value of variable1
const int MAX_VARIABLE2 = 10000; // Maximum value of variable2

// Rotary Encoder Object
ESP32Encoder encoder;

// Button State Variables
int buttonState = HIGH;         // current state of the button
int lastButtonState = HIGH;     // previous state of the button
unsigned long lastDebounceTime = 0;  // last time the button was toggled
unsigned long debounceDelay = 50;   // debounce time; increase if the output flickers



void setup() {
  Serial.begin(115200);

  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  pinMode(PIN_ENC_BTN, INPUT_PULLUP);
  
  pinMode(LED_PIN, OUTPUT);

  ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttachPin(LED_PIN, PWM_CHANNEL);

  encoder.attachHalfQuad(PIN_ENC_A, PIN_ENC_B);

  Wire.begin(PIN_SDA, PIN_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
   

 testscrolltext();    // Draw scrolling text

    display.display();
  delay(2000);

  display.clearDisplay();

}

void loop() {
  // Read the rotary encoder and update the variables
  int newPosition = encoder.getCount();
  static int oldPosition = newPosition;
  int delta = oldPosition - newPosition;
  oldPosition = newPosition;
 
     

  if (delta != 0) {
    if (currentVariable == 1) {
      variable1 = min(max(variable1 + delta, 0), MAX_VARIABLE1);
      Serial.print("Variable 1: ");
      Serial.println(variable1);
      ledcWrite(PWM_CHANNEL, variable1);
      ledPower = (float(variable1)/float(MAX_VARIABLE1))*100;
      Serial.print("led: ");
      Serial.println(ledPower);
      

   
      
      
    } else {
      variable2 = min(max(variable2 + delta, 0), MAX_VARIABLE2);
      Serial.print("Variable 2: ");
      Serial.println(variable2);
      //blinkInterval = map(variable2, 0, 100, 100, 10000);
      blinkInterval = map(variable2, 0,90, 100, 1000);
      //Reset the last blink time
      lastBlinkTime = millis();

      

      // Print the new blink interval
      Serial.print("Blink interval: ");
      Serial.println(blinkInterval);

      
    }
  }

  // Read the state of the button and debounce it
  int reading = digitalRead(PIN_ENC_BTN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        if (currentVariable == 1) {
          currentVariable = 2;
          Serial.println("Switched to Variable 2");
        } else {
          currentVariable = 1;
          Serial.println("Switched to Variable 1");
        }
      }
    }
  }

snprintf(valueStrB, 10, "%d", blinkInterval);
snprintf(valueStr, 10, "%d", ledPower);

char result[20]; // Assumes that the concatenated string will be no longer than 20 characters
strcpy(result, str);
strcat(result, valueStr);

char resultB[20]; // Assumes that the concatenated string will be no longer than 20 characters
strcpy(resultB, strB);
strcat(resultB, valueStrB);
  
  display.clearDisplay();

  display.setTextSize(1.5);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);  
  //display.write(variable1);
  //display.println(variable1);

display.println(result); 
display.println(resultB);   
  
  display.display();

  if ((millis() - lastBlinkTime) > blinkInterval) {
    ledState = !ledState;
    
    ledcWrite(PWM_CHANNEL, ledState ? variable1 : 0);
    lastBlinkTime = millis();
  }

  lastButtonState = reading;

  

 
}

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}


void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(1.5); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("LED controller"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(5000);
  display.clearDisplay();

  }
