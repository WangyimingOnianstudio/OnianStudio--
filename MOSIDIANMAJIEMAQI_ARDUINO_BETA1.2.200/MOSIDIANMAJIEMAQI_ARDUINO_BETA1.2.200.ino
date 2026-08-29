#include <LiquidCrystal.h>

// LCD引脚定义: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int BUTTON_PIN = 6;
const unsigned long LONG_PRESS_TIME = 1000; // 长按阈值 1000ms
const unsigned long MORSE_TIMEOUT = 1500;    // 摩斯码翻译延迟 200ms
const unsigned long DEBOUNCE_DELAY = 200;   // 按键防抖延迟 200ms

// 按键状态变量
bool lastButtonState = HIGH;      
bool stableButtonState = HIGH;    // 防抖后的稳定按键状态
unsigned long lastDebounceTime = 0; // 记录最后一次按键状态改变的时间

// 长按与摩斯码状态变量
unsigned long buttonPressTime = 0; 
bool buttonActive = false;        
bool longPressActive = false;     
String currentMorse = "";         // 当前正在输入的摩斯码
String decodedMessage = "";       // 已解码并确认的字符
unsigned long lastInputTime = 0;  // 记录最后一次按键释放的时间

// 摩斯密码字典 (字母和数字)
const char* morseCodes[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",   // A-I
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // J-R
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",        // S-Z
  "-----", ".----", "..---", "...--", "....-", ".....",            // 0-5
  "-....", "--...", "---..", "----."                               // 6-9
};
const char* charMap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// 查找摩斯密码对应的字符
char decodeMorse(String morse) {
  for (int i = 0; i < 36; i++) {
    if (morse == morseCodes[i]) {
      return charMap[i];
    }
  }
  return '\0'; 
}

// 更新LCD显示
void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(decodedMessage);
  lcd.setCursor(0, 1);
  lcd.print(currentMorse);
}

void setup() {
  lcd.begin(16, 2);
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  updateDisplay();
}

void loop() {
  // 1. 读取按键的原始状态
  int reading = digitalRead(BUTTON_PIN);

  // 2. 如果按键状态发生了改变（发生了抖动），重置防抖计时器
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // 3. 只有当状态改变后，经过了防抖延迟，我们才认为按键状态是稳定的
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // 如果稳定的状态与上一次记录的状态不同，说明按键确实被按下或松开了
    if (reading != stableButtonState) {
      stableButtonState = reading;

      // === 真正的按键按下 (下降沿) ===
      if (stableButtonState == LOW) {
        buttonPressTime = millis();
        buttonActive = true;
        longPressActive = false; 
      }
      // === 真正的按键释放 (上升沿) ===
      else if (stableButtonState == HIGH) {
        buttonActive = false;
        
        // 如果是短按（没有触发长按），则记录为点
        if (!longPressActive) {
          currentMorse += ".";
          updateDisplay();
        }
        
        // 无论长短按，松开时必须重置长按标志位！
        longPressActive = false; 
        
        // 【核心修改】：在松开按键时，才记录时间并启动翻译超时倒计时
        lastInputTime = millis(); 
      }
    }
  }

  // 4. 长按检测（基于稳定的状态和防抖后的按下时间）
  if (stableButtonState == LOW && buttonActive) {
    unsigned long pressDuration = millis() - buttonPressTime;
    if (pressDuration >= LONG_PRESS_TIME && !longPressActive) {
      longPressActive = true;
      currentMorse += "-"; 
      updateDisplay();
    }
  }

  // 5. 摩斯码翻译超时检测
  // 如果当前有摩斯码输入，且距离上次【松开按键】已经超过了设定的延迟时间
  if (currentMorse.length() > 0 && (millis() - lastInputTime >= MORSE_TIMEOUT)) {
    char decodedChar = decodeMorse(currentMorse);
    if (decodedChar != '\0') {
      decodedMessage += decodedChar;
      currentMorse = "";
      updateDisplay();
    } else {
      lcd.setCursor(0, 1);
      lcd.print("ERROR!SV.1.2.B23      "); 
      delay(300);                 
      currentMorse = "";
      updateDisplay();
    }
  }

  // 6. 更新上一次的原始按键状态
  lastButtonState = reading;
}
