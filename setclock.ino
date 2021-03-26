void setclock() {
  //输出设置闹钟前的状态
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Set:");
  if (hour < 10 && hour > 0)lcd.print('0');
  lcd.print(hour);
  lcd.print(':');
  if (minute < 10 && minute > 0)lcd.print('0');
  lcd.print(minute);

  //设置闹钟
  int hou = hour, minu = minute, count = 0;
  unsigned long int tim = millis();
  oldConfirmState = millis();
  String str = "";
  char ch;
  while (count < 4) {
    if (count == 0) ch = hou / 10 + '0';
    else if (count == 1) ch = hou % 10 + '0';
    else if (count == 2) ch = minu / 10 + '0';
    else if (count == 3) ch = minu % 10 + '0';
    lcd.setCursor(7 , 1);
    if (count < 2)
      for (int i = 0; i < count; i++)lcd.print(' ');
    else for (int i = 0; i < count + 1; i++)lcd.print(' ');
    lcd.write(0);
    while (1) {
      //中键下一位
      confirmState = digitalRead(confirmPin);
      if (confirmState == LOW) {
        if (millis() - oldConfirmState > 500) {
          str += ch;
          tim = millis();
          break;
        }
      }

      //旋转编码
      currentStateCLK = digitalRead(CLK);
      if (currentStateCLK != lastStateCLK  && currentStateCLK == HIGH) {
        if (count < 2)lcd.setCursor(7 + count, 0);
        else lcd.setCursor(8 + count, 0);
        if (digitalRead(DT) != currentStateCLK) {
          if (ch < '9') {
            ch++;
            tim = millis();
            lcd.print(ch);
          }
        }
        else {
          if (ch > '0') {
            ch--;
            tim = millis();
            lcd.print(ch);
          }
        }
      }
      lastStateCLK = currentStateCLK;
      if (millis() - tim > 10000) {
        count = 4;
        break;
      }
    }
    count++;
    oldConfirmState = millis();
  }

  lcd.clear();
  lcd.setCursor(8, 0);
  lcd.write(7);
  lcd.setCursor(6, 1);
  hou = (str.charAt(0) - '0') * 10 + str.charAt(1) - '0';
  minu = (str.charAt(2) - '0') * 10 + str.charAt(3) - '0';
  if (count == 5)lcd.print("EXIT!");
  else if (str.length() == 4 && hou >= 0 && hou <= 24 && minu >= 0 && minu <= 60) {
    lcd.print("DONE!");
    flag = true;
    if (hou == 24)hour = 0;
    else hour = hou;
    minute = minu;
  }
  else lcd.print("ERROR!");
  delay(1500);
  lcd.clear();
}
