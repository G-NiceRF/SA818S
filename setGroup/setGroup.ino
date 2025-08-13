/**
 * SA818S Module Hardware Connection Instructions:
 * 
 * SA818S Module Pin Connections:
 * - VCC  ->  3.3V (Pin 3V3)
 * - GND  ->  GND
 * - TX   ->  GPIO3 (RX0)
 * - RX   ->  GPIO1 (TX0)
 * 
 * Control Pin Connections:
 * - PTT (GPIO 2) -> SA818S Pin 5 (Transmit/Receive Control: LOW=Transmit, HIGH=Receive)
 * - PD  (GPIO 9) -> SA818S Pin 6 (Power Down Control: LOW=Sleep, HIGH=Normal Operation)
 * - HL  (GPIO 4) -> SA818S Pin 7 (High/Low Power Control: LOW=Low Power, HIGH=High Power)
 * 
 * ESP32-PICO-KIT Pin Mapping:
 * - GPIO 2: Pin D2
 * - GPIO 9: Pin D9  
 * - GPIO 4: Pin D4
 * - GPIO 16: Pin RX2
 * - GPIO 17: Pin TX2
 * - 3V3: Power supply (3.3V)
 * - GND: Ground

 * AT+DMOSETGROUP set group command
   Description: Set the working parameters of the module.
   Format: AT+DMOSETGROUP=GBW, TFV, RFV, Tx_CXCSS, SQ, Rx_CXCSS
   Example 1: AT+DMOSETGROUP=0,415.1250,415.1250,0012,4,0013
   Example 2: AT+DMOSETGROUP=0,415.1250,415.1250,754N,4,445I
   Parameter Description:
   GBW: bandwidth setting (0: 12.5K 1: 25K)
   TFV: Transmit frequency value (134.0000M～174.0000M,400.0000M～470.0000M)
   RFV: Receiving frequency value (134.0000M～174.0000M,400.0000M～470.0000M)
   (Note: The transmitting frequency and receiving frequency can be the same or different, but the set
   frequency must be 12.5K and 25K. Integer multiples)
   Tx_CXCSS: Transmit CXCSS value
   SQ: Squelch level (0～8) (0: monitor mode, scan mode cannot be used 0)
   Rx_CXCSS: Receive CXCSS value
   (Note: different CXCSS values can be used for transmitting and receiving, 0000: no coding
   0001-0038: CTCSS
   Followed by letters: CDCSS, coding see appendix 1)
 * +DMOSETGROUP Set group response command
   Description: After the module receives the setting group command, it returns the operation result
   Format: +DMOSETGROUP:X
   Example: +DMOSETGROUP:0
   Parameter X description: 0 —> success, 1 —> data setting out of range
 */

#include <SA818S.h>

#define PTT 2
#define PD  9
#define HL  4

SA818S radio;    
SA818S::SA818S_Status_t status;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.setPin(PTT, HIGH, PD, LOW, HL, HIGH);
  delay(3000);
  radio.CONNET();  
  Serial.println(radio.response); 
  delay(100);  

  status = radio.setGroup(0, 415.1250, 415.1250, "0012", 5, "0013");
  Serial.println(radio.response); 

  if( status == SA818S::SA818S_OK)
  {
      Serial.println("SET_OK.");
  }
  else
  {
      Serial.println("Data settings are outside the permitted range.");
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
