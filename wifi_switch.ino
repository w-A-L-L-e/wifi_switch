/*
  Wifi Web Light Switch v1 beta

  This allows to switch on lights over the wifi. When board first boots it starts as access point. You can connect to it with browser by joining
  its created wifi network. It then allows selecting wifi network + set user + pass in the browser. After this it boots and shows a webpage
  that allows toggling a relay on/off in the browser.
  
  Board: Wemos D1 mini clone. Use https://arduino.esp8266.com/stable/package_esp8266com_index.json
  in board manager in configuration and then select LOLIN Wemos d1 clone (my super cheap board has 4mb but we only use less than 360kb for this sketch.)
  Serial speed: 74880

  Libraries used:
  https://github.com/tzapu/WiFiManager
  https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer

  Author: Walter Schreppers
*/

#include <WiFiManager.h>
#include <ESP8266WebServer.h>

WiFiManager wifiManager;
ESP8266WebServer server(80);

// Right now we use the internal led, soon we'll configure a gpio pin and toggle a relay here...
// when device powered, by default turn on relay

int relayPin1 = LED_BUILTIN;
bool relayState1 = true;

String SendHTML(uint8_t relay_on){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;background-color: #000000;} h1 {color: #0d60db;margin: 50px auto 30px;} h3 {color: #0d60db;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Wifi light switch</h1>\n";
  ptr += "<p>Wemos D1 ESP8266 Web Server.</p>";
  ptr += "<p>Board ID: 0001</p>";
  ptr += "<br/><br/>";
  
  if(relay_on)
    {ptr +="<a class=\"button button-on\" href=\"/relay1off\">ON</a>\n";}
  else
    {ptr +="<a class=\"button button-off\" href=\"/relay1on\">OFF</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

void handle_OnConnect() {
  Serial.println("Status page");
  server.send(200, "text/html", SendHTML(relayState1)); 
}

void handle_relay1on() {
  relayState1 = true;
  Serial.println("relay: ON");
  server.send(200, "text/html", SendHTML(relayState1)); 
}

void handle_relay1off() {
  relayState1 = false;
  Serial.println("relay: OFF");
  server.send(200, "text/html", SendHTML(relayState1)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}



void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}

// the setup function runs once when you press reset or power the board
void setup() {
  // delay(3000);  // wait for serial monitor to start completely.
  Serial.begin(74880); // normally 115200 (but this seems too fast on this board)
  //  Serial.setDebugOutput(false);

  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  wifiManager.setAPCallback(configModeCallback);
  bool connected = wifiManager.autoConnect("WEMOS_BLINK"); // "name", "pass"
  
  // wifiManager.resetSettings();
  if(!connected) Serial.println("failed to connect to wifi network");
  else Serial.println("Connected to wifi");

  server.on("/", handle_OnConnect);
  server.on("/relay1on", handle_relay1on);
  server.on("/relay1off", handle_relay1off);
  server.onNotFound(handle_NotFound);
  server.enableCORS(true);
  server.begin();
}

// the loop function runs over and over again forever
void loop() {
  server.handleClient();

  if(relayState1){
    digitalWrite(relayPin1, LOW);   // LOW turns on internal led (reversed logic indeed) 
  }
  else{
    digitalWrite(relayPin1, HIGH);  // turn the LED off here
  }
}
