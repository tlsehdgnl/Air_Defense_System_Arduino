/*
 * [단위 테스트] 초음파 거리 센서 (HC-SR04)
 * -----------------------------------------------
 * 팀명 : 뚝@딱
 * 조원 : 신동휘, 윤태웅, 이종현
 * -----------------------------------------------
 * 핀 연결:
 *   VCC  → 5V
 *   GND  → GND
 *   Trig → 디지털 11번
 *   Echo → 디지털 10번
 * -----------------------------------------------
 * 동작 설명:
 *   초음파를 발사하고 반사되어 돌아오는 시간을 측정하여
 *   물체와의 거리를 cm 단위로 시리얼 모니터에 출력한다.
 *   50cm 이내의 거리값이 정상적으로 들어오는지 확인한다.
 * -----------------------------------------------
 * 테스트 결과: 정상 동작 확인
 */

#define TRIG 11
#define ECHO 10

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  long distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
