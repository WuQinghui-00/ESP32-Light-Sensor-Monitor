#include <Wire.h>//I2C通信库
#include <LiquidCrystal_I2C.h>//I2C LCD驱动库

//初始化LCD
LiquidCrystal_I2C lcd(0x27,16,2);

//定义光敏传感器D0引脚连接的GPIO
const int doPin = 4;
const int ledPin = 5;

//变量，用于记录上一次的状态，避免频繁刷新LCD
int lastSensorState = -1;
int currentSensorState =0;
unsigned long lastChangeTime =0;


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(doPin,INPUT);//将引脚设置为输入模式，用于读取数字信号
pinMode(ledPin,OUTPUT);
digitalWrite(ledPin,LOW);


//初始化lcd
lcd.init();
lcd.backlight();//打开背光
lcd.clear();//清屏
//显示静态标题
lcd.setCursor(0,0);
lcd.print("Light Monitor");
lcd.setCursor(0,1);
lcd.print("State:");

Serial.println("系统启动。传感器信号正常，开始监控状态变化");


}

void loop() {
  // put your main code here, to run repeatedly:
currentSensorState = digitalRead(doPin);
digitalWrite(ledPin,currentSensorState);

if(currentSensorState != lastSensorState){
  lastChangeTime = millis();
  lastSensorState = currentSensorState;//更新记录的状态

//更新lcd第二行显示
lcd.setCursor(6,1);
if(currentSensorState == HIGH){
  lcd.print("Dark,ON");
}else{
  lcd.print("Bright,OFF");
}
Serial.print("【状态变化】时间: ");
    Serial.print(millis());
    Serial.print("ms | 新状态: ");
    Serial.print(currentSensorState);
    Serial.print(" (");
    Serial.print(currentSensorState == HIGH ? "暗/HIGH" : "亮/LOW");
    Serial.print(") | 已更新LCD显示: ");
    Serial.println(currentSensorState == HIGH ? "Dark, ON" : "Bright, OFF");
  }
  

delay(100);
}
