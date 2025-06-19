void save_to_EEPROM(String str,int pos){
  Serial.print("String to Save : "); Serial.println(str);
  Serial.print("Location : "); Serial.println(pos); 
  EEPROM.write(pos,str.length());EEPROM.commit();
  for (int i=0; i<str.length();i++){
    EEPROM.write(i+pos+1,char(str[i]));EEPROM.commit();
  }
}

void get_from_EEPROM(int pos){
  save_str="";
  Serial.print("Getting from EEPROM : ");
  for (int i=0; i<EEPROM.read(pos);i++){
    Serial.print(char(EEPROM.read(i+pos+1)));
//    SerialBT.print(char(EEPROM.read(i+pos+1)));
    save_str=save_str+char(EEPROM.read(i+pos+1));
  }
  delay(500);
  if (EEPROM.read(1)!=1){
    String ble_str = "Getting from EEPROM : " + String(save_str);
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
  }
  Serial.println();
//  SerialBT.println();
  Serial.println("//////////////////////////////////////////////////");
}
