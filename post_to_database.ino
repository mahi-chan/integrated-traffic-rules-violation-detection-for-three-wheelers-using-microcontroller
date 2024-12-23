#include <ESP8266HTTPClient.h> 
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

const char* ssid     = "moin";
const char* password = "asdf1234";
String URL = "http://192.168.0.105/kccdb/connection.php";

int distance = 8;
int speed = 350;

void setup()
{
  Serial.begin(115200);
  connectWiFi();  
  
}

void loop()
{/*
    if (Serial.available()) {
        // Read data from Arduino
        String receivedData = Serial.readStringUntil('\n');
        int commaIndex = receivedData.indexOf(',');
        
        if (commaIndex != -1) {
            // Extract the two integer values
            String distString = receivedData.substring(0, commaIndex);
            String speedString = receivedData.substring(commaIndex + 1);
            
            distance = distString.toInt();
            speed = speedString.toInt();
            
            // Process the received values 
            Serial.print("Received distance: ");
            Serial.println(distance);
            Serial.print("Received speed: ");
            Serial.println(speed);
        }
    }*/

  if(WiFi.status() != WL_CONNECTED) { 
    connectWiFi();
  }

  String postData = "distance=" + String(distance) + "&speed=" + String(speed); 

  HTTPClient http; 
  WiFiClient client;
  http.begin(client, URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  int httpCode = http.POST(postData); 
  String payload = http.getString(); 
  
  Serial.print("URL : "); Serial.println(URL); 
  Serial.print("Data: "); Serial.println(postData); 
  Serial.print("httpCode: "); Serial.println(httpCode); 
  Serial.print("payload : "); Serial.println(payload); 
  Serial.println("--------------------------------------------------");
}
void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  //This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}