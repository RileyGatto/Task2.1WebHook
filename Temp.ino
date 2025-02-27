#include <WiFiNINA.h>
#include "secrets.h"
#include "DHT.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY; //dont need & because string literals is treated as const char *

// Initialize our values
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

void setup() {
  Serial.begin(115200);      // Initialize serial 
  while (!Serial) ;

  dht.begin();
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  float temperature = dht.readTemperature();

  if(isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor :(");
    return;
  }

  // set the fields with the values
  ThingSpeak.setField(1, temperature);

  
  // write to the ThingSpeak channel 
  int result = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(result == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(result));
  }
  
  delay(20000);
}
