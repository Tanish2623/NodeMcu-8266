#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

ESP8266WebServer server;
uint8_t pin_led=16;
char * ssid ="Netplus2229";
char * password="tanisharit@123";

 void setup()
 {
  pinMode(pin_led,OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print("");
    delay(500);
  }
  Serial.println("");
  Serial.print("ip Address: ");
  Serial.print(WiFi.localIP());

  server.on("/",HTTP_GET,handleLED);
  server.on("/toggle", HTTP_POST ,toggleLED);
  server.begin();
 }

 void loop()
 {
  server.handleClient();
 }

void toggleLED()
{
  digitalWrite(pin_led,!digitalRead(pin_led));
  server.send(204,"");
}

void handleLED(){
  server.send(200, "text/html", "<form action=\"/toggle\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}
