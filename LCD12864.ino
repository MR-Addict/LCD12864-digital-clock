#include <dht11.h>  //get tempeature
#include <U8glib.h>  //drive lcd12864
#include <DS1302.h>  //get accurate time
#include <EEPROM.h>  //write data to EEPROM

byte hour, minute;
const int BuzzerPin = 2;
const int TemPin =  3;
const int RS = 4;
const int WR = 5;
const int EN = 6;
const int RST = 7;
const int DAT = 8;
const int CLOCK = 9;
const int CLK = 10;
const int ConfPin = 11;
const int DT = 12;
boolean currentStateCLK;
boolean lastStateCLK;
boolean isPressed = true;
boolean isAlarm;
unsigned long int LastTime;
const int menuItems = 4;
const int ClockWidth = 96;
const int ClockHeight = 32;
const char *myMenus[] = {"Back", "Set Alarm", "Alarm On/Off", "Booting up Cartoons"};
const char *myDays[] =  {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const unsigned char Ring[] U8G_PROGMEM = {
  0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x7E, 0x18, 0x00
};  //the ring icon
const unsigned char UpArrow[] U8G_PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0xC0, 0x03, 0xE0, 0x07, 0x80, 0x01, 0x80, 0x01,
  0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00
};  //the uparrow icon
const unsigned char AlarmClock[] U8G_PROGMEM = {
  0xF0, 0x01, 0x80, 0x0F, 0x1C, 0x07, 0xE0, 0x38, 0x06, 0x0C, 0x30, 0x60, 0x02, 0x0C, 0x30, 0x40,
  0x03, 0x07, 0xE0, 0xC0, 0xC1, 0xF9, 0x9F, 0x83, 0x71, 0x8E, 0x71, 0x8E, 0x1B, 0x83, 0xC1, 0xD8,
  0xC6, 0x00, 0x00, 0x63, 0x40, 0x00, 0x00, 0x06, 0x20, 0x00, 0x40, 0x04, 0x30, 0x00, 0x60, 0x0C,
  0x10, 0x00, 0x30, 0x08, 0x18, 0x00, 0x18, 0x18, 0x08, 0x00, 0x0C, 0x10, 0x08, 0x00, 0x04, 0x10,
  0x08, 0x00, 0x06, 0x10, 0x38, 0x00, 0x03, 0x1C, 0xB8, 0xFF, 0x01, 0x1C, 0x08, 0x00, 0x00, 0x10,
  0x08, 0x00, 0x00, 0x10, 0x18, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x18, 0x10, 0x00, 0x00, 0x08,
  0x20, 0x00, 0x00, 0x04, 0x60, 0x00, 0x00, 0x06, 0xC0, 0x00, 0x00, 0x03, 0x80, 0x81, 0x81, 0x01,
  0x00, 0x86, 0x61, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x80, 0x01, 0x80, 0x01, 0xC0, 0x00, 0x00, 0x03,
};  //the bell icon
const unsigned char arduinoLogo[] U8G_PROGMEM = {
  0x00, 0xE0, 0x07, 0x00, 0x00, 0xC0, 0x07, 0x00, 0x00, 0xFC, 0x1F, 0x00, 0x00, 0xF8, 0x3F, 0x00,
  0x00, 0xFE, 0x7F, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xC1,
  0x80, 0xFF, 0xFF, 0x03, 0x80, 0xFF, 0xFF, 0xC3, 0xC0, 0xFF, 0xFF, 0x03, 0xC0, 0xFF, 0xFF, 0x03,
  0xE0, 0xFF, 0xFF, 0x07, 0xE0, 0xFF, 0xFF, 0x07, 0xF0, 0xFF, 0xFF, 0x0F, 0xF0, 0xFF, 0xFF, 0x0F,
  0xF0, 0x0F, 0xF8, 0x1F, 0xF8, 0x1F, 0xF0, 0x0F, 0xF8, 0x07, 0xE0, 0x1F, 0xF8, 0x07, 0xE0, 0x1F,
  0xF8, 0x03, 0xC0, 0x3F, 0xFC, 0x03, 0xC0, 0x1F, 0xFC, 0x01, 0x80, 0x7F, 0xFC, 0x01, 0x80, 0x3F,
  0xFC, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x3F, 0xFC, 0x00, 0x00, 0xFE, 0xFE, 0x00, 0x00, 0x7E,
  0x7E, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0x7E, 0x7E, 0x00, 0x00, 0xFC, 0x3F, 0x00, 0x00, 0x7E,
  0x3E, 0x00, 0x00, 0xF8, 0x3F, 0xC0, 0x01, 0x7C, 0x3E, 0x00, 0x00, 0xF8, 0x1F, 0xC0, 0x01, 0x7C,
  0x3E, 0x00, 0x00, 0xF0, 0x0F, 0xC0, 0x01, 0xFC, 0x3F, 0x00, 0x00, 0xF0, 0x0F, 0xC0, 0x01, 0xFC,
  0x3F, 0xF0, 0x1F, 0xE0, 0x0F, 0xF8, 0x0F, 0xF8, 0x3F, 0xF0, 0x1F, 0xE0, 0x07, 0xF8, 0x0F, 0xF8,
  0x3F, 0xF0, 0x1F, 0xE0, 0x07, 0xF8, 0x0F, 0xF8, 0x3F, 0xF0, 0x1F, 0xE0, 0x07, 0xF8, 0x0F, 0xF8,
  0x3F, 0x00, 0x00, 0xF0, 0x0F, 0xF8, 0x0F, 0xF8, 0x3E, 0x00, 0x00, 0xF0, 0x0F, 0xC0, 0x01, 0xFC,
  0x3E, 0x00, 0x00, 0xF8, 0x1F, 0xC0, 0x01, 0xFC, 0x3E, 0x00, 0x00, 0xF8, 0x1F, 0xC0, 0x01, 0x7C,
  0x3E, 0x00, 0x00, 0xFC, 0x3F, 0xC0, 0x01, 0x7C, 0x7E, 0x00, 0x00, 0xFC, 0x7F, 0x00, 0x00, 0x7E,
  0x7E, 0x00, 0x00, 0xFE, 0x7F, 0x00, 0x00, 0x7E, 0xFC, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x3F,
  0xFC, 0x01, 0x80, 0x7F, 0xFE, 0x01, 0x00, 0x3F, 0xF8, 0x01, 0x80, 0x3F, 0xFC, 0x03, 0x80, 0x3F,
  0xF8, 0x03, 0xC0, 0x3F, 0xF8, 0x07, 0xC0, 0x1F, 0xF8, 0x0F, 0xF0, 0x1F, 0xF8, 0x0F, 0xF0, 0x1F,
  0xF0, 0x3F, 0xFC, 0x0F, 0xF0, 0x3F, 0xFC, 0x0F, 0xE0, 0xFF, 0xFF, 0x0F, 0xE0, 0xFF, 0xFF, 0x07,
  0xE0, 0xFF, 0xFF, 0x07, 0xC0, 0xFF, 0xFF, 0x07, 0xC0, 0xFF, 0xFF, 0x03, 0xC0, 0xFF, 0xFF, 0x03,
  0x80, 0xFF, 0xFF, 0x01, 0x80, 0xFF, 0xFF, 0x01, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 0x00,
  0x00, 0xFC, 0x3F, 0x00, 0x00, 0xFC, 0x7F, 0x00, 0x00, 0xF8, 0x0F, 0x00, 0x00, 0xF0, 0x1F, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x60, 0xF8, 0xF1, 0x63, 0xCC, 0xCF, 0x18, 0x1F, 0x70, 0xF8, 0xF3, 0x63, 0xCC, 0xCF, 0x99, 0x3F,
  0xF0, 0x18, 0x33, 0x67, 0x0C, 0xC3, 0x9B, 0x31, 0xD8, 0x18, 0x33, 0x67, 0x0C, 0xC3, 0x9B, 0x31,
  0xD8, 0x18, 0x33, 0x66, 0x0C, 0xC3, 0x9F, 0x31, 0xD8, 0xB8, 0x33, 0x66, 0x0C, 0xC3, 0x9E, 0x31,
  0x98, 0xF9, 0x31, 0x66, 0x0C, 0xC3, 0x9E, 0x31, 0xF8, 0xF9, 0x31, 0x66, 0x0C, 0xC3, 0x9C, 0x31,
  0xF8, 0x99, 0x31, 0x67, 0x0C, 0xC3, 0x9C, 0x31, 0x98, 0x99, 0x31, 0xE3, 0x0E, 0xC3, 0x9C, 0x3B,
  0x8C, 0x19, 0xF3, 0xE3, 0xC7, 0xCF, 0x18, 0x1F, 0x8C, 0x19, 0xF3, 0xC1, 0xC7, 0xCF, 0x18, 0x0E,
};

dht11 DHT11;
U8GLIB_ST7920_128X64 u8g(EN, WR, RS, U8G_PIN_NONE);
DS1302 rtc(RST, DAT, CLOCK);

void bootingUp() {
  for (int i = -30; i < 32; i ++) {
    u8g.firstPage();
    do {
      u8g.drawXBMP( i, 0, 64, 64, arduinoLogo);
    } while ( u8g.nextPage() );
    delay(1);
  }
  delay(1000);
}

//draw the emulated clock hands
void DrawHand(float time, float steps, int radius)
{
  double angle = time * 2.0 * 3.1415 / steps - 1;
  double X = ClockWidth + radius * cos(angle);
  double Y = ClockHeight + radius * sin(angle);
  u8g.drawLine(ClockWidth, ClockHeight, X, Y);
}

//print main UI
void PrintWin() {
  u8g.firstPage();
  do {
    //print date
    Time time = rtc.time();
    DHT11.read(TemPin);
    u8g.setColorIndex(1);
    u8g.drawRFrame(0, 0, 128, 64, 4);
    u8g.drawRFrame(68, 4, 57, 57, 3);
    if (isAlarm)u8g.drawXBMP( 58, 26, 8, 8, Ring);
    if (isAlarm == true && time.hr == hour && time.min == minute) {
      u8g.drawXBMP( 18, 25, 32, 32, AlarmClock);
      tone(BuzzerPin, 300);
      if (!digitalRead(ConfPin)) {
        isAlarm = !isAlarm;
        noTone(BuzzerPin);
        LastTime = millis();
      }
    }
    else {
      u8g.setFont(u8g_font_5x8);
      u8g.setPrintPos(7, 55);
      u8g.print(time.yr);
      u8g.drawStr( 29, 55, "/");
      u8g.setPrintPos(36, 55);
      u8g.print(time.mon);
      u8g.drawStr( 43, 55, "/");
      u8g.setPrintPos(51, 55);
      u8g.print(time.date);
      u8g.setFont(u8g_font_6x13);
      u8g.setPrintPos(10, 41);
      u8g.print(myDays[time.day - 1]);
      u8g.setPrintPos(33, 41);
      u8g.print(DHT11.temperature);
      u8g.print(char(176));
      u8g.print('C');
    }

    //print time
    u8g.drawRBox(3, 4, 62, 21, 2);
    u8g.setColorIndex(0);
    u8g.setFont(u8g_font_fub17);
    u8g.drawStr(29, 21, ":");
    int position;
    if (time.hr < 10)
    {
      u8g.drawStr(3, 23, "0");
      position = 16;
    }
    else position = 3;
    u8g.setPrintPos(position, 23);
    u8g.print(time.hr);
    if (time.min < 10)
    {
      u8g.drawStr(38, 23, "0");
      position = 51;
    }
    else position = 38;
    u8g.setPrintPos(position , 23);
    u8g.print(time.min);
    u8g.setColorIndex(1);

    //print emulated clock
    u8g.drawCircle(ClockWidth, ClockHeight, 26);
    u8g.drawCircle(ClockWidth, ClockHeight, 1);
    u8g.setFont(u8g_font_6x13);
    u8g.drawStr(90, 17, "12");
    u8g.drawStr(114, 37, "3");
    u8g.drawStr(94, 56, "6");
    u8g.drawStr(72, 37, "9");
    DrawHand(time.hr - 1, 12.0, 10);
    DrawHand(time.min - 5, 60.0, 19);
    DrawHand(time.sec - 5, 60, 21);
  } while ( u8g.nextPage() );
}

void AlarmOnOff() {
  isAlarm = !isAlarm;
  EEPROM.write(0, isAlarm);
  u8g.firstPage();
  do {
    u8g.setFontPosTop();
    u8g.setFont(u8g_font_unifont);
    u8g.drawRFrame(0, 0, 128, 64, 4);
    u8g.drawRBox(27, 22, u8g.getStrWidth("Finished!") + 3, 17, 2);
    u8g.setColorIndex(0);
    u8g.drawStr(30, 24, "Finished!");
    u8g.setColorIndex(1);
  } while ( u8g.nextPage() );
  delay(1000);
}

//print setting clock UI
void printSetClock(int timeArrey[], int position, boolean isChoice) {
  u8g.firstPage();
  do {
    u8g.setFontPosTop();
    u8g.setFont(u8g_font_fub17);
    u8g.drawRFrame(0, 0, 128, 64, 4);
    u8g.setPrintPos(25 + 16 * 2 , 26);
    u8g.print(':');
    for (int i = 0; i < 4; i++) {
      if (i < 2)
        u8g.setPrintPos(25 + 16 * i, 28);
      else
        u8g.setPrintPos(20 + 16 * (i + 1), 28);
      u8g.print(timeArrey[i]);
    }
    if (position < 4) {
      u8g.setFont(u8g_font_5x8);
      u8g.drawStr(45, 55, "Confirm");
      u8g.drawStr(90, 55, "Cancle");
    }
    else if (position == 4 && isChoice) {
      u8g.setFont(u8g_font_5x8);
      u8g.drawRBox(43, 47, u8g.getStrWidth("Confirm") + 4, 10, 2);
      u8g.setColorIndex(0);
      u8g.drawStr(45, 55, "Confirm");
      u8g.setColorIndex(1);
      u8g.drawStr(90, 55, "Cancle");
    }
    else if (position == 4 && !isChoice) {
      u8g.setFont(u8g_font_5x8);
      u8g.drawRBox(88, 47, u8g.getStrWidth("Cancle") + 4, 10, 2);
      u8g.setColorIndex(0);
      u8g.drawStr(90, 55, "Cancle");
      u8g.setColorIndex(1);
      u8g.drawStr(45, 55, "Confirm");
    }
    if (position < 2)
      u8g.drawXBMP( 25 + 16 * position, 30, 16, 16, UpArrow);
    else if (position < 4)
      u8g.drawXBMP( 20 + 16 * (position + 1), 30, 16, 16, UpArrow);
  } while ( u8g.nextPage() );
}

//set clock
void SetClock() {
  LastTime = millis();
  int hour1 = hour / 10, hour2 = hour % 10, minute1 = minute / 10, minute2 = minute % 10;
  int timeArrey[] = {hour1, hour2, minute1, minute2};
  int position = 0;
  boolean isChoice = true;
  while (1) {
    isPressed = digitalRead(ConfPin);
    if (!isPressed && millis() - LastTime > 500) {
      position ++;
      LastTime = millis();
      if (position == 5) {
        int hou = timeArrey[0] * 10 + timeArrey[1];
        int minu = timeArrey[2] * 10 + timeArrey[3];
        if (hou < 24 && minu < 60 && isChoice) {
          isAlarm = true;
          hour = hou;
          minute = minu;
          EEPROM.write(0, isAlarm);//write data to EEPROM
          EEPROM.write(1, hour);
          EEPROM.write(2, minute);
          u8g.firstPage();
          do {
            u8g.setFontPosTop();
            u8g.setFont(u8g_font_unifont);
            u8g.drawRFrame(0, 0, 128, 64, 4);
            u8g.drawRBox(27, 22, u8g.getStrWidth("Finished!") + 3, 17, 2);
            u8g.setColorIndex(0);
            u8g.drawStr(30, 24, "Finished!");
            u8g.setColorIndex(1);
          } while ( u8g.nextPage() );
        }
        else {
          u8g.firstPage();
          do {
            u8g.setFontPosTop();
            u8g.setFont(u8g_font_unifont);
            u8g.drawRFrame(0, 0, 128, 64, 4);
            u8g.drawRBox(27, 22, u8g.getStrWidth("Cancled!") + 3, 17, 2);
            u8g.setColorIndex(0);
            u8g.drawStr(30, 24, "Cancled!!");
            u8g.setColorIndex(1);
          } while ( u8g.nextPage() );
        }
        delay(1000);
        break;
      }
    }
    currentStateCLK = digitalRead(CLK);
    if (currentStateCLK != lastStateCLK) {
      if (digitalRead(DT) != currentStateCLK)
        if (position < 4) {
          if (timeArrey[position] > 0)
            timeArrey[position]--;
          else if (position == 0 && timeArrey[position] < 2)
            timeArrey[position]++;
          else if (position == 1 && timeArrey[position] < 9)
            timeArrey[position]++;
          else if (position == 2 && timeArrey[position] < 6)
            timeArrey[position]++;
          else if (position == 3 && timeArrey[position] < 9)
            timeArrey[position]++;
        }
        else {
          isChoice = !isChoice;
        }
    }
    lastStateCLK = currentStateCLK;
    printSetClock(timeArrey, position, isChoice);
  }
}

//print menu UI
void PrintMenu(int count) {
  u8g.firstPage();
  u8g.setFontPosTop();
  do {
    u8g.setFont(u8g_font_6x13);
    u8g.drawRFrame(0, 0, 128, 64, 4);
    u8g.drawRBox(8, 3 + 14 * count, u8g.getStrWidth(myMenus[count]) + 3, 15, 2);
    u8g.setColorIndex(0);
    u8g.drawStr(10, 4 + 14 * count, myMenus[count]);
    u8g.setColorIndex(1);
    for (int i = 0; i < menuItems; i++) {
      if (i != count)
        u8g.drawStr(10, 4 + 14 * i, myMenus[i]);
    }
  }
  while ( u8g.nextPage() );
}

//select menu
void SelectMenu() {
  int count = 0;
  while (1) {
    isPressed = digitalRead(ConfPin);
    if (!isPressed && millis() - LastTime > 500) {
      LastTime = millis();
      switch (count) {
        case 0: ; break;
        case 1: SetClock(); break;
        case 2: AlarmOnOff(); break;
        case 3: bootingUp(); break;
      }
      break;
    }
    else {
      currentStateCLK = digitalRead(CLK);
      if (currentStateCLK != lastStateCLK  && currentStateCLK == LOW) {
        if (digitalRead(DT) != currentStateCLK) {
          if (count > 0)
            count--;
        }
        else if (count < 3)
          count++;
      }
      PrintMenu(count);
      lastStateCLK = currentStateCLK;
    }
  }
}

void setup() {
  pinMode(CLK, INPUT_PULLUP);
  pinMode(ConfPin, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  isAlarm = (boolean)EEPROM.read(0);
  hour = EEPROM.read(1);
  minute = EEPROM.read(2);
  u8g.setFontPosTop();
  LastTime = millis();
  noTone(BuzzerPin);
  bootingUp();
}

void loop() {
  isPressed = digitalRead(ConfPin);
  if (!isPressed && millis() - LastTime > 500) {
    LastTime = millis();
    SelectMenu();
  }
  else
    PrintWin();
}
