#include <Stepper.h> // Stepper.h 라이브러리 불러오기 (기본 라이브러리)
#include <SoftwareSerial.h> //시리얼통신 라이브러리 호출

Stepper step1(2048, 8, 10, 9, 11); // 모터드라이버 8, 9, 10, 11번에 연결

int blueTx=2;   //Tx (보내는핀 설정)at
int blueRx=3;   //Rx (받는핀 설정)
SoftwareSerial mySerial(blueTx, blueRx);  //시리얼 통신을 위한 객체선언

int lap = 2048; // 2048당 한바퀴(360도) 회전
boolean wakeup = false;

void setup() {
  step1.setSpeed(18); // 회전속도 (분당 18회 회전)
  Serial.begin(9600);   //시리얼모니터
  mySerial.begin(9600); //블루투스 시리얼
}

void loop() {
  if (mySerial.available()) {
    //Serial.write(mySerial.read()); //블루투스측 내용을 시리얼모니터에 출력
    wakeup = false;
    if (mySerial.read() == '1'){
      wakeup = true;
      Serial.println("1");
    } else {
      Serial.println("0");
    }
  }
  
  if (wakeup) {
    for (int i=0; i<3; i++){
      step1.step(lap); // 오른쪽으로 한바퀴 회전
      delay(1000); // 0.5초 대기
      
      wakeup = false;
    }
    while (mySerial.available()) {
      mySerial.read();
    }
  }
}
