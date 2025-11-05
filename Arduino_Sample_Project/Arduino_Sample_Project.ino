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
        readAndWriteWiFi(client);
      }
    }

    Serial.println("Client disconnected.");
    client.stop();
  }
}


void readAndWriteWiFi(WiFiClient client){
  String msg = client.readStringUntil('\n');
  msg.trim(); // Remove \r and spaces

  time = millis();

  if (msg.length() > 0 && time - prevStreamTime >= streamTimeout) {
    prevStreamTime = time;

    Serial.println("Godot: " + msg);

    if (strToBool(msg)){
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);
    }

    client.println("Arduino echo: " + msg);
  }
}


bool strToBool(String input){
  input.toUpperCase();
  return (input == "1" || input == "TRUE");
}
