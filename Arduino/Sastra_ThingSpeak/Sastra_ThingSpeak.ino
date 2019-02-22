#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


const char *ssid = "Sange & Yasha";
const char *password = "awesomegod321";

//Web/Server address to read/write from
String host = "http://api.thingspeak.com/update?api_key=";
String key = "UZJTW2NLW76FXRTN";
int a;
unsigned long previousMillis = 0;

const long interval = 15000;
HTTPClient http;


void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
void sendData() {
  String Link = host + key + "&field1=" + String(a);
  http.begin(Link);
  Serial.println(Link);
  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
}
void loop() {
  a += 1;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendData();
  }
}
