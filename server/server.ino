#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "A234";       // Wi-Fi 이름
const char* password = "a234a234"; // Wi-Fi 비밀번호

const unsigned int localPort = 4210;
char incomingPacket[255];

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi.");

  udp.begin(localPort);
  Serial.printf("UDP server listening on port %d\n", localPort);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0; // Null-terminate
    }
    unsigned long recvTime = millis();
    Serial.printf("Received: \"%s\" at %lu ms\n", incomingPacket, recvTime);

    // 받은 패킷 그대로 응답
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write((uint8_t*)incomingPacket, strlen(incomingPacket));
    udp.endPacket();
  }
}
