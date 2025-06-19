void update_ac_mode2(){

  if (EEPROM.read(10)==1){
//    if((temp_set!=1)&&(Out_temp.toInt()>=thr_val.toInt())){
    if(Out_temp.toInt()>=thr_val.toInt()){
      Serial.println("///////////////////////    Target 2 _ 2    ////////////////////////////////////////");
      Serial.println("Set AC Temperature to 20C");
      int value_ch = (String(pos_lenght[(min_temp.toInt()-14)])).toInt(); //// set AC to 20 degrees
      Serial.print("Value ch : ");Serial.print(value_ch);
      for(int ii=0; ii<=2; ii++){
        send_command(value_ch); //// set AC to 20 degrees
        delay(500);
      }
      AC_val = min_temp;
      temp_set=0; // 1
    }
//    else if((temp_set!=2)&&(Out_temp.toInt()<thr_val.toInt())){
    else if(Out_temp.toInt()<thr_val.toInt()){
      Serial.println("///////////////////////    Target 3 _ 2    ////////////////////////////////////////");
      Serial.println("Set AC Temperature to 26C");
      int value_ch = (String(pos_lenght[(max_temp.toInt()-14)])).toInt(); //// set AC to 20 degrees
      Serial.print("Value ch : ");Serial.print(value_ch);
      for(int ii=0; ii<=2; ii++){
        send_command(value_ch); //// set AC to 20 degrees
        delay(500);
      }
      AC_val = max_temp;
      temp_set=0; // 2
    }  
  }
  hit_api();
}

void mode2_on_off(){
  if ((EEPROM.read(10)!=1)&&(t_mins>=t_on_mins)&&(t_mins<t_off_mins)){
    Serial.println("///////////////////////    Target 1 _ 2    ////////////////////////////////////////");
    Serial.println("Turn AC ON");
    for(int ii=0; ii<=2; ii++){
      send_command(400); //// set AC to ON
      delay(500);
    }
    AC_val = "AC Turns ON";
    AC_val = "1";
    EEPROM.write(10,1);EEPROM.commit();
    hit_api();
  }
  
  if((EEPROM.read(10)==1)&&(t_mins>=t_off_mins)){
    Serial.println("///////////////////////    Target 4 _ 2    ////////////////////////////////////////");
    Serial.println("Turn AC OFF");
    for(int ii=0; ii<=2; ii++){
      send_command(200); //// set AC to OFF
      delay(500);
    }
    AC_val = "AC Turns OFF";
    AC_val = "0";
    EEPROM.write(10,0);EEPROM.commit();
    hit_api();
  }
  
}

void default_mode(){
  String default_threshold = "30"; /// 9AM to 9PM
  
  if ((EEPROM.read(10)!=1)&&(t_mins>=540)&&(t_mins<1260)){
    Serial.println("///////////////////////    Target 1 _ 1   ////////////////////////////////////////");
    Serial.println("Turn AC ON");
    for(int ii=0; ii<=2; ii++){
      send_command(400); //// set AC to ON
      delay(500);
    }
    AC_val = "AC Turns ON";
    AC_val = "1";
    EEPROM.write(10,1);EEPROM.commit();
    hit_api();
  }

  if (EEPROM.read(10)==1){
    if((temp_set!=1)&&(Out_temp.toInt()>=default_threshold.toInt())){
        Serial.println("///////////////////////    Target 2 _ 1    ////////////////////////////////////////");
        Serial.println("Set AC Temperature to 20C");
        int value_ch = (String(pos_lenght[(20-14)])).toInt(); //// set AC to 20 degrees
        Serial.print("Value ch : ");Serial.print(value_ch);
        for(int ii=0; ii<=2; ii++){
          send_command(value_ch); 
          delay(500);
        }
        AC_val = "20";
        temp_set=1;
        hit_api();
      }
    else if((temp_set!=2)&&(Out_temp.toInt()<default_threshold.toInt())){
      Serial.println("///////////////////////    Target 3 _ 1    ////////////////////////////////////////");
        Serial.println("Set AC Temperature to 26C");
        int value_ch = (String(pos_lenght[(26-14)])).toInt(); //// set AC to 26 degrees
        Serial.print("Value ch : ");Serial.print(value_ch);
        for(int ii=0; ii<=2; ii++){
          send_command(value_ch); //// set AC to 26 degrees
          delay(500);
        }
        AC_val = "26";
        temp_set=2;
        hit_api();
      }
  }
  
  if((EEPROM.read(10)==1)&&(t_mins>=1260)){
    Serial.println("///////////////////////    Target 4 _ 1    ////////////////////////////////////////");
    Serial.println("Turn AC OFF");
    for(int ii=0; ii<=2; ii++){
      send_command(200); //// set AC to OFF
      delay(500);
    }
    AC_val = "AC Turns OFF";
    AC_val = "0";
    EEPROM.write(10,0);EEPROM.commit();
    hit_api();
  }
}
