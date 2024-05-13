#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ADC.h> // Include the ADC library

ADC adc; // Create an instance of the ADC object

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define analogPin      A0         // A0 pin on Teensy corresponds to analog pin 0
#define chargePin      13
#define dischargePin   8
#define resistorValue  10030.0F
#define threshold      2588        // Adjust threshold for 10uF capacitor

float microFarads;
float nanoFarads;

void setup() {
  pinMode(chargePin, OUTPUT);
  digitalWrite(chargePin, LOW);
  lcd.init();
  lcd.backlight();

  adc.adc0->setAveraging(4); // Set the number of averages
  adc.adc0->setResolution(12); // Set ADC resolution to 12-bit
  adc.adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED); // Set the conversion speed
}

void loop() {
  while (true) {
    digitalWrite(chargePin, HIGH);
    unsigned long startTime = micros();
    while (adc.analogRead(analogPin) < threshold) {
      // Wait until the analog reading exceeds the threshold
    }

    float elapsedTime = (micros() - startTime);
    microFarads = (elapsedTime / resistorValue);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SCALE:  0.1uF-4F");

    if (microFarads > 1) {
      lcd.setCursor(0, 1);
      lcd.print(microFarads, 3); // Display with three decimal places
      lcd.print(" uF");
    } else {
      float nanoFarads = microFarads * 1000.0;
      lcd.setCursor(0, 1);
      lcd.print(nanoFarads, 3);  // Display with three decimal places
      lcd.print(" nF");
    }

    digitalWrite(chargePin, LOW);
    pinMode(dischargePin, OUTPUT);
    digitalWrite(dischargePin, LOW);     // Discharging the capacitor
    while (adc.analogRead(analogPin) > 0) {
      // Wait until the capacitor is fully discharged
    }
    pinMode(dischargePin, INPUT);      // Set the pin to high impedance

    lcd.setCursor(0, 0);
    lcd.print("DISCHARGING.....");
  }
}