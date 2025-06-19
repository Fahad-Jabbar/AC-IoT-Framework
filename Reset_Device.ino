void rst_fcn(){
  if (digitalRead(rst_pin)==1){
    Serial.println("Reset Device ...");
    for (int i=0; i<EEPROM_limit; i++){
      EEPROM.write(1,0);EEPROM.commit();
    }
    Serial.println("Device Restarts in Bluetooth Mode");
    ESP.restart();
  }
}
