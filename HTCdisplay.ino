//Amrit Pandher
//CS351 Assignment 6
//Monitoring and Alarm System

#include <dht.h>
#include <LiquidCrystal.h>
#include <IRremote.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //assign pins for lcd
dht DHT; //create dht object

int h=0; //hours
int m=0; //minutes
int s=0; //seconds
int buzz=0; //buzzer LOW

const int RECV_PIN = 7; //ir reciever pin
#define DHT11_PIN 6 //temperature % humidty sensor pin
IRrecv irrecv(RECV_PIN);
int buzzer = 13; //buzzer pin

void setup(){
  lcd.begin(16, 2); //start lcd display
  irrecv.enableIRIn(); //enable reciever
  irrecv.blink13(true); //blinks on use (helps with identifying key press)
  pinMode(buzzer,OUTPUT); //assign buzzer to pin mode with an output
}

void loop(){
  int chk = DHT.read11(DHT11_PIN); //get dht reading
  lcd.setCursor(0,1); //start at bottom row first column
  //TEMP
  lcd.print("T=");
  lcd.print(DHT.temperature); //print temperature reading
  lcd.print((char)223); //degree symbol
  lcd.setCursor(8,1); //set to 9th column bottom row
  //HUMIDITY
  lcd.print("H=");
  lcd.print(DHT.humidity); //print humidity reading
  lcd.print("%");

  if(DHT.humidity >= 85){ //if humidity goes over 85% set output parameter to HIGH
    buzz = 1;
  }
  digitalWrite(buzzer,buzz); //pass output parameter into buzzer
  
  //TIME (didn't have real time clock module)
  if(buzz==0){ //if buzzer is not going continue with clock
    if(s >= 60){ //handles seconds
      s = 0;
      m += 1;
    }
    if(m >= 60){ //handles minutes
      m = 0;
      h += 1;
    }
    if(h >= 24){ //handles hours
      h = 0;
      s = 0;
      m = 0;
    }
    lcd.setCursor(0,0); //set cursor to first row first column
    if(h<10) //handles leading 0
    lcd.print(0);
    lcd.print(h); //print hour
    lcd.setCursor(2,0); //move over 2 spaces
    lcd.print(":");
    lcd.setCursor(3,0);
    if(m<10) //handles leading 0
    lcd.print(0);
    lcd.print(m); //print minute
    lcd.setCursor(5,0); //move over 2 spaces
    lcd.print(":");
    lcd.setCursor(6,0);
    if(s<10) //handles leading 0
    lcd.print(0);
    lcd.print(s); //print second
    s += 2; //since delay is 2000 seconds will increase by 2 everytime
  }
  if(irrecv.decode()){ //if there is a value to decode check that raw data value for command
    if(irrecv.decodedIRData.decodedRawData == 4077715200){ //button 1
    h += 5;
    }
    if(irrecv.decodedIRData.decodedRawData == 3877175040){ //button 2
    m += 5;
    }
    if(irrecv.decodedIRData.decodedRawData == 2707357440){ //button 3
    s += 5;
    }
    if(irrecv.decodedIRData.decodedRawData == 4144561920){ //button 4
    h -= 1;
    }
    if(irrecv.decodedIRData.decodedRawData == 3810328320){ //button 5
    m -= 1;
    }
    if(irrecv.decodedIRData.decodedRawData == 2774204160){ //button 6
    s -= 1;
    }
  }
  
  irrecv.resume(); //resume for new button click
  delay(2000); //delay 2 seconds
}
