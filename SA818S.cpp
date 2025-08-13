#include <SA818S.h>


/**
 * @brief Connect to the SA818S module and verify connection
 * @return true if connection is successful (+DMOCONNECT:0 received), false otherwise
 */
SA818S::SA818S_Status_t SA818S::CONNET(void)
{
    // Clear any existing data in the serial buffer before sending command
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    // Send the connection command to the SA818S module
    Serial.println("AT+DMOCONNECT");
    
    // Wait for and read the module's response with timeout handling
    if (this->readSerialTimeout()) {
        // Verify if the response indicates a successful connection
        if (lastResponse.indexOf("+DMOCONNECT:0") >= 0) {
            return SA818S_OK;  // Return success status
        }
    }
    // Return error status for timeout or connection failure
    return SA818S_ERROR;  
}


/**
 * @brief Set the control pins and their logic levels for SA818S module
 * @param PIN_PTT PTT (Push To Talk) pin number - Transmit/Receive control
 * @param level1 Logic level for PTT pin: LOW=Transmit, HIGH=Receive (default)
 * @param PIN_PD PD (Power Down) pin number - Sleep mode control
 * @param level2 Logic level for PD pin: LOW=Sleep mode, HIGH=Normal operation
 * @param PIN_HL HL (High/Low power) pin number - RF power level control
 * @param level3 Logic level for HL pin: LOW=Low power, HIGH=High power (default)
 */
void SA818S::setPin(uint8_t PIN_PTT, uint8_t level1, uint8_t PIN_PD, uint8_t level2,uint8_t PIN_HL,  uint8_t level3)
{
    // Set pin modes to output
    pinMode(PIN_PTT, OUTPUT);
    pinMode(PIN_PD, OUTPUT);
    pinMode(PIN_HL, OUTPUT);

    // Set logic levels for each control pin
    digitalWrite(PIN_PTT, level1);  // PTT: LOW=Transmit, HIGH=Receive
    digitalWrite(PIN_PD, level2);   // PD: LOW=Sleep, HIGH=Normal operation
    digitalWrite(PIN_HL, level3);   // HL: LOW=Low power, HIGH=High power
}


/**
 * @brief Set scan frequency and check for signal detection
 * @param frequency The frequency to scan for signal detection
 * @return SA818S_OK if signal is detected (S=0), SA818S_ERROR if no signal detected (S=1) or timeout
 */
SA818S::SA818S_Status_t SA818S::setScanFrequency(float frequency)
{
    // Clear any existing data in the serial buffer before sending command
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    // Send scan command with the specified frequency
    Serial.print("S+");
    char freqStr[12];
    dtostrf(frequency, 0, 4, freqStr);  // Convert float frequency to string with 4 decimal places
    Serial.println(freqStr);
    
    // Wait for and read the module's response with timeout handling
    if (this->readSerialTimeout()) {
        // Parse the scan result from the module's response
        if (lastResponse.indexOf("S=0") >= 0) {
            return SA818S_OK;  // Signal detected at the specified frequency
        } else if (lastResponse.indexOf("S=1") >= 0) {
            return SA818S_ERROR;  // No signal detected at the specified frequency
        }
    }
    // Return error status for timeout or invalid response
    return SA818S_ERROR;
}


/**
 * @brief Set group parameters for SA818S module including frequency, bandwidth and CTCSS/DCS
 * @param bandwidth Bandwidth setting (0: 12.5KHz, 1: 25KHz)
 * @param txFrequency Transmit frequency in MHz (134.0000-174.0000M or 400.0000-470.0000M)
 * @param rxFrequency Receive frequency in MHz (134.0000-174.0000M or 400.0000-470.0000M)
 * @param txCSS Transmit CTCSS/DCS code ("0000" for none;"0001"-"0038"：CTCSS;With a letter at the end:CDCSS)
   The encoding can be found in Table 1 of the appendix of File SA818S 1W Embedded walkie talkie module.
 * @param squelch Squelch level (0-8, where 0 is monitor mode)
 * @param rxCSS Receive CTCSS/DCS code ("0000" for none;"0001"-"0038"：CTCSS;With a letter at the end:CDCSS)
   The encoding can be found in Table 1 of the appendix of File SA818S 1W Embedded walkie talkie module.
 * @return SA818S_OK if group setting is successful (+DMOSETGROUP:0 received), SA818S_ERROR otherwise
 */
SA818S::SA818S_Status_t SA818S::setGroup(uint8_t bandwidth, float txFrequency, float rxFrequency, 
                      String txCSS, uint8_t squelch, String rxCSS)
{   
    // Clear serial buffer before sending command
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    // Construct and send the AT+DMOSETGROUP command with all parameters
    Serial.print("AT+DMOSETGROUP=");
    Serial.print(bandwidth);
    Serial.print(",");
    
    char txFreqStr[12];
    dtostrf(txFrequency, 0, 4, txFreqStr);
    Serial.print(txFreqStr);
    Serial.print(",");
    
    char rxFreqStr[12];
    dtostrf(rxFrequency, 0, 4, rxFreqStr);
    Serial.print(rxFreqStr);
    Serial.print(",");
    
    Serial.print(txCSS);
    Serial.print(",");
    
    Serial.print(squelch);
    Serial.print(",");
    
    Serial.println(rxCSS);

    // Read module response
    if (this->readSerialTimeout()) {
        // Check if response indicates successful group setting
        if (lastResponse.indexOf("+DMOSETGROUP:0") >= 0) {
            return SA818S_OK;  // Group setting successful
        }
    }
    return SA818S_ERROR;  // Group setting failed or timeout occurred
}

/**
 * @brief Set the volume level for SA818S module
 * @param volume Volume level (1-8, where 1 is minimum and 8 is maximum)
 * @return SA818S_OK if volume setting is successful (+DMOSETVOLUME:0 received), SA818S_ERROR otherwise
 */
SA818S::SA818S_Status_t SA818S::setVolume(uint8_t volume) 
{
    // Clear serial buffer before sending command
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    // Send volume setting command
    Serial.print("AT+DMOSETVOLUME=");
    Serial.println(volume);
    
    // Read module response
    if (this->readSerialTimeout()) {
        // Check if response indicates successful volume setting
        if (lastResponse.indexOf("+DMOSETVOLUME:0") >= 0) {
            return SA818S_OK;  // Volume setting successful
        }
    }
    return SA818S_ERROR;  // Volume setting failed or timeout occurred
}


/**
 * @brief Read RSSI (Received Signal Strength Indicator) from SA818S module
 * RSSI=XXX Signal Strength Response:
 * Format: RSSI:XXX
 * Example: RSSI:010
 * Response Parameter XXX: Current signal strength value
 * - Higher values indicate stronger signals
 * - Lower values indicate weaker signals
 */
void SA818S::readRSSI(void) 
{    
    // Clear serial buffer before sending command
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    // Send RSSI query command
    Serial.println("RSSI?");
    
    // Read module response
    this->readSerialTimeout();
}

/**
 * @brief Set filter parameters for SA818S module
 * @param preDeEmph Pre-emphasis/De-emphasis setting (0: Enable, 1: Disable)
 * @param highPass High-pass filter setting (0: Enable, 1: Disable)
 * @param lowPass Low-pass filter setting (0: Enable, 1: Disable)
 * @return SA818S_OK if filter setting is successful (+DMOSETFILTER:0 received), SA818S_ERROR otherwise
 */
SA818S::SA818S_Status_t SA818S::setFilter(uint8_t preDeEmph, uint8_t highPass, uint8_t lowPass)
{    
    // Clear serial buffer before sending command
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    // Send filter setting command
    Serial.print("AT+SETFILTER=");
    Serial.print(preDeEmph);
    Serial.print(",");
    Serial.print(highPass);
    Serial.print(",");
    Serial.println(lowPass); 
    
    // Read module response
    if (this->readSerialTimeout()) {
        // Check if response indicates successful filter setting
        if (lastResponse.indexOf("+DMOSETFILTER:0") >= 0) {
            return SA818S_OK;  // Filter setting successful
        }
    }
    return SA818S_ERROR;  // Volume setting failed or timeout occurred
}

/**
 * @brief Set transmit tail tone for SA818S module
 * @param tail Tail tone setting (0: Disable transmit tail tone, 1: Enable transmit tail tone)
 * @return SA818S_OK if tail tone setting is successful (+DMOSETTAIL:0 received), SA818S_ERROR otherwise
 */
SA818S::SA818S_Status_t SA818S::setTail(uint8_t tail) 
{
    // Clear serial buffer before sending command
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    // Send tail tone setting command
    Serial.print("AT+SETTAIL=");
    Serial.println(tail);
    
    // Read module response
    if (this->readSerialTimeout()) {
        // Check if response indicates successful tail tone setting
        if (lastResponse.indexOf("+DMOSETTAIL:0") >= 0) {
            return SA818S_OK;  // Tail tone setting successful
        }
    }
    return SA818S_ERROR;  // Tail tone setting failed or timeout occurred
}

/**
 * @brief Read current group settings from SA818S module
 * AT+DMOREADGROUP Response:
 * Format: +DMOREADGROUP:bandwidth,txFreq,rxFreq,txCSS,squelch,rxCSS
 * Example: +DMOREADGROUP:0,145.1250,145.1250,0001,4,0001
 * 
 * Response Parameters:
 * - bandwidth: Current bandwidth setting (0=12.5KHz, 1=25KHz)
 * - txFreq: Current transmit frequency in MHz
 * - rxFreq: Current receive frequency in MHz
 * - txCSS: Current transmit CTCSS/DCS code
 * - squelch: Current squelch level (0-8)
 * - rxCSS: Current receive CTCSS/DCS code
 */
void SA818S::readGroup(void) 
{    
    // Clear serial buffer before sending command
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    // Send read group settings command
    Serial.println("AT+DMOREADGROUP");
    
    // Read module response containing current group settings
    this->readSerialTimeout();
}

/**
 * @brief Get firmware version information from SA818S module
 */
void SA818S::getVersion(void) 
{    
    // Clear serial buffer before sending command
    while(Serial.available() > 0) {
        Serial.read();
    }
    
    // Send version query command
    Serial.println("AT+VERSION");
    
    // Read module response containing version information
    this->readSerialTimeout();
}



/**
 * @brief Read serial data with timeout mechanism
 * @return uint8_t true if data received successfully, false if timeout occurred
 * 
 * This function implements a timeout-based serial reading mechanism:
 * - Continuously monitors serial port for incoming data
 * - Resets timeout counter when data is received
 * - Stops reading when no new data received for 500ms
 */
uint8_t SA818S::readSerialTimeout()
{
    response = "";
    unsigned long timeout = millis();
    
    // Read serial data with 500ms timeout
    while (millis() - timeout < 500) {
        if (Serial.available() > 0) {
            char c = Serial.read();
            response += c;
            
            // Reset timeout counter when data is received
            timeout = millis();
        }
    }
    
    if (response.length() > 0) {
        // Process received response
        lastResponse = response; 
        return true;  // Data received successfully
    } else {
        Serial.println("Read timeout");
        return false; // Timeout occurred
    }
}




