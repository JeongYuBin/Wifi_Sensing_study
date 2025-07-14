#include <WiFi.h>
#include <WiFiUdp.h>
#include "esp_wifi.h"

// const char* ssid = "A234";
// const char* password = "a234a234";

const char* ssid = "715_2.4G";
const char* password = "ilovekwu";

WiFiUDP Udp;
unsigned int localUdpPort = 5500;
uint8_t incomingPacket[1200];  // binary buffer

void setChannelBandwidth(bool is40MHz) {
  wifi_second_chan_t secondChan = is40MHz ? WIFI_SECOND_CHAN_ABOVE : WIFI_SECOND_CHAN_NONE;
  esp_wifi_set_channel(4, secondChan);    // 채널 수정할 부분
}

unsigned long lastReceivedTime = 0;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected as Server");

  setChannelBandwidth(false);    // 수정할 부분
  Udp.begin(localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize >= 4) {  // 최소한 4바이트(패킷 번호) 있어야 함
    int len = Udp.read(incomingPacket, 1200);

    // 패킷 번호 추출 (앞 4바이트)
    uint32_t packetNum = ((uint32_t)incomingPacket[0] << 24) |
                         ((uint32_t)incomingPacket[1] << 16) |
                         ((uint32_t)incomingPacket[2] << 8) |
                         ((uint32_t)incomingPacket[3]);

    unsigned long currentTime = millis();
    unsigned long delta = currentTime - lastReceivedTime;
    lastReceivedTime = currentTime;

    Serial.printf("Received packet #%lu, size: %d bytes, at %lu ms (+%lu ms)\n",
                  packetNum, len, currentTime, delta);
  }
}
