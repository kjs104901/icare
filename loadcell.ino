#include <Stepper.h> // Stepper.h 라이브러리 불러오기 (기본 라이브러리)
#include <SoftwareSerial.h> //시리얼통신 라이브러리 호출
#include "hx711.h"

Hx711 scale(A1, A0);

int blueTx=2;   //Tx (보내는핀 설정)at
int blueRx=3;   //Rx (받는핀 설정)
SoftwareSerial mySerial(blueTx, blueRx);  //시리얼 통신을 위한 객체선언

void setup() {
  Serial.begin(9600);   //시리얼모니터
  mySerial.begin(9600); //블루투스 시리얼
}

void loop() {
  //Serial.print(scale.getGram()*4, 1);
  //Serial.println(" g");
  
  mySerial.println(scale.getGram()*4);
  
  delay(5000);
}
