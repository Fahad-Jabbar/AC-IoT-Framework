void fetchAPI() {
  HTTPClient http;

  // Your API URL
  String apiUrl = "https://atmt.innovateitt.com/dash/2/data_read.php?user_id=123&device_id=456";

  // Send an HTTP GET request
  http.begin(apiUrl);

  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    String payload = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response data: ");
    Serial.println(payload);
//    String payload_str = "{\""+ payload.substring(0,5) + "\"" + payload.substring(5) + "}";
//    Serial.print("Response data Json : ");
//    Serial.println(payload_str);
//    json_extract(payload_str);
    json_extract(payload);

    // You can process the 'payload' string here
  } else {
    Serial.println("Error on HTTP request");
  }

  http.end();
}


void json_extract(String str){
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 10;
  DynamicJsonBuffer jsonBuffer(bufferSize);

  JsonObject& root = jsonBuffer.parseObject(str);
  
  if (!root.success()) {
    Serial.println("Failed to parse JSON");
    return;
  }
  
  //AC_val, In_temp, Out_temp, max_temp, min_temp, on_time, off_time, mode_val, thr_val, t_stamp

  // Check if the root object contains the key "State"
  if ((!root.containsKey("ac_reading"))||(!root.containsKey("inter_temp"))||(!root.containsKey("exter_temp"))||(!root.containsKey("max_temp"))||(!root.containsKey("min_temp"))||(!root.containsKey("schedule_on"))||(!root.containsKey("schedule_off"))||(!root.containsKey("control_value"))||(!root.containsKey("threshold_value"))||(!root.containsKey("state"))){//||(!root.containsKey("time_stamp"))){ 
    Serial.println("Respective keys not found in JSON");
    return;
  }
  
//  String  
  String A_val = root["ac_reading"];
  String I_temp = root["inter_temp"];
  String O_temp = root["exter_temp"];
  String ma_temp = root["max_temp"];
  String mi_temp = root["min_temp"];
  String onn_time = root["schedule_on"];
  String of_time = root["schedule_off"];
  String mod_val = root["control_value"];
  String th_val = root["threshold_value"];
  String stt = root["state"];
//  String stamp = root["time_stamp"]

  AC_val = A_val;
  In_temp = I_temp;
  Out_temp = O_temp;
  max_temp = ma_temp;
  min_temp = mi_temp;
  on_time = onn_time;
  off_time =of_time;
  mode_val = mod_val;
  thr_val = th_val;
  st = stt;
//  t_stamp = time_stamp;
  Serial.print("API Data ::    AC_Reading: ");Serial.print(AC_val);
  Serial.print(" ::    inter_temp: ");Serial.print(In_temp);
  Serial.print(" ::    exter_temp: ");Serial.print(Out_temp);
  Serial.print(" ::    max_temp: ");Serial.print(max_temp);
  Serial.print(" ::    min_temp: ");Serial.print(min_temp);
  Serial.print(" ::    schedule_on: ");Serial.print(on_time);
  Serial.print(" ::    schedule_off: ");Serial.print(off_time);
  Serial.print(" ::    control_value: ");Serial.print(mode_val);
  Serial.print(" ::    threshold_value: ");Serial.print(thr_val);
  Serial.print(" ::    State: ");Serial.println(st);

  Serial.println("///////////////////////////////////////////////");
  
  String hrs = on_time.substring(0,2);
  Serial.print("ON Time in hrs: ");Serial.println(hrs);
  String mins = on_time.substring(3,5);
  Serial.print("ON Time in mins: ");Serial.println(mins);

  t_on_mins = hrs.toInt()*60 + mins.toInt();
  Serial.print("Total time in mins: ");Serial.println(t_on_mins);

  hrs = off_time.substring(0,2);
  Serial.print("ON Time in hrs: ");Serial.println(hrs);
  mins = off_time.substring(3,5);
  Serial.print("ON Time in mins: ");Serial.println(mins);

  t_off_mins = hrs.toInt()*60 + mins.toInt();
  Serial.print("Total time in mins: ");Serial.println(t_off_mins);

  
  if (st == "ble"){
    EEPROM.write(1,0);EEPROM.commit();
    Serial.println("Going to BLE Mode");
    delay(1000);
    ESP.restart();
  }
  
}
