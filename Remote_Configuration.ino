void remote_setup(){
    irsend.begin();       // Start up the IR sender.
  Serial.print("Device will retransmit from Pin ");
  Serial.println(kIrLedPin);

  // Perform a low level sanity checks that the compiler performs bit field
  // packing as we expect and Endianness is as we expect.
  assert(irutils::lowLevelSanityCheck() == 0);

  Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);

#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(kMinUnknownSize);
#endif  // DECODE_HASH
  irrecv.setTolerance(kTolerancePercentage);  // Override the default tolerance.
  irrecv.enableIRIn();  // Start the receiver
}

void remote_configuration(){
  //////////////////////////////// Set Commands With respect to the asking commands /////////////////////////
  ////////////////////////////////////////////// Configure AC commands //////////////////////////////////////
  for (int i = 0; i < 15; i++) {
//    SerialBT.println("Press " + String(buttons_p[i]) + " Button");
    String ble_str = "Press :" + String(buttons_p[i])+"Button";
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
    Serial.println("Press " + String(buttons_p[i]) + " Button");
    
    int bb=0;
    while(bb==0){
      // Check if the IR code has been received.
      if (irrecv.decode(&results)) {
        // Display a crude timestamp.
        uint32_t now = millis();
        Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
        
        // Check if we got an IR message that was to big for our capture buffer.
        if (results.overflow)
          Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
        
        // Display the library version the message was captured with.
        Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_STR "\n");
        
        // Display the tolerance percentage if it has been change from the default.
        if (kTolerancePercentage != kTolerance)
          Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
        
        ////////////////////////////////////// Saving AC Protocol /////////////////////////
        protocol = results.decode_type;
        save_to_EEPROM(typeToString(protocol).c_str(),150);
    
        ////////////////////////////////////// Saving Protocol Size ///////////////////////
        size = results.bits;
        save_to_EEPROM(String(size),140);        
        
        String teststring = resultToSourceCode(&results);
        int first = teststring.indexOf('//');
        String UserID=teststring.substring(0,first);
        
        int secondOpen = first + 1;
        int secondClos = teststring.indexOf('[', secondOpen);
        
        String DeviceGroup=teststring.substring(secondOpen,secondClos);
        
        int thirdOpen= secondClos + 1;
        int thirdClose = teststring.indexOf(']',thirdOpen);
        
        String Array_Lenght=teststring.substring(thirdOpen,thirdClose);
        
        if (Array_Lenght.startsWith("uint16_t ")){
          Serial.println ("Wrong attributes");
//          "Press :" + String(buttons_p[i])+"Button";
          String ble_str = "Not Recognized! : Press the " + String(buttons_p[i]) +" button Again!";
          pCharacteristic->setValue(ble_str.c_str());
          pCharacteristic->notify();
//          SerialBT.println("Press the button AGAIN!");
          led_blinking(red_pin, no_of_blinks, blinking_speed); ////////////////////////////////////////////// Red led indication
        }  
        else{
          int forthOpen= thirdClose + 1;
          int forthClose = teststring.indexOf('{',forthOpen);
          String FALTU1=teststring.substring(forthOpen,forthClose);
          
          int fifthOpen= forthClose + 1;
          int fifthClose = teststring.indexOf('}',fifthOpen);
          String DeviceID=teststring.substring(fifthOpen,fifthClose);
        
          if (DeviceID.startsWith("uint16_t ")){
            Serial.println ("Wrong attributes");
            String ble_str = "Not Recognized! : Press the " + String(buttons_p[i]) +" button Again!";
            pCharacteristic->setValue(ble_str.c_str());
            pCharacteristic->notify();
//            SerialBT.println("Press the button AGAIN!");
            led_blinking(red_pin, no_of_blinks, blinking_speed); ////////////////////////////////////////////// Red led indication
          }
          else{
            Serial.print("Protocol = ");Serial.print(protocol);Serial.print("/");Serial.print(typeToString(protocol).c_str());Serial.print(" and size = ");Serial.println (size);
            Serial.println (pos_lenght[i]);
            Serial.println (DeviceID);

            //////////////////////////////////// Saving Command to EEPROM //////////////////////////////////
            save_to_EEPROM(DeviceID,int(pos_lenght[i]));
            delay (10);
            led_blinking(blue_pin, no_of_blinks, blinking_speed); ////////////////////////////////////////////// Blue led indication
          } // end of esle loop (if read correct codes of remote)
          
          yield();             // Feed the WDT (again)
          bb=1;
        } // end of esle loop (if read correct lenght of array)
      }  // end of if loop of data decode result.decode
    } //end of while loop
  
  }  // end of for loop
  
//  SerialBT.println("");
//  SerialBT.println("Remote Added Successfully");
  String ble_str = "Remote Added Successfully!";
  pCharacteristic->setValue(ble_str.c_str());
  pCharacteristic->notify();
  Serial.println("Remote Added Successfully");
}

void read_AC_Commands(){
  /////////////////////////////// Getting Protocol From EEPROM ////////////////////////////
    get_from_EEPROM(150);   
    protocol_abc = strToDecodeType(save_str.c_str());
    Serial.print("Converted back protocol = ");Serial.println(protocol_abc);
    String ble_str = "Converted back protocol = " + String(protocol_abc);
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
//    SerialBT.print("Converted back protocol = ");SerialBT.println(protocol_abc);
    
    /////////////////////////////// Getting Size of Protocol From EEPROM ////////////////////////////
    get_from_EEPROM(140);
    read_size=save_str.toInt();
    Serial.print("Converted back size = ");Serial.println(read_size);
    ble_str = "Converted back size = " + String(read_size);
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
//    SerialBT.print("Converted back size = ");SerialBT.println(read_size);

    /////////////////////////////// Getting all the commands From EEPROM ////////////////////////////
    for (int q = 0; q < 15; q++) {
      get_from_EEPROM(pos_lenght[q]);
      delay(50);
    }
}
