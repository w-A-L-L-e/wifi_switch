/*
  Wifi Web Light Switch v1 beta

  This allows to switch on lights over the wifi. When board first boots it
  starts as access point. You can connect to it with browser by joining its
  created wifi network. It then allows selecting wifi network + set user + pass
  in the browser. After this it boots and shows a webpage that allows toggling a
  relay on/off in the browser.

  Board: Wemos D1 mini clone. Use
  https://arduino.esp8266.com/stable/package_esp8266com_index.json in board
  manager in configuration and then select LOLIN Wemos d1 clone (my super cheap
  board has 4mb but we only use less than 360kb for this sketch.) Serial speed:
  74880

  Libraries used:
  https://github.com/tzapu/WiFiManager
  https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer

  Author: Walter Schreppers
*/

#include <ESP8266WebServer.h>
#include <WiFiManager.h>

WiFiManager wifiManager;
ESP8266WebServer server(80);

// remark LED_BUILTIN == D4 == GPIO2
// we connect our relay to D3 pin
int relayPin1 = D3; // D3 == GPIO0

// state at power up (it switches on anyway at power up due to 
// inverse logicsetting false will briefly on/off)
bool relayState1 = true;


String SendHTML(uint8_t relay_on) {
  String page = "<!DOCTYPE html> <html>\n";
  page += "<head><meta name=\"viewport\" content=\"width=device-width, "
         "initial-scale=1.0, user-scalable=no\">\n";
  page += "<title>LED Control</title>\n";
  page += "<style>html { font-family: Helvetica; display: inline-block; margin: "
         "0px auto; text-align: center;}\n";
  page += "body{margin-top: 50px;background-color: #000000;} h1 {color: "
         "#0d60db;margin: 50px auto 30px;} h3 {color: #0d60db;margin-bottom: "
         "50px;}\n";
  page +=
      ".button {display: block;width: 80px;background-color: #1abc9c;border: "
      "none;color: white;padding: 13px 30px;text-decoration: none;font-size: "
      "25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  page += ".button-on {background-color: #1abc9c;}\n";
  page += ".button-on:active {background-color: #16a085;}\n";
  page += ".button-off {background-color: #34495e;}\n";
  page += ".button-off:active {background-color: #2c3e50;}\n";
  page += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  page += "</style>\n";
  page += "</head>\n";
  page += "<body>\n";
  page += "<h1>Wifi light switch</h1>\n";
  page += "<p><a href=\"/\">Wemos D1 ESP8266 Web Server</a></p>";
  page += "<p>Board ID: 0001</p>";
  page += "<br/><br/>";

  if (relay_on) {
    page += "<a class=\"button button-on\" href=\"/relay1off\">ON</a>\n";
  } else {
    page += "<a class=\"button button-off\" href=\"/relay1on\">OFF</a>\n";
  }

  page += "</body>\n";
  page += "</html>\n";
  return page;
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

void handle_wifireset(){
  server.send(404, "text/plain", "Wifi will be reset. You can connect to 'WIFI_SWITCH' access point.");
  wifiManager.resetSettings();
  ESP.restart();
}

void handle_NotFound() { server.send(404, "text/plain", "Not found"); }

void configModeCallback(WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setPins(bool relayState){
  if (relayState) {
    digitalWrite(LED_BUILTIN, LOW);  // led on
    digitalWrite(relayPin1, LOW);    // relais on
  } else {
    digitalWrite(LED_BUILTIN, HIGH);   // led off
    digitalWrite(relayPin1, HIGH);     // relay off
  }
}

// the setup function runs once when you press reset or power the board
void setup() {  
  Serial.begin(74880);
  // Serial.setDebugOutput(false);

  setPins(false);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  setPins(false);

  
  Serial.print("relay pin = ");
  Serial.println(relayPin1);

  Serial.print("led pin = ");
  Serial.println(LED_BUILTIN);
  
  wifiManager.setAPCallback(configModeCallback);
  bool connected = wifiManager.autoConnect("WIFI_SWITCH"); // "name", "pass"

  if (!connected)
    Serial.println("failed to connect to wifi network");
  else
    Serial.println("Connected to wifi");

  server.on("/", handle_OnConnect);
  server.on("/relay1on", handle_relay1on);
  server.on("/relay1off", handle_relay1off);
  server.on("/resetwifi", handle_wifireset);
  server.onNotFound(handle_NotFound);
  server.enableCORS(true);

  setPins(relayState1);

  server.begin();
}

// Handle web requests and toggle relayPin
void loop() {
  server.handleClient();
  setPins(relayState1);
}
