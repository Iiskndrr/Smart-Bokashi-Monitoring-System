#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <ModbusMaster.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Soil Sensor (RS485 and Modbus)
#define MAX485_DE_RE 4  // DE and RE connected to GPIO4 for TX/RX control
ModbusMaster node;

// DHT22 Setup
#define DHTPIN 32
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// Water level sensor
#define ONE_WIRE_BUS 13
#define POWER_PIN 17
#define SIGNAL_PIN 36
int value = 0;

// Functions to toggle between transmit and receive mode for RS485
void preTransmission() {
  digitalWrite(MAX485_DE_RE, HIGH);  // Enable RS485 Transmit mode
}
void postTransmission() {
  digitalWrite(MAX485_DE_RE, LOW);   // Enable RS485 Receive mode
}

// Task 1: Soil Sensor Task
void soilSensorTask(void *pvParameters) {
  uint8_t result;
  uint16_t data[4];

  while (true) {
    // Read Moisture Content
    result = node.readInputRegisters(0x0000, 1);
    if (result == node.ku8MBSuccess) {
      data[0] = node.getResponseBuffer(0);
      float moisture = data[0] / 10.0;
      Serial.print("Moisture Compost: ");
      Serial.print(moisture);
      Serial.println(" %");
      Blynk.virtualWrite(V4, moisture);
    } else {
      Serial.print("Moisture Read Error: ");
      Serial.println(result, HEX);
    }
    
    delay(2000);

    // Read Temperature
    result = node.readInputRegisters(0x0001, 1);
    if (result == node.ku8MBSuccess) {
      data[1] = node.getResponseBuffer(0);
      float temperature = data[1] / 10.0;
      Serial.print("Temperature Compost: ");
      Serial.print(temperature);
      Serial.println(" °C");
      Blynk.virtualWrite(V3, temperature);
    } else {
      Serial.print("Temperature Read Error: ");
      Serial.println(result, HEX);
    }
    
    delay(2000);

    // Read pH Value
    result = node.readInputRegisters(0x0003, 1);
    if (result == node.ku8MBSuccess) {
      data[2] = node.getResponseBuffer(0);
      float ph = data[2] / 10.0;
      Serial.print("pH Compost: ");
      Serial.println(ph);
      Blynk.virtualWrite(V2, ph);
    } else {
      Serial.print("pH Read Error: ");
      Serial.println(result, HEX);
    }
    
    delay(2000);
  }
}

// Task 2: Blynk Task
void otherSensorTask(void *pvParameters) {
  while (true) {
    // Read temperature and humidity from DHT22
    float t = dht.readTemperature();
    Serial.print("Temperature Ambient: ");
    Serial.print(t);
    Serial.println(" C ");
    Blynk.virtualWrite(V0, t);  // Blynk virtual pin for temperature
    delay(2000);

    // Read water level
    digitalWrite(POWER_PIN, HIGH);  // Turn sensor ON
    delay(10);
    value = analogRead(SIGNAL_PIN); // Read water level
    digitalWrite(POWER_PIN, LOW);   // Turn sensor OFF
    Serial.print("Water Level: ");
    Serial.println(value-2300);
    Blynk.virtualWrite(V1, value-2300);  // Blynk virtual pin for water level
    delay(2000);
  }
}

void setup() {
  // Initialize Serial for debug
  Serial.begin(115200);

  // Initialize RS485 communication for soil sensor
  Serial2.begin(4800, SERIAL_8N1, 16, 17);
  pinMode(MAX485_DE_RE, OUTPUT);
  digitalWrite(MAX485_DE_RE, LOW); // Start in Receive mode
  node.begin(1, Serial2);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // Initialize WiFi and Blynk
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  Blynk.begin(auth, ssid, pass);
  while (!Blynk.connected()) {
    Serial.println("Connecting to Blynk...");
    delay(1000);
  }
  Serial.println("Connected to Blynk");

  // Initialize DHT sensor
  dht.begin();

  // Create FreeRTOS tasks
  xTaskCreate(soilSensorTask, "Soil Sensor Task", 10000, NULL, 1, NULL);
  xTaskCreate(otherSensorTask, "Blynk Task", 10000, NULL, 1, NULL);
}

void loop() {
  Blynk.run();  // Ensure Blynk connection stays active
}
