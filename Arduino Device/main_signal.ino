#include <SoftwareSerial.h>
#define redLED 10
#define greenLED 11
#define BUTTON 2
#define SSID "Pedetector"
#define PASS "bluescreen"
#define SERVER "smartsignal.dothome.co.kr"

SoftwareSerial wifiSerial(8,9);

void setup() {
  //적색, 녹색등을 출력모드로, 버튼을 입력모드로 세팅
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(BUTTON, INPUT);
  //시리얼통신 및 WiFi시리얼 시작
  Serial.begin(9600);
  wifiSerial.begin(9600);
  Serial.println("Pedetector Project by BLUESCREEN");
  wifiSerial.println("AT+RST");
  delay(2000);
  boolean connected=false; 
  for(int i=0;i<5;i++) {
    if(connectWiFi()) { 
      connected = true; 
      break; 
    } 
  } 
  if (!connected) { 
    while(1); 
  } 
  delay(5000);
  wifiSerial.println("AT+CIPMUX=0");
}

void loop() {
  //적색등 점등, 녹색등 소등
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);
  //버튼이 눌린경우 실행되는 if문
  if(digitalRead(BUTTON) == HIGH){
    //AT커맨드를 통해 서버와 TCP 통신을 통해 연결
    String cmd = "AT+CIPSTART=\"TCP\",\""; 
    cmd += SERVER; 
    cmd += "\",80"; 
    wifiSerial.println(cmd); 
    Serial.println(cmd); 
    if(wifiSerial.find("Error"))
      return;
    //서버로 전송할 요청 작성
    cmd = "GET /push_notification.php?devcode=notice HTTP/1.1\r\nHost: smartsignal.dothome.co.kr\r\nConnection: close\r\n\r\n"; 
    wifiSerial.print("AT+CIPSEND="); 
    wifiSerial.println(cmd.length()); 
    if(wifiSerial.find(">")) { 
      Serial.print(">"); 
    } else { 
      wifiSerial.println("AT+CIPCLOSE"); 
      Serial.println("connect timeout"); 
      delay(1000); 
      return; 
    } 
    //서버로 요청 전송
    wifiSerial.print(cmd); 
    delay(2000); 
    while (wifiSerial.available()) { 
      char c = wifiSerial.read(); 
      Serial.write(c); 
      if(c=='\r')
        Serial.print('\n'); 
    } 
    delay(1000); 
    while (wifiSerial.available()) { 
      char c = wifiSerial.read(); 
      Serial.write(c); 
      if(c=='\r')
        wifiSerial.print('\n'); 
    } 
    Serial.println("===="); 
    //녹색등 점등, 적색등 소등
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    delay(5000);
    //녹색등이 점등되고 5초가 지나면 점멸 시작
    //녹색등 점멸을 5회 완료시 for문 종료. Loop함수 처음으로 되돌아가 적색등 점등 및 놎색등 소등
    for(int i=0; i<6; i++){
      digitalWrite(greenLED, LOW);
      delay(500);
      digitalWrite(greenLED, HIGH);
      delay(500);
    }
  }
}

boolean connectWiFi() { 
  wifiSerial.println("AT+CWMODE=1"); 
  String cmd="AT+CWJAP=\"";
  cmd+=SSID; 
  cmd+="\",\""; 
  cmd+=PASS; 
  cmd+="\""; 
  Serial.println(cmd); 
  wifiSerial.println(cmd); 
  delay(2000); 
  if(wifiSerial.find("OK")) { 
    Serial.println("OK, Connected to WiFi."); 
    return true; 
  } else { 
    Serial.println("Can not connect to the WiFi."); 
    return false; 
  } 
}