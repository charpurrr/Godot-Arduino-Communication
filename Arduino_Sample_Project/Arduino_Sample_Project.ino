#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "Kitsunetwork";
char pass[] = "unfl3934";
long int port = 80;

int status = WL_IDLE_STATUS;
WiFiServer server(port);

unsigned long time = 0;
unsigned long prevStreamTime = 0;
const unsigned long streamTimeout = 1000; // How fast data will be sent and received. (in ms)


void setup() {
  Serial.begin(9600);

  while (!Serial) {}
  Serial.println("Serial communication opened.");

  // Check module
  if (WiFi.status() == WL_NO_MODULE){
    Serial.println("WiFi connection failed.");
    while(true){}
  }

  // Attempt connection
  while (status != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");

    status = WiFi.begin(ssid, pass);
    delay(1000);
  }

  // Complete connection
  Serial.print("Connected at: ");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(port);

  server.begin();

  // Pin setup
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  time = millis();

  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected.");

    while (client.connected()) {
      if (client.available()) {
        handleTraffic(client);
      }
    }

    Serial.println("Client disconnected.");
    client.stop();
  }
}


void handleTraffic(WiFiClient client){
  time = millis();

  // Retrieve the data from the client
  String msg = client.readStringUntil('\n');
  msg.trim(); // Remove \n, \r, and spaces.

  if (time - prevStreamTime >= streamTimeout) {
    prevStreamTime = time;

    if (msg.length() > 0) {
      handleIncoming(msg); 
    } 

    handleOutgoing(client);
  }
}


// Incoming client traffic
void handleIncoming(String msg){
  // VVV Defining functionality VVV
  Serial.println("Godot: " + msg);

  if (strToBool(msg)){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}


// Outgoing client traffic
void handleOutgoing(WiFiClient client){
  // VVV Defining functionality VVV
  String outgoing = ""; // Defines what data gets sent out to the client.
  outgoing = "Arduino echo: " + String(digitalRead(LED_BUILTIN));

  // Send data to the client
  client.println(outgoing);
}


bool strToBool(String input){
  input.toUpperCase();
  return (input == "1" || input == "TRUE");
}
