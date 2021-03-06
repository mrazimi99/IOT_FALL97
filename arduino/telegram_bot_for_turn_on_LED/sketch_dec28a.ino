#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <TelegramBot.h>  

// ----------------------------
const int myLED = 16;
bool turned_on = 0;
// ----------------------------

const char* ssid = "behzad";
//const char* password = "13781999";
const char* BotToken = "709094307:AAEwTNccHxGm0PMRfoB2Fwa6x1YDH3HVYHI";

ESP8266WebServer server(80);
WiFiClient client;
TelegramBot bot(BotToken,client);  

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "Its connected well!!!");
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
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid);
  Serial.println("");

// --------------------------
  pinMode(myLED,OUTPUT);
// --------------------------

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

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

// +++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++
}

void loop(void){
  server.handleClient();

// +++++++++++++++++++++++++++++++++++
  message m = bot.getUpdates(); // Read new messages  
 if (m.text.equals("1")) 
       {  
   digitalWrite(myLED, HIGH);  
   Serial.println("message received");  
   bot.sendMessage(m.chat_id, "The Led is now ON");  
 }  
 else if (m.text.equals("0")) 
       {  
   digitalWrite(myLED, LOW);  
   Serial.println("message received");  
   bot.sendMessage(m.chat_id, "The Led is now OFF");  
 }
// +++++++++++++++++++++++++++++++++++
}
