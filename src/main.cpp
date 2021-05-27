
#define DHTTYPE DHT11 // DHT 11
#define DHTPIN 33
#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char *ssid = "EOLO-FRITZ!Box7430HS";
const char *password = "44033832564642442904";

//Your Domain name with URL path or IP address with path
const char *serverName = "http://bonex27.byethost31.com/esp-post-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /esp-post-data.php also needs to have the same key
String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "DHT11";
String sensorLocation = "Bedroom";

/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

DHT dht2 = DHT(DHTPIN, DHTTYPE);

//Adafruit_BME280 bme(BME_CS);  // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);  // software SPI

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;

unsigned long timerDelay = 10000;

void setup()
{
  Serial.begin(115200);
  pinMode(DHTPIN, INPUT);

  dht2.begin();
  Serial.println(F("DHTxx test!"));
  WiFi.begin(ssid, password);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("Connection");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    M5.Lcd.setCursor(100, 20);
    M5.Lcd.printf("...");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 30 seconds (timerDelay variable), it will take 30 seconds before publishing the first reading.");
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {

    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(serverName);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Prepare your HTTP POST request data

      String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName + "&location=" + sensorLocation + "&value1=" + String(dht2.readTemperature()) + "&value2=" + String(dht2.readHumidity()) + "&value3=" + String(0) + "";
      Serial.print("httpRequestData: ");
      Serial.println(httpRequestData);

      // You can comment the httpRequestData variable above
      // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");

      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

      if (httpResponseCode > 0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}