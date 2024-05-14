#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions for Teensy 4.1
const int OUT_PIN = 16; // Analog pin A2 on Teensy 4.1
const int IN_PIN = 14; // Analog pin A0 on Teensy 4.1

LiquidCrystal_I2C lcd(0x27, 20, 4); // LCD address and dimensions

const float IN_STRAY_CAP_TO_GND = 12.5; //24.48 stock //TUNED VIA 20PF CAP. May need to adjust. No internal 
const float IN_CAP_TO_GND = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 34.8; //stock: 34.8 
const int MAX_ADC_VALUE = 1023; // Teensy 4.1 ADC max value 4095. 1023 at 10 bits 

void setup()
{
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
}

void loop()
{
  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, HIGH);
  int val = analogRead(IN_PIN);
  digitalWrite(OUT_PIN, LOW);

  if (val < 1000)
  {
    pinMode(IN_PIN, OUTPUT);

    float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);

    lcd.setCursor(0, 0);
    lcd.print("Scale:   1pF-1nF");
    lcd.setCursor(0, 1);
    lcd.print(capacitance, 3);
    lcd.setCursor(14, 1);
    lcd.print("pF");
  }

  else
  {
    pinMode(IN_PIN, OUTPUT);
    delay(1);
    pinMode(OUT_PIN, INPUT_PULLUP);
    unsigned long u1 = micros();
    unsigned long t;
    int digVal;

    do
    {
      digVal = digitalRead(OUT_PIN);
      unsigned long u2 = micros();
      t = u2 > u1 ? u2 - u1 : u1 - u2;
    } while ((digVal < 1) && (t < 400000UL)); // Ensure correct type for UL

    pinMode(OUT_PIN, INPUT);
    val = analogRead(OUT_PIN);
    digitalWrite(IN_PIN, HIGH);
    int dischargeTime = (int)(t / 1000UL) * 5; // Ensure correct type for UL
    delay(dischargeTime);
    pinMode(OUT_PIN, OUTPUT);
    digitalWrite(OUT_PIN, LOW);
    digitalWrite(IN_PIN, LOW);

    float capacitance = -(float)t / R_PULLUP / log(1.0 - (float)val / (float)MAX_ADC_VALUE);

    lcd.setCursor(0, 0);
    lcd.print("Scale:   1pF-1nF");

    if (capacitance > 1000.0)
    {
      lcd.setCursor(0, 1);
      lcd.print(capacitance / 1000.0, 2);
      lcd.setCursor(14, 1);
      lcd.print("uF ");
    }

    else
    {
      lcd.setCursor(0, 1);
      lcd.print(capacitance);
      lcd.setCursor(14, 1);
      lcd.print("nF");
    }
  }
  while (micros() % 1000 != 0);
}