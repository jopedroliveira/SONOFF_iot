/*
   This script uses a ESP8266 wifi module to control a LED Pin through an  JSON client.
   The webserver is hosted in the module and the LED is connected to the PIN 2.
   The JSON clients allows to tunr on and off the light and check the current status.
   Developed by Pedro Oliveira
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


const char* ssid = "****";//type your ssid
const char* password = "****";//type your password

IPAddress ip(192, 168, 1, 49); //Requested static IP address for the ESP
IPAddress router(192, 168, 1, 1); // IP address for the Wifi router
IPAddress netmask(255, 255, 255, 0);

int ledPin = 2; // GPIO2 of ESP8266
WiFiServer server(80);
int value = LOW;

void setup() {
  Serial.begin(9600);
  delay(10);


  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.config(ip, router, netmask);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request --------- REGEXP \r means CARIAGE RETURN
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  
  if (request.indexOf("LED=ON") != -1) { // if indexOf returns -1 means that the string Request do not contains "/LED=ON"
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("LED=OFF") != -1) { // if indexOf returns -1 means that the string Request do not contains "/LED=OFF"
    digitalWrite(ledPin, LOW);
    value = LOW;
  }

  String response = "";
  if (value == HIGH) {
    response = "{status: on}";
  } else {
    response = "{status: off}";
  }
  
  client.println("HTTP/1.1 200 OK");
  
  if(request.indexOf("api/status") !=-1 || request.indexOf("api/LED=") !=-1){
    client.println("Content-Type: application/json;charset=utf-8");
    client.println("Server: ESP8266");
    client.println("Server: Arduino");
    client.println();
    client.println(response);
    client.println();
    } else {
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("Atual status: \n");
    client.println(response);
    client.println("<br><br>");
    client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 2 ON<br>");
    client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 2 OFF<br>");
    client.println("</html>");
  }
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
