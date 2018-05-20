#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Armiarma";
const char* password = "4rmi4rm4";

IPAddress ip(192, 168, 1, 31); //set static ip
IPAddress gateway(192, 168, 1, 1); //set getteway
IPAddress subnet(255, 255, 255, 0);//set subnet

int relay1Pin =  4;//12;
int relay1Interval = 2000;

int relay2Pin =  5;//12;
int relay2Interval = 2000;

ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "I Love Jazar");
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);

  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.config(ip, gateway, subnet);
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
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/ireki", [](){
    server.send(200, "text/json","{\"status\":\"ok\",\"action\":\"ireki\"}");
    triggerRelay1();
  });
   server.on("/relay2", [](){
    server.send(200, "text/json", "{\"status\":\"ok\",\"action\":\"relay2\"}");
    triggerRelay2();
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}

void triggerRelay1(){
  digitalWrite(relay1Pin,HIGH);
  delay(relay1Interval);
  digitalWrite(relay1Pin,LOW);
}
void triggerRelay2(){
  digitalWrite(relay2Pin,HIGH);
  delay(relay2Interval);
  digitalWrite(relay2Pin,LOW);
}

