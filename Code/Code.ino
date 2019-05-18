#include "FirebaseArduino.h"
#include <ESP8266WiFi.h>
#include "ACS712.h"
#include <Smoothed.h> 
#include <math.h>
#define FIREBASE_HOST "YOUR HOST"
#define FIREBASE_AUTH "YOUR AUTH KEY"
#define WIFI_SSID "YOUR WIFI SSID"
#define WIFI_PASSWORD "YOUR WIFI PASSWORD"
#define sensorInput A0
#define lastVal
#define smoothedCurrentValExP
#define currentReading
#define Relay_pin D3
ACS712 sensor(ACS712_20A, sensorInput);
Smoothed <float> mySensor; 
float I_reading = 0;
void setup() {
  pinMode(Relay_pin,OUTPUT);
String path="/MeasuredData";
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
	  
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  mySensor.begin(SMOOTHED_AVERAGE, 10);
  sensor.calibrate();
  delay(1000);
  sensor.calibrate();
 
}

void loop() {
	bool menu=Firebase.getBool("MeasuredData/Menu");
		switch (menu)
		{
		case true: {
			delay(1000);
			digitalWrite(Relay_pin, HIGH);
			float I_reading = sensor.getCurrentAC(50);
			Firebase.setFloat("MeasuredData/Current",I_reading);
			break;
		}
		case false:
		{sensor.calibrate();
		digitalWrite(Relay_pin, LOW);
		sensor.calibrate();
		I_reading = 0;
		Firebase.setFloat("MeasuredData/Current", I_reading);
		break;
		}
		default:
			break;
		}
	
	 }
