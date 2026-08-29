
#include <LiquidCrystal.h>

// 初始化LCD引脚 (RS, E, D4, D5, D6, D7)
// 请根据实际接线修改此处引脚号
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

unsigned long startTime;
unsigned long elapsedTime;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Timer Started");
  startTime = millis();
}

void loop() {
  // 计算经过的时间（毫秒）
  elapsedTime = millis() - startTime;

  // 转换为时、分、秒
  int seconds = (elapsedTime / 1000) % 60;
  int minutes = (elapsedTime / (1000 * 60)) % 60;
  int hours = (elapsedTime / (1000 * 60 * 60));

  // 设置光标到第二行开头
  lcd.setCursor(0, 1);
  
  // 格式化打印时间 HH:MM:SS
  if (hours < 10) lcd.print("0");
  lcd.print(hours);
  lcd.print(":");
  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);
  
  // 短暂延时以减少屏幕刷新闪烁，同时保证计时精度影响极小
  delay(100);
}
