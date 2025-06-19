void hit_api(){
  Serial.println();
  Serial.print(" ::    AC_Reading: ");Serial.println(AC_val);
  Serial.print(" ::    inter_temp: ");Serial.println(In_temp);
  Serial.print(" ::    exter_temp: ");Serial.println(Out_temp);
  Serial.print(" ::    max_temp: ");Serial.println(max_temp);
  Serial.print(" ::    min_temp: ");Serial.println(min_temp);
  Serial.print(" ::    schedule_on: ");Serial.println(on_time);
  Serial.print(" ::    schedule_off: ");Serial.println(off_time);
  Serial.print(" ::    control_value: ");Serial.println(mode_val);
  Serial.print(" ::    threshold_value: ");Serial.println(thr_val);
  Serial.print(" ::    State: ");Serial.println(st);

  Serial.println("///////////////////////////////////////////////");
  
  String apiURL = "https://atmt.innovateitt.com/dash/2/data_insert_api.php?";
  String urlWithParams = apiURL + "user_id=123&device_id=456&ac_reading=" + AC_val + "&inter_temp="+ In_temp + "&exter_temp=" + Out_temp + "&max_temp=" + max_temp + "&min_temp="+ min_temp + "&schedule_on=" + on_time + "&schedule_off=" + off_time + "&control_value=" + mode_val + "&threshold_value="+ thr_val + "&state=wifi";
//user_id=123&device_id=456&ac_reading=" + AC_val + "&inter_temp="+ In_temp + "&exter_temp=" + Out_temp + "&max_temp=" + max_temp + "&min_temp="+ min_temp + "&schedule_on=" + on_time + "&schedule_off=" + off_time + "&control_value=" + mode_val + "&threshold_value="+ thr_val + "&state=$wifi
  // Replace spaces with underscores
  urlWithParams.replace(" ", "");
  
  Serial.print("Url With Parameters : ");Serial.println(urlWithParams);
  
  // Create an HTTP object
  HTTPClient http;
  delay(1000);
  // Begin HTTP request
  http.begin(urlWithParams);

  // Send the GET request and check the HTTP response
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
//    String payload = http.getString();   // Get the response payload
//    Serial.println(payload);             // Print the response payload
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  // End HTTP request
  http.end();

  Serial.print("Folder_Name : ");Serial.println(str_date);
  Serial.print("Time_Stamp : ");Serial.println(str_time);
  check_Dir(SD, String("/"+str_date).c_str(), str_date);
  
}



void wifi_api(){
  String urlWithParams = "https://atmt.innovateitt.com/dash/2/statewifi.php?user_id=123&device_id=456";
  Serial.print("Url With Parameters : ");Serial.println(urlWithParams);
  
  // Create an HTTP object
  HTTPClient http;
  delay(1000);
  // Begin HTTP request
  http.begin(urlWithParams);

  // Send the GET request and check the HTTP response
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
//    String payload = http.getString();   // Get the response payload
//    Serial.println(payload);             // Print the response payload
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  // End HTTP request
  http.end(); 
}
