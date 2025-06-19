void wifi_setup(){
//  blinking();
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



    Serial.print("ssid : ");Serial.println(ssid);
    Serial.print("password : ");Serial.println(password);
  
    WiFi.begin(ssid, password);
    int a = millis()/1000;
    while (WiFi.status() != WL_CONNECTED) {
//      blinking();
      delay(500);
      Serial.print(".");
      rst_fcn();
      int b=millis()/1000;
      if ((b-a)>50){
        Serial.println("Reset Btn Pressed");
        delay(500);
//        for (int i=0; i<=700; i++){
//          EEPROM.write(i,0);EEPROM.commit();
//        }
        ESP.restart();  
      }
    }
    Serial.println("");
    Serial.println("WiFi connected");
  
    // Write data to Firebase
    delay(2000);  
}

void wifi_check(){
  if (WiFi.status() != WL_CONNECTED){
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
  
  
  
      Serial.print("ssid : ");Serial.println(ssid);
      Serial.print("password : ");Serial.println(password);
    
      WiFi.begin(ssid, password);
      int a = millis()/1000;
      while (WiFi.status() != WL_CONNECTED) {
  //      blinking();
        delay(500);
        Serial.print(".");
        rst_fcn();
        int b=millis()/1000;
        if ((b-a)>50){
          Serial.println("Reset Btn Pressed");
          delay(500);
  //        for (int i=0; i<=700; i++){
  //          EEPROM.write(i,0);EEPROM.commit();
  //        }
          ESP.restart();  
        }
      }
      Serial.println("");
      Serial.println("WiFi connected");
    
      // Write data to Firebase
      delay(2000);  
  }
}
