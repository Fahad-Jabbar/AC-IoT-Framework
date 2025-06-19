void ble_setup(){
  BLEDevice::init("AC Temp. Control Device");
  
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
  
    BLEService *pService = pServer->createService("19B10000-E8F2-537E-4F6C-D104768A1214");
  
    pCharacteristic = pService->createCharacteristic(
                        "19B10001-E8F2-537E-4F6C-D104768A1214",
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE |
                        BLECharacteristic::PROPERTY_NOTIFY
                      );
  
    pCharacteristic->setCallbacks(new MyCallbacks());
  
    pService->start();
  
    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
    Serial.println("Fingerprint BLE device advertising...");
}

void ble_cmd(String msg){
  
  if (msg.startsWith("@ID:")){
    msg=msg.substring(4);
    device_id="";
    for (int i=0; i<=msg.length(); i++){
      if (msg[i]!=';'){
        device_id += msg[i]; 
      }
    }
    
    Serial.print("Device ID : ");Serial.print(device_id);
    save_to_EEPROM(device_id, id_pos);
    Serial.print("Get Device ID : ");Serial.print(device_id);
    String ble_str = "Device_id :" + device_id;
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
  }
  
  else if (msg.startsWith("@read:")){
    get_from_EEPROM(id_pos);
    device_id=save_str;
    Serial.print("Get Device ID : ");Serial.print(device_id);
    String ble_str = "Device_id :" + device_id;
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
//    SerialBT.print("Get Device ID : ");SerialBT.println(device_id);
  }

  else if (msg.startsWith("@Placement_start")){
    pos_set=1;
    String ble_str = "Send '@Placement_done' to stop sending command";
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
  }
  
  else if (msg.startsWith("@Placement_done")){
    pos_set=0;
    String ble_str = "Placement Done!";
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
  }
  
  else if (msg.startsWith("@configure")){
    remote_configuration();
  }
  
  else if (msg.startsWith("@commands")){
    read_AC_Commands();
  }
  
  else if (msg.startsWith("@get_MAC")){
    Serial.print("MAC Address: ");Serial.println(WiFi.macAddress()); 
    String ble_str = "MAC_Address :" + String(WiFi.macAddress());
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
//    SerialBT.print("MAC Address: ");SerialBT.println(WiFi.macAddress());
  }

  else if (msg.startsWith("@done")){
    EEPROM.write(1,1);EEPROM.commit();
    Serial.println("ESP Restarts in GSM Mode");
    delay(100);
    ESP.restart();
  }
  
  else if (msg.startsWith("@running")){
    EEPROM.write(1,1);EEPROM.commit();
    delay(1000);
    ESP.restart();
  }
  
  else if(msg.startsWith("@cred:")){
    msg=msg.substring(6);
    ssid_str="";
    int lo=0;
    for (int i=0;i<=msg.length();i++){
      
      if (msg[i]!= ';'){
        lo++;
        Serial.print("Length : ");Serial.println(lo);
        ssid_str = ssid_str+msg[i];
        
        EEPROM.write(31+i,msg[i]);EEPROM.commit();
        
        Serial.print("ssid_str : ");Serial.println(ssid_str);
        Serial.print("Ascii : ");Serial.println(EEPROM.read(31+i));
      }
      else{
        i=100;
      }
    }
    Serial.print("ssid_str : ");Serial.println(ssid_str);
    Serial.print("ssid_str_length : ");Serial.println(ssid_str.length());
    Serial.print("Length : ");Serial.println(lo);
    EEPROM.write(30,ssid_str.length());EEPROM.commit();
    
    msg=msg.substring(ssid_str.length()+1);
    pass_str="";
    for (int i=0;i<=msg.length();i++){
      if (msg[i]!= ';'){
        pass_str = pass_str+msg[i];
        EEPROM.write(71+i,msg[i]);EEPROM.commit();
      }

      else{
        i=100;
      }
    }
    Serial.print("pass_str : ");Serial.println(pass_str);
    Serial.print("pass_str_length : ");Serial.println(pass_str.length());
    EEPROM.write(70,pass_str.length());EEPROM.commit();
    
    Serial.println("//////////////////////// Const_Char ////////////////////////");
    ssid = ssid_str.c_str();
    Serial.print("ssid : ");Serial.println(ssid);
    
    password = pass_str.c_str();
    Serial.print("password : ");Serial.println(password);


    Serial.println("//////////////////////// EEPROM DATA ////////////////////////");
    ssid_str="";
    for (int i=0; i<=EEPROM.read(30)-1; i++){
      ssid_str = ssid_str + char(EEPROM.read(i+31));
      
    }
    Serial.print("ssid_str : ");Serial.println(ssid_str);
    Serial.print("ssid_str_length : ");Serial.println(ssid_str.length());
    
    pass_str="";
    for (int i=0; i<=EEPROM.read(70)-1; i++){
      pass_str = pass_str + char(EEPROM.read(i+71));
    }
    Serial.print("pass_str : ");Serial.println(pass_str);
    Serial.print("pass_str_length : ");Serial.println(pass_str.length());
    
    Serial.println("//////////////////////// Const_Char ////////////////////////");
    ssid = ssid_str.c_str();
    Serial.print("ssid : ");Serial.println(ssid);
    
    password = pass_str.c_str();
    Serial.print("password : ");Serial.println(password);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    int a = millis()/1000;
    while (WiFi.status() != WL_CONNECTED) {
//      blinking();
      delay(500);
      Serial.print(".");
      rst_fcn();
      int b=millis()/1000;
      if ((b-a)>50){
        Serial.println("Device Reset");
        String ble_str = "Device Reset : Not Connected!";
        pCharacteristic->setValue(ble_str.c_str());
        pCharacteristic->notify();
        delay(500);
        for (int i=0; i<=700; i++){
          EEPROM.write(i,0);EEPROM.commit();
        }
        ESP.restart();  
      }
    }
    String ble_str = "Device Connected!";
    pCharacteristic->setValue(ble_str.c_str());
    pCharacteristic->notify();
    Serial.println("\nConnected to WiFi");
    EEPROM.write(1,1);EEPROM.commit();
    delay(2000);
    ESP.restart();
    }
}

void BLE_Loop(){

  if (deviceConnected) {
  // You can add any additional operations here if needed
    delay(50);            //don't ned to run this at full speed.
  }

  // Disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);
    pServer->startAdvertising();
    Serial.println("Disconnected");
    Serial.println("Advertising again");
    oldDeviceConnected = deviceConnected;
  }
  // Connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
    Serial.println("Connected");
  }

  // Display received message
  if (receivedMessage.length() > 0) {
    Serial.print("Received Message: ");
    Serial.println(receivedMessage);
    String send_str = "Received String : " + String(receivedMessage);
    // Reply with the same message
    ble_cmd(String(receivedMessage));
//    pCharacteristic->setValue(send_str.c_str());
//    pCharacteristic->notify();

    receivedMessage = ""; // Clear the received message
  }
  delay(100); // Adjust as needed

}
