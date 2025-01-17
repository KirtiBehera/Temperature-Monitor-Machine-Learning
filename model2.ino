#include <Wire.h>
#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define NUM_SAMPLES 21

LiquidCrystal lcd(12, 11, 4, 5, 6, 7); // LCD pins: RS, EN, D4, D5, D6, D7
DHT dht(DHTPIN, DHTTYPE);

float temperatures[NUM_SAMPLES] = {20.5, 21.0, 22.3, 23.1, 24.5, 25.0, 25.8, 26.2, 27.0, 27.5, 30.4, 35.5, 40.0, 41.5, 42.8, 43.3, 44.7, 45.5, 47.1, 50.6, 51.0};
unsigned long timestamps[NUM_SAMPLES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4); // Initialize LCD
  dht.begin(); // Initialize DHT sensor
}

void loop() {
  float currentTemperature = readTemperature();
  float predictedTemperature = predictTemperature(currentTemperature);
  
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("Current Temp:");
  lcd.setCursor(0, 4);
  lcd.print(currentTemperature);
  lcd.setCursor(0, 0);
  lcd.print("Predicted Temp:");
  lcd.setCursor(0, 1);
  lcd.print(predictedTemperature);
  
  delay(2000); // Delay for display
}

float readTemperature() {
  delay(2000); // Delay to stabilize sensor
  float temperature = dht.readTemperature(); // Read temperature from DHT sensor
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return -1; // Error value
  }
  return temperature;
}

float predictTemperature(float currentTemperature) {
  float closestTemperature = temperatures[0];
  float minDifference = abs(currentTemperature - closestTemperature);
  
  for (int i = 1; i < NUM_SAMPLES; i++) {
    float difference = abs(currentTemperature - temperatures[i]);
    if (difference < minDifference) {
      minDifference = difference;
      closestTemperature = temperatures[i];
    }
  }
  
  return closestTemperature;
}
