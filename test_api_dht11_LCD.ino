#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>

#include "DHT.h"
#include <LCD_I2C.h>
LCD_I2C lcd(0x27);

#define DHTPIN 4 
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "TESTESP";// เปลี่ยน ssid ก่อนทดลอง
const char* password = "a1234567";// เปลี่ยน password ก่อนทดลอง

void setup() {
  Serial.begin(115200);
  delay(500);
 
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  };
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  lcd.begin();
  lcd.backlight();
}

void loop() {
    
 

    
   
String url_post = "https://mysensor-test.herokuapp.com/data";
  Serial.println();
  Serial.println("Get content from " + url_post);

  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int t = dht.readTemperature();
  
  
  HTTPClient http;
  
  // Send data to database
  http.begin(url_post);
  Serial.println(url_post); // show url  
  http.addHeader("Content-Type", "application/json");
  String test = "{\"sensor_id\":\"1\",\"measurement_value\":"+String(t)+"}"; // Temperature
  Serial.println(test); // show url
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
  Serial.println("END POST");
  
  // Send data to database
  http.begin(url_post);
  Serial.println(url_post); // show url  
  http.addHeader("Content-Type", "application/json");
   test = "{\"sensor_id\":\"2\",\"measurement_value\":"+String(h)+"}"; // Huminity
  Serial.println(test);
  httpCode = http.POST( test);
  
//  int httpCode = http.GET();
  if (httpCode == 200) {
    String content = http.getString();
    Serial.println("Content ---------");
    Serial.println(content);
    Serial.println("-----------------");
  } else {
    Serial.println("Fail. error code " + String(httpCode));
  }
  Serial.println("END POST");
  
  // Read to check data after send to database
  String url_get = "https://mysensor-test.herokuapp.com/data?id=1";
  http.begin(url_get);
  Serial.println(url_get);
  httpCode = http.GET();
  if (httpCode == 200) {
    String content = http.getString();
    Serial.println("Content ---------");
    Serial.println(content);
    Serial.println("-----------------");
    lcd.clear();
    //[{"sensor_id":1,"measurement_value":29,"date_time":"2021-05-05T13:31:35.000Z"}]
    lcd.setCursor(0, 0); 
    lcd.print("sensor_id:");
    lcd.print(content[14]);lcd.print("  ");
    lcd.setCursor(0, 1); 
    lcd.print("value:");
    lcd.print(content[36]);
    lcd.print(content[37]);lcd.print("  ");
  } else {
    Serial.println("Fail. error code " + String(httpCode));
  }
  Serial.println("END GET");
   delay(3000); 
    // Read to check data after send to database
   url_get = "https://mysensor-test.herokuapp.com/data?id=2";
  http.begin(url_get);
  Serial.println(url_get);
  httpCode = http.GET();
  if (httpCode == 200) {
    String content = http.getString();
    Serial.println("Content ---------");
    Serial.println(content);
    Serial.println("-----------------");
    lcd.clear();
    //[{"sensor_id":1,"measurement_value":29,"date_time":"2021-05-05T13:31:35.000Z"}]
    lcd.setCursor(0, 0); 
    lcd.print("sensor_id:");
    lcd.print(content[14]);lcd.print("   ");
    lcd.setCursor(0, 1); 
    lcd.print("value:");
    lcd.print(content[36]);
    lcd.print(content[37]);lcd.print("   ");
  } else {
    Serial.println("Fail. error code " + String(httpCode));
  }
  Serial.println("END GET");
  delay(3000); 
    
 
 

    
  }
  
