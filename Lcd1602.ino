#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS1302.h>
#define ONE_WIRE_BUS 8
#define buzzerPin 12
#define confirmPin 11
#define CLK 9
#define DT 10

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DS1302 rtc(5, 6, 7);
boolean flag; //是否闹铃
boolean confirmState;
unsigned long oldConfirmState;
boolean currentStateCLK;
boolean lastStateCLK;
byte arrow[] = {0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x00, 0x00};
byte clock1[] = {0x00, 0x00, 0x01, 0x01, 0x02, 0x04, 0x04, 0x04};
byte clock2[] = {0x00, 0x00, 0x11, 0x1F, 0x00, 0x04, 0x04, 0x04};
byte clock3[] = {0x00, 0x00, 0x10, 0x10, 0x08, 0x04, 0x04, 0x04};
byte clock4[] = {0x04, 0x04, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00};
byte clock5[] = {0x07, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00};
byte clock6[] = {0x14, 0x04, 0x04, 0x04, 0x08, 0x10, 0x00, 0x00};
byte duck[]  = {0x00, 0x06, 0x17, 0x1E, 0x1E, 0x0C, 0x00, 0x05};
int hour = 14, minute = 16; //闹铃初始化

void setup() {
  pinMode(13, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(confirmPin, INPUT_PULLUP);
  digitalWrite(buzzerPin, HIGH);
  lcd.init();
  lcd.backlight();
  lcd.noCursor();
  lcd.createChar(0, arrow);
  lcd.createChar(1, clock1);
  lcd.createChar(2, clock2);
  lcd.createChar(3, clock3);
  lcd.createChar(4, clock4);
  lcd.createChar(5, clock5);
  lcd.createChar(6, clock6);
  lcd.createChar(7, duck);
  lastStateCLK = digitalRead(CLK);
  oldConfirmState = millis();
  sensors.begin();
  Serial.begin(9600);
  bootUp();
}

void loop() {
  printMenu();
}
