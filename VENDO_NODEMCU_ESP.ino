#include <ESP8266WiFi.h>
#include <ESP8266Firebase.h>
#include <SoftwareSerial.h>

#define _SSID "C25s"         // Your WiFi SSID
#define _PASSWORD "123456789" // Your WiFi Password
#define REFERENCE_URL ""  // Your Firebase project reference URL

// #define rx RX

Firebase firebase(REFERENCE_URL);
SoftwareSerial wifiUART(4, 5);

void setup() {
  Serial.begin(9600);
  wifiUART.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void sendToFirebase(const char* data) {
  firebase.pushString("IVenTaMo", data);
}

void loop() {
  
  if (wifiUART.available() == 2) {
    byte parameterOneHbyte = wifiUART.read();
    byte parameterOneLbyte = wifiUART.read();
    int parameterOne = (parameterOneHbyte << 8) | (parameterOneLbyte);
    Serial.println(parameterOne);
    Serial.println("read parameter 1");

    String message = "Unknown";
    switch (parameterOne) {
      case 1: message = "IDLace Button Pressed"; break;
      case 2: message = "Necktie Button Pressed"; break;
      case 3: message = "Pin Button Pressed"; break;
      case 4: message = "Booklet Pressed"; break;
      case 5: message = "Coin Accepted"; break;
    }
    
    sendToFirebase(message.c_str());
  }
}