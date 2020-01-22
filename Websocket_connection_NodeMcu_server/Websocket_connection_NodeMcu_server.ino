#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<WebSocketsServer.h>

ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

uint8_t pin_led=16;
char * ssid ="Netplus2229";
char * password="tanisharit@123";

char webpage[] PROGMEM = R"=====(
<html>
<head>
  <script>
    var Socket;
    function init(){
       Socket = new WebSocket('ws://' +window.location.hostname+':81/');
       Socket.onmessage =function(event){
          document.getElementById("rxconsole").value+=event.data;
       }
    }
    function sendText(){
      Socket.send(document.getElementById("txBar").value);
      document.getElementById("txBar").value ="";
    }

    function sendBrightness(){
      Socket.send("#"+document.getElementById("brightness").value);
      
    }
  </script>
</head>
<body onload="javascript:init()">
<div>
<textarea id="rxconsole"></textarea>
</div>
<div>
<input type="text" id="txBar" onkeydown="if(event.keyCode== 13) sendText();"/>
</div>

<div>
<input type="range" min="0" max="1023" value="512" id="brightness" oninput="sendBrightness()"/>
</div>
<body>
</html>
)=====";

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

  server.on("/",[](){
    server.send_P(200, "text/html",webpage); //_p because webpage store in flash memory
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

      if(payload[0]=='#')
      {
         uint16_t brightness = (uint16_t) strtol((const char *) &payload[1],NULL,10);
         brightness =1024- brightness;
         analogWrite(pin_led,brightness);
         Serial.print("brightness= ");
         Serial.println(brightness);
       }
      else
      {
      for(int i=0;i<length;i++)
        Serial.print((char)payload[i]);
        Serial.println();}
    }
 }
