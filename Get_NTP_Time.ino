void get_time(){
    // Update NTP client
  timeClient.update();

  // Retrieve epoch time
  unsigned long epochTime = timeClient.getEpochTime();

  // Convert epoch time to human-readable format
  char formattedTime[30];
  strcpy(formattedTime, ctime((const time_t *)&epochTime));

  // Print date and time
  Serial.print("Date and Time: ");
  
  time_str = String(formattedTime);
  Serial.println(time_str);
  str_date = time_str.substring(0,10)+ "_" + time_str.substring(20,24);
  str_date.replace(" ", "_");
  Serial.print("Date : ");Serial.println(str_date);
  
  str_time = time_str.substring(11,19);
  Serial.println();
  String hrs = str_time.substring(0,2);
  Serial.print("Time in hrs: ");Serial.println(hrs);
  String mins = str_time.substring(3,5);
  Serial.print("Time in mins: ");Serial.println(mins);

  t_mins = hrs.toInt()*60 + mins.toInt();
  Serial.print("Total time in mins: ");Serial.println(t_mins);
}
