#include <WiFi.h>
#include <WiFiUdp.h>
#include "esp_wifi.h"

// ===== 전역 변수 선언 =====
const char* ssid = "715_2.4G";
const char* password = "ilovekwu";

WiFiUDP Udp;
IPAddress IP_Remote(192, 168, 0, 89); // Server IP
unsigned int remotePort = 5500;

int dataSize = 100;     // 전송할 데이터 크기
int sendDelay = 100;    // 전송 주기 (ms)

uint32_t packetCount = 0; // 전송한 패킷 수

// ===== 40MHz 설정 함수 =====
void setChannelBandwidth40MHz() {
  esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT40);                 
  esp_wifi_set_channel(4, WIFI_SECOND_CHAN_ABOVE); // 채널 8
}

// ===== setup =====
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected as Client");

  setChannelBandwidth40MHz();
  Udp.begin(5400);
}

// ===== loop =====
void loop() {
  static uint8_t data[1004];
  static uint32_t lastSentTime = 0;

  packetCount++;

  // 패킷 번호 삽입
  data[0] = (packetCount >> 24) & 0xFF;
  data[1] = (packetCount >> 16) & 0xFF;
  data[2] = (packetCount >> 8) & 0xFF;
  data[3] = packetCount & 0xFF;

  // 데이터 채우기
  for (int i = 4; i < dataSize + 4; i++) {
    data[i] = i % 256;
  }

  // UDP 패킷 전송
  Udp.beginPacket(IP_Remote, remotePort);
  Udp.write(data, dataSize + 4);
  Udp.endPacket();

  // 시간 측정 및 출력
  unsigned long now = millis();

  Serial.printf("Sent packet #%lu at %lu ms\n", packetCount, now);

  delay(sendDelay);
}
