#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#define GAS_SENSOR 35
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int trigPin = 17;
const int echoPin = 14;
int val;
long duration;
float distanceCm;

AsyncWebServer server(80);

// Set your access point network credentials
const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

String readDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  // Convert to inches
  // distanceInch = distanceCm * CM_TO_INCH;
  
  return String(distanceCm);
  //return String(1.8 * bme.readTemperature() + 32);
}

String readAlcohol() {
    val=analogRead(GAS_SENSOR);
    return String(val);
  
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);

  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDist().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readAlcohol().c_str());
  });
  
  /*server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());
  });*/

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
