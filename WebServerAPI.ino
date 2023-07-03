/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp32-webupdate.local/update
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>

#define LED 13

//defining the directions 
#define DOZA 5
#define STICLA 18
#define PLASTIC 19
#define DETECTED 21

//Define additional pins
// #define PICKUP 22
// #define DETECTED 23

#ifndef STASSID
// #define STASSID "Unplugged 2.4Ghz"
// #define STAPSK  "robobitch188"

#define STASSID "dlink-2B40"
#define STAPSK  "jzmml09848"
#endif

const char* host = "esp32-webupdate";
const char* ssid = STASSID;
const char* password = STAPSK;

WebServer httpServer(80);
HTTPUpdateServer httpUpdater;

void setup(void) {

  pinMode(LED, OUTPUT);
  pinMode(DOZA, OUTPUT);
  pinMode(STICLA, OUTPUT);
  pinMode(PLASTIC, OUTPUT);
  pinMode(DETECTED, OUTPUT);

  // pinMode(DETECTED, OUTPUT);
  // pinMode(PICKUP, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("WiFi failed, retrying.");
  }

  MDNS.begin(host);
  if (MDNS.begin("esp32")) {
    Serial.println("mDNS responder started");
  }

  httpServer.on("/test", []() {
    //String params = httpServer.arg(0);
    httpServer.send(200, "text/plain", "Hello and welcome to the test");
    digitalWrite(LED, HIGH);
  });

  httpServer.on("/recognized", []() {
    float doza = httpServer.arg(0).toFloat();
    float sticla = httpServer.arg(1).toFloat();
    float plastic = httpServer.arg(2).toFloat();
    float detected = httpServer.arg(3).toFloat();
    //float pickup = httpServer.arg(4).toFloat();

    analogWrite(DOZA, doza);
    analogWrite(STICLA, sticla);
    analogWrite(PLASTIC, plastic);
    analogWrite(DETECTED, detected);

    Serial.println("Recived request");

    httpServer.send(200, "text/plain", "Hello and welcome to the test");

    
  });

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop(void) {
  httpServer.handleClient();
}