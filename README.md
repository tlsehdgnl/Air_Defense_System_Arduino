# 📡 물체 감지 레이더 시스템 (뚝딱 레이더)

> **아두이노와 초음파 센서, 서보 모터를 활용하여 주변 환경을 0°~180° 왕복 스캔하고, 침입 물체를 실시간으로 탐지하여 시각·청각적 경고를 제공하는 소형 방공망/보안 시스템 모델입니다.**
> 본 프로젝트는 임베디드 시스템 설계, 센서 데이터 처리 및 소프트웨어 모듈화(함수 구조화)를 깊이 있게 학습하기 위해 진행된 팀 프로젝트입니다.

---

# 🛠 1. 프로젝트 개요

- **프로젝트명**: 뚝딱 레이더 (물체 감지 레이더망 시스템)

- **개발 인원**: 3인 (팀 프로젝트)

  - **신동휘 (팀장/PM)**  
    프로젝트 총괄, 하드웨어 및 소프트웨어 설계, 자료 제작 및 보고서 작성, 발표 진행

  - **이종현 (하드웨어)**  
    하드웨어 설계 및 디자인, 동작 환경 구성, 촬영 및 자료 제작, 발표 진행

  - **윤태웅 (소프트웨어)**  
    소프트웨어 아키텍처 설계, 스케치 코드 작성 및 디버깅, 발표 진행

---

## 🚀 주요 기능

### 🔹 왕복 스캐닝 (Dual-direction Sweeping)
서보 모터를 이용해 0도부터 180도까지 정방향 및 역방향으로 반복 회전하며 주변 환경을 탐색합니다.

### 🔹 실시간 거리 측정 (Ultrasonic Sensing)
초음파 센서(HC-SR04)를 통해 각도별 물체와의 거리를 cm 단위로 계산하며, 최대 약 400cm 범위까지 탐지합니다.

### 🔹 실시간 경보 시스템
설정된 임계 거리 이내로 물체가 접근하면:
- LCD 경고 문구 출력
- LED 점등
- 피에조 스피커 경고음 발생

을 통해 사용자에게 즉시 경고를 제공합니다.

---

# 🔌 2. 하드웨어 구성 및 회로 (Hardware Components)

## 📦 사용 부품 및 핀 맵 (Pin Mapping)

| 부품 | 역할 | 핀 연결 |
|------|------|------|
| Arduino Uno | 메인 제어 보드 | - |
| HC-SR04 초음파 센서 | 거리 측정 | Trig: D11 / Echo: D10 |
| SG90 서보 모터 | 센서 회전 스캔 | Signal: D9 |
| I2C LCD 16x2 | 거리 및 상태 표시 | SDA/SCL |
| LED | 시각 경고 | 디지털 핀 |
| Piezo Buzzer | 청각 경고 | 디지털 핀 |
| 브레드보드 & 저항 | 회로 안정화 | 5V / GND 공유 |

---

# 📐 3. 소프트웨어 아키텍처 (Software Architecture)

본 프로젝트는 유지보수성과 가독성을 높이기 위해 기능별 모듈화(Function Modularization)를 기반으로 설계되었습니다.

---

## ⚙️ 주요 함수 구조

### 🔹 `setup()`
- 핀 모드 초기화
- 시리얼 통신 시작
- LCD 및 서보 모터 초기화
- 시스템 시작 메시지 출력

---

### 🔹 `loop()`
- `0° → 180°`
- `180° → 0°`

방향으로 반복 스캔하며 각 위치마다 `radarScan(angle)` 실행

---

### 🔹 `radarScan(int angle)`
레이더 동작의 핵심 통합 처리 함수

#### 내부 수행 작업
1. 서보 각도 이동
2. 거리 측정
3. LCD 정보 출력
4. 경고 시스템 제어

---

### 🔹 `calculateDistance()`
- 초음파 발사
- 반사 시간 측정
- 거리 계산
- 비정상 데이터 필터링 수행

---

### 🔹 `displayLCD(angle, dist)`
현재 각도 및 거리 정보를 LCD에 실시간 출력

---

### 🔹 `checkAlert(dist)`
임계 거리 이내 접근 시:
- LED 활성화
- 부저 경고음 출력
- LCD 경고 문구 표시

---

# 💻 4. 소프트웨어 핵심 로직 (Code Structure)

```cpp
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 핀 및 임계값 정의
#define TRIG 11
#define ECHO 10
#define SERVO_PIN 9
#define ALERT_DIST 20 // 임계 거리(cm)

Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 초기 설정
void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  myServo.attach(SERVO_PIN);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Radar System");

  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  delay(2000);
  lcd.clear();
}

// 메인 루프
void loop() {

  // 정방향 스캔
  for (int angle = 0; angle <= 180; angle++) {
    radarScan(angle);
    delay(30);
  }

  // 역방향 스캔
  for (int angle = 180; angle >= 0; angle--) {
    radarScan(angle);
    delay(30);
  }
}

// 레이더 스캔 함수
void radarScan(int angle) {

  myServo.write(angle);

  int distance = calculateDistance();

  Serial.print("Angle: ");
  Serial.print(angle);

  Serial.print(", Distance: ");
  Serial.println(distance);

  displayLCD(angle, distance);

  checkAlert(distance);
}

// 거리 계산 함수
int calculateDistance() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);

  long distance = duration * 0.034 / 2;

  // 비정상 데이터 처리
  if (distance > 400 || distance <= 0) {
    return 999;
  }

  return distance;
}

// LCD 출력 함수
void displayLCD(int angle, int dist) {

  lcd.setCursor(0, 0);
  lcd.print("Ang: ");
  lcd.print(angle);
  lcd.print("deg   ");

  lcd.setCursor(0, 1);

  if(dist < ALERT_DIST) {
    lcd.print("!! DETECTED !!  ");
  }
  else {
    lcd.print("Dist: ");
    lcd.print(dist);
    lcd.print("cm    ");
  }
}

// 경고 시스템 함수
void checkAlert(int dist) {

  if (dist < ALERT_DIST) {

    // LED ON
    // Buzzer ON

  } else {

    // LED OFF
    // Buzzer OFF

  }
}
```

---

# 📈 5. 프로젝트 성과 및 배운 점 (Retrospective)

## 🔹 소프트웨어 모듈화 및 구조적 설계
기능을 독립적인 함수(`calculateDistance`, `displayLCD`, `checkAlert`)로 완전히 분리하여 구현했습니다. 이를 통해 메인 루프(`loop`)의 복잡도를 낮추고 코드의 가독성을 확보하여 디버깅 및 팀원 간 협업 프로세스를 효율화했습니다.

---

## 🔹 예외 처리 및 데이터 신뢰성 확보
초음파 센싱 과정에서 환경적 요인으로 발생하는 노이즈나 비정상 데이터(400cm 초과 등)를 소프트웨어 조건문으로 필터링(`999cm` 예외 처리)하여 시스템의 불필요한 오작동을 차단하고 신뢰성을 극대화했습니다.

---

## 🔹 H/W 및 S/W 통합 제어 역량 강화
센서 입력값(거리)의 변화에 따라 출력 장치(LCD 문구, LED 점등, 피에조 경고음)와 구동 모터가 유기적인 시퀀스로 연동되도록 하드웨어 인터페이싱과 타이밍 제어를 설계하며 임베디드 시스템 개발 흐름을 깊이 있게 이해할 수 있었습니다.

---

# 📊 6. 시스템 동작 흐름도

```text
[Servo Sweep]
      ↓
[Distance Measure]
      ↓
[Distance Filtering]
      ↓
 ┌─────────────┐
 │ Distance < 20cm ? │
 └──────┬──────┘
        │ YES
        ↓
[LED ON + Buzzer ON + LCD Alert]
        │
        NO
        ↓
[Normal Monitoring]
```

---

# 🧩 7. 개발 환경

| 항목 | 내용 |
|------|------|
| 개발 보드 | Arduino Uno |
| 개발 언어 | Arduino C/C++ |
| IDE | Arduino IDE |
| 사용 라이브러리 | Servo, Wire, LiquidCrystal_I2C |
| 운영 환경 | Embedded System |

---

# ✅ 8. 기대 효과

- 임베디드 시스템 설계 경험 습득
- 센서 데이터 처리 및 필터링 학습
- 하드웨어-소프트웨어 인터페이싱 이해
- 실시간 제어 시스템 구현 경험
- 팀 기반 프로젝트 협업 능력 향상

---

# 📄 License

This project is developed for educational and academic purposes.
