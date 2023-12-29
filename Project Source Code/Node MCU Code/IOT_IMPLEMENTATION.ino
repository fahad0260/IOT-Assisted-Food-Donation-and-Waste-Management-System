#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "max6675.h"

int thermoDO = D7;
int thermoCS = D6;
int thermoCLK = D5;
int ledPin = D4;
int gasPin = A0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
float gasLevel = 0.00;
float celsius = 0.00;
float fahrenheit = 0.00;

#define i2c_Address 0x3c
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 1
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void checkFoodSafety();

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  celsius = thermocouple.readCelsius();
  fahrenheit = thermocouple.readFahrenheit();
  gasLevel = analogRead(gasPin) / 1023.0 * 100.0;

  Serial.print("Temperature (C): ");
  Serial.println(celsius);  // Display temperature in Celsius on Serial monitor
  Serial.print("Temperature (F): ");
  Serial.println(fahrenheit);
  Serial.print("Gas Level: ");
  Serial.println(gasLevel);

  checkFoodSafety();

  delay(1000); // Adjust delay as needed
}

void checkFoodSafety() {
  Serial.println("Checking food safety...");
  
  int expectedGasValue = 5;
  double expectedMinTemperature = 30.0;
  double expectedMaxTemperature = 60.0;

  display.clearDisplay();

  if (gasLevel >= expectedGasValue || celsius < expectedMinTemperature || celsius > expectedMaxTemperature) {
    Serial.println("Food is not safe!");
    digitalWrite(ledPin, HIGH);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.println("Food is not safe!");
    display.setCursor(0, 10);
    display.println("Inedible");
  } 
  else {
    Serial.println("Food is safe!");
    digitalWrite(ledPin, LOW);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.println("Food is safe!");
    display.setCursor(0, 10);
    display.println("Edible");
  }

  if (celsius > 100.0) {
    Serial.println("Temperature is out of range!");
    digitalWrite(ledPin, HIGH);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 20);
    display.println("Temperature out of range!");
  }

  display.display();
  delay(2000);
  display.clearDisplay();
}
