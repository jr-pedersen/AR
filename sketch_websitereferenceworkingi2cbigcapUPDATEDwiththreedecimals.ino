#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

#define analogPin      0          
#define chargePin      13         
#define dischargePin   8        
#define resistorValue  10000.0F

unsigned long startTime;
unsigned long elapsedTime;
float microFarads;                
float nanoFarads;

void setup(){
  pinMode(chargePin, OUTPUT);     
  digitalWrite(chargePin, LOW); 
  lcd.init();
  lcd.backlight();
}

void loop(){
  digitalWrite(chargePin, HIGH);  
  startTime = micros();
  while(analogRead(analogPin) < 648){       
  }

  elapsedTime= micros() - startTime;
  microFarads = ((float)elapsedTime / resistorValue) ;

  if (microFarads > 1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SCALE:  0.1uF-4F");
    lcd.setCursor(0,1);  
    lcd.print(microFarads, 3); // Display with three decimal places
    lcd.setCursor(17,1);        // Adjusted cursor position
    lcd.print("uF");   
    delay(500);    
  }

  else{
    nanoFarads = microFarads * 1000.0; 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SCALE:  0.1uF-4F");
    lcd.setCursor(0,1);       
    lcd.print(nanoFarads, 3);  // Display with three decimal places
    lcd.setCursor(17,1);       // Adjusted cursor position
    lcd.print("nF");          
    delay(500); 
  }

  digitalWrite(chargePin, LOW);            
  pinMode(dischargePin, OUTPUT);            
  digitalWrite(dischargePin, LOW);     // Discharging the capacitor     
  while(analogRead(analogPin) > 0){         
  } // Wait till the capacitor is discharged

  pinMode(dischargePin, INPUT);      // Set the pin to high impedance
  
  lcd.setCursor(0,0);
  lcd.print("DISCHARGING.....");
  lcd.setCursor(0,1);  
}