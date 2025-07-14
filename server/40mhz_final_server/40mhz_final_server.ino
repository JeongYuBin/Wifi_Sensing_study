#include <WiFi.h>
#include <WiFiUdp.h>
#include "esp_wifi.h"

const char* ssid = "715_2.4G";       // Wi-Fi 이름
const char* password = "ilovekwu"; // Wi-Fi 비밀번호

WiFiUDP Udp;
unsigned int localUdpPort = 5500;
uint8_t incomingPacket[1200];

unsigned long lastReceivedTime = 0;

void setChannelBandwidth40MHz() {
  esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT40);                 // 40MHz
  esp_wifi_set_channel(4, WIFI_SECOND_CHAN_ABOVE);                   // 채널 8
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected as Server");

  setChannelBandwidth40MHz();
  Udp.begin(localUdpPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize >= 4) {
    int len = Udp.read(incomingPacket, 1200);

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
