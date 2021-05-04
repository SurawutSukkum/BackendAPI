#include <WiFi.h>
#include <HTTPClient.h>

#include "DHT.h"
#define DHTPIN 4 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define WIFI_STA_NAME "TESTESP"
#define WIFI_STA_PASS "a1234567"


void setup() {
  Serial.begin(115200);
  //

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

 // digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  dht.begin();
}
void loop() {
   
  String url = "https://mysensor-test.herokuapp.com/data";
  Serial.println();
  Serial.println("Get content from " + url);

  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int t = dht.readTemperature();
  
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  String test = "{\"sensor_id\":\"555\",\"measurement_value\":"+String(t)+"}";
  Serial.println(test);
  int httpCode = http.POST( test);
  
//  int httpCode = http.GET();
  if (httpCode == 200) {
    String content = http.getString();
    Serial.println("Content ---------");
    Serial.println(content);
    Serial.println("-----------------");
  } else {
    Serial.println("Fail. error code " + String(httpCode));
  }
  Serial.println("END");
  delay(3000); 
  }
