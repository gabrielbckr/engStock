#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ESP8266WebServer.h>   // Include the Web Server library

const uint8_t LED_HIGH = 0;
const uint8_t LED_LOW = 1;

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LED_LOW);
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("Becker", "theovoador");   // add Wi-Fi networks you want to connect to
  //wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  //wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer
  String macAdress = WiFi.macAddress();
  macAdress.replace(":","");
  if (!MDNS.begin("nodemcu"+macAdress)) {             // Start the mDNS responder for nodemcu.local
    Serial.println("Error setting up mDNS responder!");
  }
  else {
    digitalWrite(LED_BUILTIN, LED_HIGH);
    Serial.println("mDNS responder started");
    Serial.println("Use domain name nodemcu.local to access this device");
  }
  server.on("/", handleRoot);  //Associate handler function to path
  server.begin();              //Start server
  Serial.print("HTTP server started.");
}

void loop() {
  server.handleClient();
  MDNS.update();
}

void handleRoot() {
  server.send(200, "text/plain", "Hello resolved by mDNS! MAC: "+WiFi.macAddress());
}
