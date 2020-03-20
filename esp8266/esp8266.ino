#include <ArduinoJson.h>

#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "Ground floor"
#define STAPSK  "KA36kavinagar"
#define FIREBASE_HOST "sf-cads.firebaseio.com"
#define FIREBASE_AUTH "ZPp4FDBftxIYDaCk4iBLeb8gW0XaMt3k32UHRkVN"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int DATA0 = 4; // D2
const int DATA1 = 5; // D1

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
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

void setup(void) {
  pinMode(DATA0, OUTPUT);
  pinMode(DATA1, OUTPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP().toString());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("ip", WiFi.localIP().toString());
  while (!Firebase.success()) {
    delay(1000);
    Serial.println("Retrying: Writing IP to Firebase.");
    Firebase.setString("ip", WiFi.localIP().toString());
  }
  Serial.println("Success: IP written to Firebase.");
  server.on("/halt", []() {
    digitalWrite(DATA0, LOW);
    digitalWrite(DATA1, LOW);
    Serial.println("Halt.\n");
    server.send(200, "text/plain", "Halt.");
  });
  server.on("/forward", []() {
    digitalWrite(DATA0, HIGH);
    digitalWrite(DATA1, HIGH);
    Serial.println("Forward.\n");
    server.send(200, "text/plain", "Forward.");
  });
  server.on("/right", []() {
    digitalWrite(DATA0, HIGH);
    digitalWrite(DATA1, LOW);
    Serial.println("Right.\n");
    server.send(200, "text/plain", "Right.");
  });
  server.on("/left", []() {
    digitalWrite(DATA0, LOW);
    digitalWrite(DATA1, HIGH);
    Serial.println("Left.\n");
    server.send(200, "text/plain", "Left.");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}
void loop(void) {
  server.handleClient();
  MDNS.update();
}
