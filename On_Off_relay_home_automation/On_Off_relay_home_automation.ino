#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<WebSocketsServer.h>

ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

uint8_t pin=0;
String page="";
char * ssid ="Netplus2229";
char * password="tanisharit@123";

 void setup()
 {
  page = "<p><a href=\"LEDOn\"><button>ON</button></a>&nbsp;<a href=\"LEDOff\"><button>OFF</button></a></p>";
  pinMode(pin,OUTPUT);
  digitalWrite(pin,HIGH);

  delay(1000);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("ip Address: ");
  Serial.print(WiFi.localIP());

  server.on("/",[](){
    server.send(200, "text/html",page); //_p because webpage store in flash memory
    });

  server.on("/LEDOn",[](){
    server.send(200,"text/html",page);
    digitalWrite(pin,LOW);
    delay(1000);
    });
    

  server.on("/LEDOff",[](){
    server.send(200,"text/html",page);
    digitalWrite(pin,HIGH);
    delay(1000);
    });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
 }

 void loop()
 {
  webSocket.loop();
  server.handleClient();
  if(Serial.available() > 0){
    char c[]={(char)Serial.read()};
    webSocket.broadcastTXT(c,sizeof(c));
  }
 }

 void webSocketEvent(uint8_t num,WStype_t type, uint8_t *payload, size_t length){
    if(type == WStype_TEXT){
      for(int i=0;i<length;i++)
        Serial.print((char)payload[i]);
        Serial.println();
        }
        
 }
