#include <WiFi.h>
#include <WiFiUdp.h>
#include "esp_wifi.h"

// const char* ssid = "A234";
// const char* password = "a234a234";

const char* ssid = "715_2.4G";
const char* password = "ilovekwu";

WiFiUDP Udp;
IPAddress IP_Remote(192, 168, 0, 89);
unsigned int remotePort = 5500;

// 설정값(수정할 부분)
int dataSize = 100;  // 1, 100, 1000
int sendDelay = 100;  // 50, 100, 1000

void setChannelBandwidth(bool is40MHz) {
  wifi_second_chan_t secondChan = is40MHz ? WIFI_SECOND_CHAN_ABOVE : WIFI_SECOND_CHAN_NONE;
  esp_wifi_set_channel(4, secondChan);    // 채널 수정할 부분
}

uint32_t packetCount = 0;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected as Client");

  setChannelBandwidth(false);    // (false -20 / true -40)
  Udp.begin(5400);
}

void loop() {
  static uint8_t data[1004];  // 앞 4바이트는 packet 번호
  packetCount++;

  // 패킷 번호를 앞에 삽입 (4바이트)
  data[0] = (packetCount >> 24) & 0xFF;
  data[1] = (packetCount >> 16) & 0xFF;
  data[2] = (packetCount >> 8) & 0xFF;
  data[3] = packetCount & 0xFF;

  // 나머지 데이터
  for (int i = 4; i < dataSize + 4; i++) {
    data[i] = i % 256;
  }

  Udp.beginPacket(IP_Remote, remotePort);
  Udp.write(data, dataSize + 4);  // 총 전송 크기: 데이터 + 번호(4)
  Udp.endPacket();

  Serial.printf("Sent packet #%lu\n", packetCount);
  delay(sendDelay);
}
