#include <SoftwareSerial.h>
#define redLED 10
#define greenLED 11
#define BUTTON 2
#define SSID "Pedetector"
#define PASS "bluescreen"
#define SERVER "smartsignal.dothome.co.kr"

SoftwareSerial wifiSerial(8,9);

void setup() {
  //����, ������� ��¸���, ��ư�� �Է¸��� ����
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(BUTTON, INPUT);
  //�ø������ �� WiFi�ø��� ����
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
  //������ ����, ����� �ҵ�
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, HIGH);
  //��ư�� ������� ����Ǵ� if��
  if(digitalRead(BUTTON) == HIGH){
    //ATĿ�ǵ带 ���� ������ TCP ����� ���� ����
    String cmd = "AT+CIPSTART=\"TCP\",\""; 
    cmd += SERVER; 
    cmd += "\",80"; 
    wifiSerial.println(cmd); 
    Serial.println(cmd); 
    if(wifiSerial.find("Error"))
      return;
    //������ ������ ��û �ۼ�
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
    //������ ��û ����
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
    //����� ����, ������ �ҵ�
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, HIGH);
    delay(5000);
    //������� ����ǰ� 5�ʰ� ������ ���� ����
    //����� ������ 5ȸ �Ϸ�� for�� ����. Loop�Լ� ó������ �ǵ��ư� ������ ���� �� ������ �ҵ�
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