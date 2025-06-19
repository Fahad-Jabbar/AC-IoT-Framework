void led_blinking(int pin, int bl_loop, int d){
  for (int bl=0; bl<=bl_loop; bl++){
   digitalWrite(pin,0);
   delay(d);
   digitalWrite(pin,1);
   delay(d);
  }
}
