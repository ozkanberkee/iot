
#include "thingProperties.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128    
#define SCREEN_HEIGHT 64    
#define OLED_RESET -1       
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int sampleWindow = 50;                              
unsigned int sample;

void setup() {
   Serial.begin(115200);                                   
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);             
   display.display();                                    
   display.clearDisplay();                               
   display.setTextSize(1);                                
   display.setTextColor(WHITE);                           
   display.setCursor(20,20);                                
   display.println("Decibel Meter");              
   display.display();                                     
   delay(2000); 

 
  initProperties();

  
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
 
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  
    display.clearDisplay();
    display.setCursor(20,20);  
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.print("WiFi Connected");
    display.display();
    delay(4000);
    display.clearDisplay(); 
}

void loop() {
  ArduinoCloud.update();
   unsigned long startMillis= millis();                  
   float peakToPeak = 0;                                  
 
   unsigned int signalMax = 0;                            
   unsigned int signalMin = 1024;                         
 
                                                          
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);                             
      if (sample < 1024)                                  
      {
         if (sample > signalMax)
         {
            signalMax = sample;                          
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           
         }
      }
   }
   peakToPeak = signalMax - signalMin;                    
   db = map(peakToPeak,20,900,49.5,90);            
   display.setCursor(0,0);                               
   display.setTextSize(2);                                
   display.print(db);                                    
   display.print(" dB");                                  
 
 
    for(int x =5;x<114;x=x+6){                            
      display.drawLine(x, 32, x, 27, WHITE);
    }
   display.drawRoundRect(0, 32, 120, 20, 6, WHITE);      
   int r = map(db,0,120,1,120);                           
   display.fillRoundRect(1, 33, r, 18, 6, WHITE);         
   display.display();                                     
   display.clearDisplay();
   delay(150);
  
}