void printMenu() {
  if (confirmState == LOW) {
    if (millis() - oldConfirmState > 500) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(7);
      lcd.print(" Pre Date&Temp");
      lcd.setCursor(2, 1);
      lcd.print("Alarm ON/OFF");
      int count = 0;
      unsigned long int Tim = millis();
      oldConfirmState = millis();
      while (1) {
        confirmState = digitalRead(confirmPin);
        if (confirmState == LOW) {
          if (millis() - oldConfirmState > 500) {
            if (count == 0)preview();
            else if (count == 1) {
              flag = !flag;
              lcd.clear();
              lcd.setCursor(8, 0);
              lcd.write(7);
              lcd.setCursor(4, 1);
              if (flag)lcd.print("Alarm ON!");
              else lcd.print("Alarm OFF!");
              delay(2000);
            }
            else if (count == 2)setclock();
            break;
          }
        }

        currentStateCLK = digitalRead(CLK);
        if (currentStateCLK != lastStateCLK  && currentStateCLK == HIGH) {
          if (digitalRead(DT) != currentStateCLK && count < 3) {
            count++;
            if (count == 1) {
              lcd.setCursor(0, 1); lcd.write(7);
              lcd.setCursor(0, 0); lcd.print(' ');
            }
            else if (count == 2) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("  Alarm ON/OFF");
              lcd.setCursor(0, 1);
              lcd.write(7);
              lcd.print(" Set alarm");
            }
            else if (count == 3) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("  Set alarm");
              lcd.setCursor(0, 1);
              lcd.write(7);
              lcd.print(" Home");
            }
          }
          else if (digitalRead(DT) == currentStateCLK && count > 0) {
            count--;
            if (count == 0) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.write(7);
              lcd.print(" Pre Date&Temp");
              lcd.setCursor(0, 1);
              lcd.print("  Alarm ON/OFF");
            }
            else if (count  == 1) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.write(7);
              lcd.print(" Alarm ON/OFF");
              lcd.setCursor(0, 1);
              lcd.print("  Set alarm");
            }
            else if (count == 2) {
              lcd.setCursor(0, 0); lcd.write(7);
              lcd.setCursor(0, 1); lcd.print(' ');
            }
          }
          Tim = millis();
        }
        lastStateCLK = currentStateCLK;
        if (millis() - Tim > 10000)break;
      }
      lcd.clear();
      oldConfirmState = millis();
    }
  }
  else printTime_Tem();
  confirmState = digitalRead(confirmPin);
}
