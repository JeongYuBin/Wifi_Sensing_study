#include <WiFi.h>
#include <WiFiUdp.h>
#include "esp_wifi.h"

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WiFiUDP Udp;
IPAddress IP_Remote(192, 168, 0, 0);  // 서버 IP 주소
unsigned int remotePort = 5500;

// ⚙️ 실험 설정값
int dataSize = 100;         // 변경: 1, 100, 1000
int sendDelay = 10;         // 변경: 1000, 100, 10(ms)

void setChannelBandwidth(bool is40MHz) {
  wifi_second_chan_t secondChan = is40MHz ? WIFI_SECOND_CHAN_ABOVE : WIFI_SECOND_CHAN_NONE;
  esp_wifi_set_channel(4, secondChan);  // 채널 4 사용 예시
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected as Client");

  setChannelBandwidth(true); // true: 40MHz, false: 20MHz
  Udp.begin(5400);  // 송신기 포트
}

void loop() {
  static uint8_t data[1000];  // 최대 크기 1000으로 확보
  for (int i = 0; i < dataSize; i++) {
    data[i] = i % 256;
  }

  Udp.beginPacket(IP_Remote, remotePort);
  Udp.write(data, dataSize);
  Udp.endPacket();

  delay(sendDelay);
}
