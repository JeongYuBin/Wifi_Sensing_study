#include <WiFi.h>

const char* ssid = "715_2.4G";       // Wi-Fi 이름
const char* password = "ilovekwu"; // Wi-Fi 비밀번호

// const char* ssid = "A234";       // Wi-Fi 이름
// const char* password = "a234a234"; // Wi-Fi 비밀번호


void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  // WiFi 연결 대기
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");

  // ESP32의 IP 주소 출력
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 아무 동작 없음
}