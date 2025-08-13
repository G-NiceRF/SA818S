#ifndef __SA818S_H__
#define __SA818S_H__

#include "Arduino.h"


class SA818S
{
	
public:

  String response;  
  typedef enum {
      SA818S_OK = 0,      
      SA818S_ERROR,      
  } SA818S_Status_t;

  SA818S_Status_t CONNET(void);
  void setPin(uint8_t PIN_PTT, uint8_t level1, uint8_t PIN_PD, uint8_t level2,uint8_t PIN_HL,  uint8_t level3);
  SA818S_Status_t setScanFrequency(float frequency);
  SA818S_Status_t setGroup(uint8_t bandwidth, float txFrequency, float rxFrequency, String txCSS, uint8_t squelch, String rxCSS);
  SA818S_Status_t setVolume(uint8_t volume);
  void readRSSI(void);
  SA818S_Status_t setFilter(uint8_t preDeEmph, uint8_t highPass, uint8_t lowPass);
  SA818S_Status_t setTail(uint8_t tail);
  void readGroup(void);
  void getVersion(void);
  uint8_t readSerialTimeout();
protected:
  
	
private:
  String lastResponse; 
};

#endif





