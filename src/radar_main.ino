/*
 * radar_main.ino
 * -----------------------------------------------
 * 프로젝트 : Arduino Radar System
 * 팀명 : 뚝@딱
 * 조원 : 신동휘, 윤태웅, 이종현
 * -----------------------------------------------
 * 사용 모듈:
 *   1. LCD 디스플레이 (I2C, 16x2)
 *   2. 서보모터 (SG90)
 *   3. 초음파 거리 센서 (HC-SR04)
 * -----------------------------------------------
 * 핀 연결
 *
 * [LCD I2C]
 *   VCC → 5V
 *   GND → GND
 *   SDA → A4
 *   SCL → A5
 *
 * [서보모터]
 *   VCC    → 5V
 *   GND    → GND
 *   Signal → D9
 *
 * [초음파 센서]
 *   VCC  → 5V
 *   GND  → GND
 *   Trig → D11
 *   Echo → D10
 * -----------------------------------------------
 * 동작 설명:
 *   - 서보모터가 0도 ~ 180도까지 회전하며 스캔 수행
 *   - 초음파 센서로 거리 측정
 *   - LCD에 현재 각도와 거리 출력
 *   - 시리얼 모니터에도 거리 출력
 * -----------------------------------------------
 * 주의:
 *   LCD 주소가 작동하지 않으면
 *   0x27 → 0x3F 로 변경 필요
 * -----------------------------------------------
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// =========================
// LCD 설정
// =========================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// =========================
// 서보모터 설정
// =========================
Servo myServo;

// =========================
// 초음파 센서 핀 설정
// =========================
#define TRIG 11
#define ECHO 10

// =========================
// 거리 측정 함수
// =========================
long getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);

  long distance = duration * 0.034 / 2;

  return distance;
}

// =========================
// 초기 설정
// =========================
void setup() {

  // 시리얼 통신 시작
  Serial.begin(9600);

  // LCD 시작
  lcd.init();
  lcd.backlight();

  // 시작 화면 출력
  lcd.setCursor(0, 0);
  lcd.print("Radar System");

  lcd.setCursor(0, 1);
  lcd.print("Initializing");
  delay(2000);

  lcd.clear();

  // 서보모터 연결
  myServo.attach(9);

  // 초음파 센서 핀 설정
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

// =========================
// 메인 반복
// =========================
void loop() {

  // 0도 → 180도 스캔
  for (int angle = 0; angle <= 180; angle += 10) {

    myServo.write(angle);

    delay(200);

    long distance = getDistance();

    // 시리얼 출력
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" deg | Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // LCD 출력
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Angle: ");
    lcd.print(angle);

    lcd.setCursor(0, 1);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm");
  }

  // 180도 → 0도 스캔
  for (int angle = 180; angle >= 0; angle -= 10) {

    myServo.write(angle);

    delay(200);

    long distance = getDistance();

    // 시리얼 출력
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" deg | Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // LCD 출력
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Angle: ");
    lcd.print(angle);

    lcd.setCursor(0, 1);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm");
  }
}