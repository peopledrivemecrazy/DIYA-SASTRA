#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "DUNEBUGGY";
const char* password = "godofwar";

ESP8266WebServer server(80);

int motorpins[] = {D4, D5, D6, D7}; //motor1 = D4,D5 motor2 = D6,D7

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    Serial.println( server.arg(i));
  }
  server.send(404, "text/plain", message);
}

void handleDirection() {
  String dir = server.arg(0);
  String steps = server.arg(1);
  String message = "going ";
  message += dir;
  message += " ";
  message += steps;
  message += " steps";
  Serial.println(message);
  server.send(200, "text/plain", message);
  handleMotors(dir, steps.toInt());
}

void handleMotors(String dir, int steps) {
  steps = steps * 1000;
  if (dir == "forward") {
    moveForward(steps);
  }
  if (dir == "backward") {
    moveBackward(steps);
  }
  if (dir == "left") {
    moveLeft(steps);
  }
  if (dir == "right") {
    moveRight(steps);
  }
  if (dir == "stop") {
    stopBot();
  }
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  for (int i = 0; i < 4; i++) {
    pinMode(motorpins[i], OUTPUT);
    digitalWrite(motorpins[i], LOW);
  }

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/direction", handleDirection);

  server.on("/inline", []() {
    server.send(200, "text / plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}

void moveForward(int steps) {
  digitalWrite(D4, HIGH);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  delay(steps);
  stopBot();
}

void moveBackward(int steps) {
  digitalWrite(D4, LOW);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
  delay(steps);
  stopBot();
}

void moveRight(int steps) {
  digitalWrite(D4, HIGH);
  digitalWrite(D5, LOW);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, LOW);
  delay(steps);
  stopBot();
}
void moveLeft(int steps) {
  digitalWrite(D4, HIGH);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, HIGH);
  delay(steps);
  stopBot();
}
void stopBot() {
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);
}
