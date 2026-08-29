This is a code for Arduino but no 8051.
LCD1602 引脚	连接到 Arduino	备注说明
1 (VSS)	GND	电源负极
2 (VDD)	5V	电源正极
3 (V0)	电位器中间脚	调节对比度（电位器两端分别接 5V 和 GND）
4 (RS)	Pin 12	对应代码 LiquidCrystal lcd(12, 11, 5, 4, 3, 2)
5 (RW)	GND	必须接地，表示“写入模式”
6 (EN)	Pin 11	使能引脚
11 (D4)	Pin 5	数据位 4
12 (D5)	Pin 4	数据位 5
13 (D6)	Pin 3	数据位 6
14 (D7)	Pin 2	数据位 7
15 (A)	5V	屏幕背光正极（可串联 220Ω 电阻防过亮）
16 (K)	GND	屏幕背光负极
