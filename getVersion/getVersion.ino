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
 *
 *  AT+VERSION read version number command
    Description: Read the software version number of the module
    Format: AT+VERSION
    Example: AT+VERSION
 *  +VERSION Read version number response command
    Description: Read version number response command
    Format: +VERSION:X
    Example: +VERSION:SA818_V5.0
    Parameter description: X —> module software version number
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

  radio.getVersion();
  Serial.println(radio.response); 

}
void loop() {
  // put your main code here, to run repeatedly:

}
