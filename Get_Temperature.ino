void temperature_loop(){
  Serial.println("/////////////////// Internal Temperature ////////////////////");
  int_temp.requestTemperatures(); 
  float temperatureC = int_temp.getTempCByIndex(0);
//  Temp_internal = temperatureC;
  In_temp = String(temperatureC);
  float temperatureF = int_temp.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  delay(500);

  Serial.println("/////////////////// External Temperature ////////////////////");
  ext_temp.requestTemperatures(); 
  float temperature2C = ext_temp.getTempCByIndex(0);
//  Temp_external = temperature2C;
  Out_temp = String(temperature2C);
  float temperature2F = ext_temp.getTempFByIndex(0);
  Serial.print(temperature2C);
  Serial.println("ºC");
  Serial.print(temperature2F);
  Serial.println("ºF");
  delay(500);
}
