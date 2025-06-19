void send_command(int command_pos){
  
    /////////////////////////////// Getting Protocol From EEPROM ////////////////////////////
    get_from_EEPROM(150);   
    protocol_abc = strToDecodeType(save_str.c_str());
    Serial.print("Converted back protocol = ");Serial.println(protocol_abc);

    /////////////////////////////// Getting Size of Protocol From EEPROM ////////////////////////////
    get_from_EEPROM(140);
    read_size=save_str.toInt();
    Serial.print("Converted back size = ");Serial.println(read_size);

    /////////////////////////////// Getting Command From EEPROM ////////////////////////////
    get_from_EEPROM(command_pos);
    delay(500);
    String eepromString = save_str;
    int index = 0;
    while (eepromString.length() > 0) {
        int commaIndex = eepromString.indexOf(",");
        if (commaIndex == -1) {
            commaIndex = eepromString.length();
        }
        String hexValue = eepromString.substring(0, commaIndex);
        arrayFromEEPROM[index] = strtol(hexValue.c_str(), nullptr, 16);
        index++;
        eepromString.remove(0, commaIndex + 2);
    }
    // Print the array of uint8_t values
    for (int i = 0; i < (read_size/8); i++) {
        Serial.print(arrayFromEEPROM[i], HEX);
        Serial.print(" ");
    }
    irsend.send(protocol_abc, arrayFromEEPROM, read_size/8);butt=0;
}
