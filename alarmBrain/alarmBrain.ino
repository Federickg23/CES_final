
/**********************************************************************
  Filename    : Drive LiquidCrystal I2C to display characters
  Description : I2C is used to control the display characters of LCD1602.
  Auther      : www.freenove.com
  Modification: 2020/07/11
**********************************************************************/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include "pitches.h"
#define  VARIANT_MCK 84000000
#define TONEpin    15
/*
 * Remote keys: 
FFA25D     power
FFE21D     menu
FF22DD     test
FF02FD     +
FFC23D     back
FFE01F     reverse
FFA857     play
FF906F     forward
FF6897     0
FF9867     -
FFB04F     C
FF30CF     1
FF18E7     2
FF7A85     3
FF10EF     4
FF38C7     5
FF5AA5     6
FF42BD     7
FF4AB5     8
FF52AD     9
 * 
 */

#define PIN_BUZZER 4

const uint16_t recvPin = 15; // Infrared receiving pin
IRrecv irrecv(recvPin);      // Create a class object used to receive class
decode_results results;       // Create a decoding results class object

#define SDA 13                    //Define SDA pins
#define SCL 14                    //Define SCL pins

bool on = false;

//86400 

/*
 * note:If lcd1602 uses PCF8574T, IIC's address is 0x27,
 *      or lcd1602 uses PCF8574AT, IIC's address is 0x3F.
*/
LiquidCrystal_I2C lcd(0x27,16,2); 

char *problems[] = {"Solve 3x-5=10", "Solve 2(x+6)=24", "Solve x-5=4", "Solve 2x-8=6", "21: fibonacci #?" , "Solve 9x+2 = 20" , "Solve x=6+2", "Solve x^2+4= 12", "Free. Type 4", "Free. Type 4"};
char *answers[] = {"5", "6", "9", "7" , "9" , "2", "8" , "4" , "3", "4"};

void setup() {
  Wire.begin(SDA, SCL);           // attach the IIC pin
  lcd.init();                     // LCD driver initialization
  lcd.backlight();                // Open the backlight
  lcd.setCursor(0,0);             // Move the cursor to row 0, column 0
  lcd.print("7*3");     // The print content is displayed on the LCD
  Serial.begin(115200);       // Initialize the serial port and set the baud rate to 115200
  irrecv.enableIRIn();        // Start the receiver
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(recvPin);   //print the infrared receiving pin
  pinMode(PIN_BUZZER, OUTPUT); 

}

bool newQuestion = true; 
bool correct = false; 
int question; 

void loop() {
  lcd.setCursor(0,0); 
//  lcd.clear();
  if((millis() / 1000) % 86400 == 0){

    lcd.backlight(); 
    newQuestion=true; 
    correct = false; 
  }
  if(!correct){
    digitalWrite(PIN_BUZZER, HIGH);
    if(newQuestion){
      question = random(0,9);
      lcd.clear();
      lcd.print(problems[question]); 
      newQuestion=false; 

    }
    
      if (irrecv.decode(&results)) {          // Waiting for decoding
      
        switch(results.value){
          case 0xFFA25D:
  //          lcd.print("Power");
            if(on){
              lcd.noBacklight();
            } else {
              lcd.backlight();
            }
            on = !on; 
            break;
          case 0xFFE21D:
  //          lcd.print("Menu");
            break;  
          case 0xFF22DD:
  //          lcd.print("Test");
            break;  
          case 0xFF02FD:
  //          lcd.print("+");
            break;      
          case 0xFFC23D:
  //          lcd.print("Back");
            break;  
          case 0xFFE01F:
  //          lcd.print("Reverse");
            break;  
          case 0xFFA857:
  //          lcd.print("Play");
            break;  
          case 0xFF906F:
  //          lcd.print("Forward");
            break;  
          case 0xFF6897:
  //          lcd.print("0");
            break;  
          case 0xFF9867:
  //          lcd.print("-");
            break;  
          case 0xFFB04F:
  //          lcd.print("C");
            break;  
          case 0xFF30CF:
  //          lcd.print("1");
            break;  
          case 0xFF18E7:
  //          lcd.print("2");
              if(question == 5){
                correct = true; 
              } else {
                newQuestion = true; 
              }
            break;  
          case 0xFF7A85:
  //          lcd.print("3");
               if(question == 8){
                correct = true; 
              } else {
                newQuestion = true; 
              }
            break;  
          case 0xFF10EF:
  //          lcd.print("4");
              if(question == 9 || question == 7){
                correct = true; 
              } else {
                newQuestion = true; 
              }
            break; 
          case 0xFF38C7:
  //          lcd.print("5");
              if(question == 0){
                correct = true; 
              } else {
                newQuestion = true; 
              }
            break;  
          case 0xFF5AA5:
  //          lcd.print("6");
              if(question == 1){
                correct = true; 
              } else {
                newQuestion = true; 
              }
            break;  
          case 0xFF42BD:
  //          lcd.print("7");
              if(question == 3){
                correct = true; 
              } else {
                newQuestion = true; 
              }
            break;  
          case 0xFF4AB5:
  //          lcd.print("8");
              if(question == 6){
                correct = true; 
              } else {
                newQuestion = true; 
              }
            break;  
          case 0xFF52AD:
  //          lcd.print("9");
              if(question == 4){
                correct = true; 
              }  else {
                newQuestion = true; 
              } 
            break;
             
        } 
        
        serialPrintUint64(results.value, HEX);// Print out the decoded results
        Serial.println("");
        irrecv.resume();                      // Release the IRremote. Receive the next value
    }
  }else {
    digitalWrite(PIN_BUZZER,LOW);
    lcd.clear();
    lcd.print("Correct!");
    lcd.noBacklight();
  }
  
//  lcd.setCursor(0,1);             // Move the cursor to row 1, column 0
//  lcd.print("Counter:");          // The count is displayed every second
//  lcd.print(millis() / 1000);
  delay(1000);
}

//#define TONE_TIMER TC1
//#define TONE_CHNL 0
//#define TONE_IRQ TC3_IRQn
//
//// TIMER_CLOCK4   84MHz/128 with 16 bit counter give 10 Hz to 656KHz
////  piano 27Hz to 4KHz
//
//#define TONE_TIMER TC1
//#define TONE_CHNL 0
//#define TONE_IRQ TC3_IRQn
//
//static uint8_t pinEnabled[36];
//static uint8_t TCChanEnabled = 0;
//static boolean pin_state = false ;
//static tc *chTC = TONE_TIMER;
//static uint32_t chNo = TONE_CHNL;
//
//volatile static int32_t toggle_count;
//static uint32_t tone_pin;
//
//void tone(uint32_t ulPin, uint32_t frequency, int32_t duration)
//{
//        const uint32_t rc = VARIANT_MCK / 256 / frequency; 
////        tone_pin = ulPin;
//        toggle_count = 0;  // strange  wipe out previous duration
//        if (duration > 0 ) toggle_count = 2 * frequency * duration / 1000;
//         else toggle_count = -1;
//
//        if (!TCChanEnabled) {
//            pmc_set_writeprotect(false);
//            pmc_enable_periph_clk((uint32_t)TONE_IRQ);
//            TC_Configure(chTC, chNo,
//                TC_CMR_TCCLKS_TIMER_CLOCK4 |
//                TC_CMR_WAVE |         // Waveform mode
//                TC_CMR_WAVSEL_UP_RC ); // Counter running up and reset when equals to RC
//
//            chTC->TC_CHANNEL[chNo].TC_IER=TC_IER_CPCS;  // RC compare interrupt
//            chTC->TC_CHANNEL[chNo].TC_IDR=~TC_IER_CPCS;
//             NVIC_EnableIRQ(TONE_IRQ);
//                         TCChanEnabled = 1;
//        }
//        if (!pinEnabled[ulPin]) {
//            pinMode(ulPin, OUTPUT);
//            pinEnabled[ulPin] = 1;
//        }
//        TC_Stop(chTC, chNo);
//                TC_SetRC(chTC, chNo, rc);    // set frequency
//        TC_Start(chTC, chNo);
//}
//
//void noTone(uint32_t ulPin)
//{
//    TC_Stop(chTC, chNo);  // stop timer
//    digitalWrite(ulPin,LOW);  // no signal on pin
//}
