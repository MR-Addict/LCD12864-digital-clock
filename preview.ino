void preview() {
  //打印温度
  sensors.requestTemperatures();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Temp:");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.print(char(223));
  lcd.print('C');

  //打印闹铃状态和具体日期
  lcd.setCursor(1, 1);
  Time tim = rtc.time();
  if (tim.yr < 10 && tim.yr > 0)lcd.print('0');
  lcd.print(tim.yr);
  lcd.print('-');
  if (tim.mon < 10 && tim.mon > 0)lcd.print('0');
  lcd.print(tim.mon);
  lcd.print('-');
  if (tim.date < 10 && tim.date > 0)lcd.print('0');
  lcd.print(tim.date);
  lcd.print('-');
  switch (tim.day) {
    case 1: lcd.print("SUN"); break;
    case 2: lcd.print("MON"); break;
    case 3: lcd.print("TUE"); break;
    case 4: lcd.print("WEN"); break;
    case 5: lcd.print("THU"); break;
    case 6: lcd.print("FRI"); break;
    case 7: lcd.print("STA"); break;
  }
  unsigned long int Tim = millis();
  oldConfirmState = millis();
  while (millis() - Tim <= 10000) {
    confirmState = digitalRead(confirmPin);
    if (confirmState == LOW) {
      if (millis() - oldConfirmState > 500) {
        oldConfirmState = millis();
        break;
      }
    }
  }
  lcd.clear();
}
