void bootUp() {
  //滚动的"Hello world"
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0, 0);
    for (int j = 0; j < i; j++)
      lcd.print(' ');
    lcd.setCursor(i, 0);
    lcd.print("Welcome to");
    delay(500);
  }
  delay(800);
  for (int i = 15; i >= 0; i--) {
    lcd.setCursor(i + 5, 1);
    for (int j = 15; j > i; j--)
      lcd.print(' ');
    lcd.setCursor(i, 1);
    lcd.print("my arduino clock");
    delay(500);
  }
  delay(800);
  lcd.clear();

  //打印鸭子
  delay(800);
  lcd.setCursor(7, 0);
  for (int i = 7; i < 16; i++) {
    lcd.write(7);
    delay(100);
  }
  for (int i = 15; i >= 0; i--) {
    lcd.setCursor(i, 1);
    lcd.write(7);
    delay(100);
  }
  lcd.setCursor(0, 0);
  for (int i = 0; i < 7; i++) {
    lcd.write(7);
    delay(100);
  }
  delay(800);
  lcd.clear();

  //打印"LOADING......"
  delay(800);
  lcd.setCursor(4, 0);
  lcd.print("LOADING");
  delay(500);
  for (int i = 0; i < 3; i++) {
    lcd.setCursor(9, 1);
    for (int j = 0; j < 6; j++) {
      lcd.print('.');
      delay(400);
    }
    delay(800);
    lcd.setCursor(9, 1);
    lcd.print("      ");
    if (i < 2)delay(800);
  }
  lcd.clear();
  delay(800);
}
