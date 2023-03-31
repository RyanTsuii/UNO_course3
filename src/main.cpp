#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "Login.h"
#include "Logo.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

#define FMD_PIN 2
#define ECHO    3
#define TIRG    4

// 创建 SSD1306 实例
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup() {
    // 引脚设置
    pinMode(FMD_PIN, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(TIRG, OUTPUT);
    // 初始化串口
    Serial.begin(9600, SERIAL_8E2);
    // 初始化 SSD1306
    if (!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println("failed");
        for (;;);
    } else {
        Serial.print("Hello");
    }
    // 字体设置
    display.setTextColor(WHITE);
    display.setTextSize(2);
    // 显示姓名学号
    display.clearDisplay();
    display.drawBitmap(0, 0, gImage_Login, 128, 64, WHITE);
    display.display();
    delay(1500);
    // 显示logo
    display.clearDisplay();
    display.drawBitmap(26, 0, gImage_logo, 77, 64, WHITE);
    display.display();
    delay(1500);

}

// 超声波模块发出发出指令信号
void HC_SR04_start() {
    digitalWrite(TIRG, LOW);
    delayMicroseconds(2);
    digitalWrite(TIRG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TIRG, LOW);
}
// 计算距离
float get_dis() {
    HC_SR04_start();
    float time = float(pulseIn(ECHO, HIGH));
    float dis = time * 0.017;
    return dis;
}

void loop() {

    float dis = get_dis();
    // 串口发送距离信息
    Serial.print("dis:");
    Serial.println(dis);
    // 显示距离数值大小
    display.clearDisplay();
    display.setCursor(0,10);
    display.print(dis, 1);
    display.println("cm");
    // 图形化显示距离
    display.drawRect(0, 33, 128, 12, WHITE);
    display.fillRect(2, 35, map(dis, 2, 200, 2, 124), 8, WHITE);
    display.display();
    // 距离小于 50 cm 时，启动蜂鸣器报警
    if(dis < 50) {
        tone(FMD_PIN, 882, 1000);
    }

    delay(2000);

}