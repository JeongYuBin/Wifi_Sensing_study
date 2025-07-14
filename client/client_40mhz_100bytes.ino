#include <WiFi.h>
#include <WiFiUdp.h>
#include "esp_wifi.h"

const char* ssid = "715_2.4G";
const char* password = "ilovekwu";

WiFiUDP Udp;
IPAddress IP_Remote(192, 168, 0, 89);  // 서버 ESP32 IP 주소
unsigned int localUdpPort = 5500;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT40);  // 40MHz 설정

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected.");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  Udp.begin(localUdpPort);
}

void loop() {
  // 100바이트 데이터 준비 + 앞에 timestamp 포함
  uint8_t data[100];
  unsigned long sendTime = millis();
  memcpy(data, &sendTime, sizeof(sendTime)); // 앞 4바이트에 timestamp 기록

  for (int i = sizeof(sendTime); i < 100; i++) {
    data[i] = i;
  }

  Udp.beginPacket(IP_Remote, 5500);
  Udp.write(data, 100);
  Udp.endPacket();

  // 응답 대기
  unsigned long startWait = millis();
  while (!Udp.parsePacket()) {
    if (millis() - startWait > 1000) {
      Serial.println("No response");
      return;
    }
  }

  uint8_t recvBuf[100];
  int len = Udp.read(recvBuf, 100);
  unsigned long recvTime = millis();

  unsigned long originalSendTime;
  memcpy(&originalSendTime, recvBuf, sizeof(originalSendTime));

  unsigned long rtt = recvTime - originalSendTime;

  Serial.printf("Packet RTT: %lu ms (sendTime: %lu, recvTime: %lu)\n", rtt, originalSendTime, recvTime);

  delay(1000); // 1초 간격
}
