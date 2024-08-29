#include "DHT.h"
#define DHTPIN 5 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "BABBITTY";
const char* password = "nopassword";
const char* mqtt_server = "mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if (String(topic) == "switch_lamp") //รับข้อมูลจาก subscriber ชิ้อว่า dewdewbearr
  {
    if ((char)payload[0] == '1')  //มีตัวอักษตร ตำเเหน่งเเรก [0]= 1 ไฟติด
    digitalWrite(D2, HIGH);   

    if ((char)payload[0] == '0') 
    digitalWrite(D2, LOW);
  } 
  

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected nanananan");
      // Once connected, publish an announcement...
      client.publish("dewdewbear", "hello world");   //เมื่อทำการ connect mtqq ได้จะทำการ Publish ข้อมูลไป topic (dewdewbear) ว่า Hello world
      // ... and resubscribe
      client.subscribe("switch_lamp"); //เมื่อทำการ connect mtqq ได้จะทำการ subscribe topic (dewdewbearr)
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(D2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
   dht.begin();
}

void loop() {
 
 float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  Serial.print("Temperture =");Serial.print(t);Serial.println("C");
  delay(1000);
  Serial.print("Humidity =");Serial.print(f);Serial.println("%");
  delay(1000);
  

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

 

String test = String(t); //แปลงให้เป็น const char คำส่ง name.c_str() 
String hum = String(h);    
  
    Serial.print("Publish message: ");
    Serial.println(test.c_str());
    client.publish("dht_temp", test.c_str()); // ส่งข้อมูลไปที่ Topic dewdewbear บน Mtqq ด้วยขอความ (test.c_str() ที่แปลงมา
    client.publish("dht_hum", hum.c_str()); // ส่งข้อมูลไปที่ Topic dewdewbear บน Mtqq ด้วยขอความ (test.c_str() ที่แปลงมา
}
