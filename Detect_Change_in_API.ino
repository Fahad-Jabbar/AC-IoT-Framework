void detect_change(){
//  max_temp, min_temp, on_time, off_time, mode_val, thr_val, st;
  if (mode_val.toInt()!=1){
    mode2_on_off();
    if ((max_temp!=max_temp_old)||(min_temp!=min_temp_old)||(on_time!= on_time_old)||(off_time!= off_time_old)||(thr_val!= thr_val_old)){      
      change = 1;
      Serial.print("Change Detected : ");Serial.println(change);
      max_temp_old = max_temp;
      min_temp_old = min_temp;
      on_time_old = on_time;
      off_time_old = off_time, 
      thr_val_old = thr_val;

      update_ac_mode2();
    }
    else{
      Serial.print("No Change Detected!");
    }
  }
  else{
    default_mode();
  }
}
