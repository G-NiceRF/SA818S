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

 * S+ sweep command
   Description: Set the frequency to be scanned. 
   Format: S+scanRF
   Example: S+455.2250
   Parameter Description:
   scanRF: the frequency to be scanned
 * S=Sweep frequency response command
   Description: Sweep response command, the module will go to sweep after receiving the sweep
   command, and return the sweep result after sweeping the frequency
   Format: S=X
   Parameter X description: 0—>the frequency to be swept has a signal 1—>the frequency to be swept
   has no signal
 */

#include <SA818S.h>

#define scanRF 415.2250
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

  status = radio.setScanFrequency(scanRF);
  Serial.println(radio.response); 

  if( status == SA818S::SA818S_OK)
  {
      Serial.println("The frequency to be scanned has a signal.");
  }
  else
  {
      Serial.println("There is no signal at the frequency to be scanned.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
