
#include <LiquidCrystal.h>
#include "ClickButton.h"
#include "Timer.h"  
#include <avr/eeprom.h>


#define PIN_BUTON 8 //pin buton
#define PIN_RELEU 13 //pin releu
#define INTERVAL 5//intervalul de crestere
#define IMAX 60//intervalul maxim
#define DECLANS 2000//din cat in cat sa declanseze. pentru 1min=6000
#define ADRESA_EEPROM  0//adresa la care salveaza in eeprom

ClickButton button1(PIN_BUTON, HIGH, CLICKBTN_PULLUP);

int stareButon=0; 

Timer t;   
int tickEvent;
uint32_t index;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int timp=0;
boolean ruleaza;

void setup() {
  pinMode(PIN_RELEU,OUTPUT);
  digitalWrite(PIN_RELEU,LOW);
  index= eeprom_read_dword((const uint32_t *)ADRESA_EEPROM);
  lcd.begin(16, 2);
  lcd.print("Timp setat:");
  lcd.setCursor(11, 0);
  lcd.print(timp);
  lcd.setCursor(0, 1); 
  lcd.print("Index:");   
  lcd.setCursor(6, 1); 
  lcd.print(index);
  
  button1.debounceTime   = 10;   // Debounce timer in ms
  button1.multiclickTime = 50;  // Time limit for multi clicks
  button1.longClickTime  = 1000; // time until "held-down clicks" register
  

    
  Serial.begin(9600);
}

void loop() {
  t.update();
  button1.Update();

  if (button1.clicks != 0) stareButon = button1.clicks;
  
  //apasare scurta b1
  if(stareButon == 1 && ruleaza==false) {
  lcd.clear();  
  lcd.setCursor(0, 0); 
  lcd.print("Timp setat:");   
  timp=timp+INTERVAL;
  lcd.setCursor(11, 0);
  lcd.print(timp);
  if (timp>IMAX) {
  timp=0;
  lcd.setCursor(11, 0);
  lcd.print("0 ");
  }
    lcd.setCursor(0, 1); 
  lcd.print("Index:");   
  lcd.setCursor(6, 1); 
  lcd.print(index);
  stareButon=0;
  }

 
  //apasare lunga
  if(stareButon == -1) {
    if(timp>0){
  index=index+timp;
  eeprom_write_dword((uint32_t *)ADRESA_EEPROM, index);
  tickEvent = t.every(DECLANS, doSomething); 
  digitalWrite(PIN_RELEU,HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Timp ramas:");
  stareButon = 0;
  ruleaza=true;}
}    
}

void doSomething()
{
timp=timp-1;
if (timp<10){
lcd.setCursor(12, 0);
lcd.print(" ");}
lcd.setCursor(11, 0);
lcd.print(timp);

if (timp==0){
t.stop(tickEvent);
ruleaza=false;
index= eeprom_read_dword((const uint32_t *)ADRESA_EEPROM);
digitalWrite(PIN_RELEU,LOW);
lcd.setCursor(0, 0);
lcd.print("Timp setat:");
lcd.setCursor(11, 0);
lcd.print(timp);
lcd.setCursor(0, 1); 
lcd.print("Index:");   
lcd.setCursor(6, 1); 
lcd.print(index);
}
}
