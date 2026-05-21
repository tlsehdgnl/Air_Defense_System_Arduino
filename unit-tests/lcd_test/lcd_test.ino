/*
 * [단위 테스트] LCD 디스플레이 (I2C, 16x2)
 * -----------------------------------------------
 * 팀명 : 뚝@딱
 * 조원 : 신동휘, 윤태웅, 이종현
 * -----------------------------------------------
 * 핀 연결 (I2C):
 *   VCC → 5V
 *   GND → GND
 *   SDA → A4
 *   SCL → A5
 * -----------------------------------------------
 * 동작 설명:
 *   I2C 통신을 이용하여 LCD 화면에
 *   "Radar System" 과 "Module Test OK" 문구를 출력한다.
 *   백라이트가 켜지는지, 글자가 깨지지 않고
 *   선명하게 나오는지 확인한다.
 * -----------------------------------------------
 * 주의: I2C 주소가 0x27이 아닐 경우 0x3F로 변경 필요
 * 테스트 결과: 정상 동작 확인
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // 주소 확인 필요 (0x27 or 0x3F)

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Radar System");

  lcd.setCursor(0, 1);
  lcd.print("Module Test OK");
}

void loop() {
  // 표시 유지 (별도 동작 없음)
}
