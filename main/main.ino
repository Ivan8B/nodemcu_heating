#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 4;
const int led = 2;
const int heating = 5;

const int highTemperature = 3; //ºC
const int lowTemperature = -3; //ºC
const int errorTemperature = -100; // when sensor failed temperature is -127ºC
const int pollDelaySeconds = 60 * 6 * 10; // in minutes

int temperatureCounter = 0;
int blinkCounter = 0;
int blinkMode = 0; // 0 - heating disabled, 1 - enabled, -1 - error

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(led, OUTPUT);
  pinMode(heating, OUTPUT);
  checkTemperature();
}

void loop() {
  temperatureCounter++;
  blinkCounter++;

  if (temperatureCounter == pollDelaySeconds) {
    temperatureCounter = 0;
    blinkCounter = 0;
    checkTemperature();
  }

  blink();

  delay(1 * 1000);
}

void checkTemperature() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  if (temperatureC > lowTemperature && temperatureC < highTemperature) {
    enableHeating();
    logEnable(temperatureC);
    blinkMode = 1;
  }
  else {
    if (temperatureC < errorTemperature) {
      disableHeating();
      logError();
      blinkMode = -1;
    }
    else {
      disableHeating();
      logNotEnable(temperatureC);
      blinkMode = 0;
    }
  }
}

void enableHeating() {
  digitalWrite(heating, HIGH);
}

void disableHeating() {
  digitalWrite(heating, LOW);
}


void logNotEnable(float temperatureC) {
  Serial.print("Heating not enabled, current temperature ");
  Serial.print(temperatureC);
  Serial.print("ºC, ");
  Serial.print("current settings - < ");
  Serial.print(highTemperature);
  Serial.print("ºC and > ");
  Serial.print(lowTemperature);
  Serial.println("ºC");
}

void logEnable(float temperatureC) {
  Serial.print("Heating enabled, current temperature ");
  Serial.print(temperatureC);
  Serial.print("ºC, ");
  Serial.print("current settings - < ");
  Serial.print(highTemperature);
  Serial.print("ºC and > ");
  Serial.print(lowTemperature);
  Serial.println("ºC");
}

void logError() {
  Serial.print("Temperature sensor signal loss!!!");
}

void blink() {

  if (blinkMode == 0) {
    if (blinkCounter < 9) {
      digitalWrite(led, HIGH);
    }
    else {
      digitalWrite(led, LOW);
      blinkCounter = 0;
    }
  }

  if (blinkMode == 1) {
    if (blinkCounter < 9) {
      digitalWrite(led, LOW);
    }
    else {
      digitalWrite(led, HIGH);
      blinkCounter = 0;
    }
  }

  if (blinkMode == -1) {
    if (blinkCounter < 2) {
      digitalWrite(led, LOW);
    }
    else {
      digitalWrite(led, HIGH);
      blinkCounter = 0;
    }
  }
}
