void printTime_Tem()
{
  lcd.setCursor(7, 0);
  if (flag)lcd.print("ON ");
  else lcd.print("OFF");

  //打印时间
  Time tim = rtc.time();
  lcd.setCursor(4, 1);
  if (tim.hr < 10 && tim.hr > 0)lcd.print('0');
  lcd.print(tim.hr);
  lcd.print(':');
  if (tim.min < 10 && tim.min > 0)lcd.print('0');
  lcd.print(tim.min);
  lcd.print(':');
  if (tim.sec < 10 && tim.sec > 0)lcd.print('0');
  lcd.print(tim.sec);

  //判断是否闹铃
  if (tim.hr == hour && tim.min == minute && flag == true) {
    digitalWrite(buzzerPin, LOW);
    unsigned long Tim = millis();
    lcd.clear();
    lcd.setCursor(7, 0);
    for (int i = 1; i <= 3; i++)
      lcd.write(i);
    lcd.setCursor(7, 1);
    for (int i = 4; i <= 6; i++)
      lcd.write(i);
    while (millis() - Tim <= 60000) {//闹铃一分钟
      confirmState = digitalRead(confirmPin);
      if (confirmState == LOW) {
        if (millis() - oldConfirmState > 500) {
          flag = false;
          break;
        }
        oldConfirmState = millis();
      }
    }
    digitalWrite(buzzerPin, HIGH);
    lcd.clear();
    oldConfirmState = millis();
  }
}
