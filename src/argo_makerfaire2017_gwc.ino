/*
 *  argo design - Maker Faire Austin 2017 
 *  Girls Who Code Workshop
 *  
 *  This work is licensed under a MIT license https://opensource.org/licenses/MIT
 *  Copyright (c) 2017, argo design
 */


#include <MPR121_MOD.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h> 
#include <SFEMP3Shield.h>

#define baudRate 57600
#define cap1Addr 0x5C  //Touch board

#define STOP_PLAY 0

SFEMP3Shield MP3player;
SdFat sd;
SdFile trackFile;

MPR121_MOD_t cap = MPR121_MOD_t();

unsigned long touchPoints = 0;
unsigned long lastTouchPoints = 0;



void setup() 
{
    Serial.begin(baudRate);
  
    if(!sd.begin(SD_SEL, SPI_HALF_SPEED)) sd.initErrorHalt();
   
    Wire.begin();
    beginCap(&cap, cap1Addr); 

    delay(1000);
    byte result = MP3player.begin();   
    MP3player.setVolume(10, 10); 

    if(result != 0) 
    {
        Serial.print("Error code: ");
        Serial.print(result);
        Serial.println(" when trying to start MP3 player");
    }       
}



void loop() 
{
    updateCapTouchPoints();
  
    if(touchPoints != lastTouchPoints) {
        playSong();  
    }
    
    delay(500);
}



void beginCap(MPR121_MOD_t *cap, int addr)
{
    if (cap->begin(addr)) {
        cap->setTouchThreshold(12);
        cap->setReleaseThreshold(5);
    }
}



void playSong() 
{ 
      if(touchPoints == 0 || touchPoints > 8191) return;
      
      char trackName[] = "T_0000.mp3";
      sprintf(trackName, "T_%04d.mp3", touchPoints);

      MP3player.stopTrack();  
      uint8_t result = MP3player.playMP3(trackName);
      /*
      Serial.print("result for ");
      Serial.print(trackName);
      Serial.print(": ");
      Serial.println(result);      
      */
}



void updateCapTouchPoints() 
{
    unsigned long touchInput = 0;
    lastTouchPoints = touchPoints;       
    
    touchPoints = 0;

    cap.updateTouchData();    
    touchPoints = cap.getRawTouchData();    

    /*
    Serial.print("read: ");
    Serial.println(touchPoints);  
    */
}
