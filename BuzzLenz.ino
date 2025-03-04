#include <WiFi.h>              // Library for WiFi connection
#include <HTTPClient.h>        // Library for making HTTP requests
#include <ArduinoJson.h>       // Library for handling JSON data
#include <UbidotsESPMQTT.h>    // Ubidots MQTT library for sending sensor data

// Define pin numbers
#define IR_SENSOR_PIN 4       // Infrared sensor pin for detecting eye closure
#define BUZZER_PIN 18         // Buzzer pin for alerting the driver

// WiFi Credentials
#define ssid  "realme12"         // WiFi SSID
#define password "ashu1234"      // WiFi Password

// API Keys
#define apiKey  "AIzaSyDXukze0ajBBLpdC_UiuM63rxDJP9IHXvk"  // Google API Key for Geolocation
#define TOKEN "BBUS-igfy01RLC8rVyDT7ebJUShDGM4eMav"         // Ubidots API Token

// Ubidots Device Label
#define device_label "buzzlenz"  

// Initialize Ubidots client
Ubidots client(TOKEN);

// Variables to store geolocation data
float lat = 0.0, lon = 0.0;

// Timers for various functions
unsigned long blinkStartTime = 0;          // Timer for eye-blink detection
unsigned long lastGeolocationUpdate = 0;   // Timer for updating location
unsigned long lastUbidotsUpdate = 0;       // Timer for sending data to Ubidots

bool mqttConnected = false; // Flag to track MQTT connection status

void setup() {
    Serial.begin(115200);  // Start serial communication for debugging

    pinMode(IR_SENSOR_PIN, INPUT);   // Set IR sensor as input
    pinMode(BUZZER_PIN, OUTPUT);     // Set buzzer as output
    digitalWrite(BUZZER_PIN, LOW);   // Ensure buzzer is off initially

    // Start WiFi connection without blocking code execution
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");
}

// Function to establish an MQTT connection with Ubidots
void connectMQTT() {
    if (WiFi.status() == WL_CONNECTED && !mqttConnected) {  // Check if WiFi is connected and MQTT is not already connected
        Serial.println("Connecting to Ubidots MQTT...");
        client.wifiConnection(ssid, password);
        client.ubidotsSetBroker("things.ubidots.com");
        client.setDebug(true);
        client.begin(NULL);
        mqttConnected = true; // Mark MQTT as connected
    }
}

// Function to fetch current geolocation using Google API
void getGeolocation() {
    if (WiFi.status() == WL_CONNECTED) {  // Ensure WiFi is connected before making a request
        HTTPClient http;
        String url = "https://www.googleapis.com/geolocation/v1/geolocate?key=" + String(apiKey); // API endpoint
        http.begin(url);
        http.addHeader("Content-Type", "application/json");
        
        int httpCode = http.POST("{}"); // Send a POST request with empty body
        
        if (httpCode > 0) {  // Check if request was successful
            String payload = http.getString();
            StaticJsonDocument<512> doc;
            deserializeJson(doc, payload); // Parse the JSON response

            // Extract latitude and longitude
            lat = doc["location"]["lat"];
            lon = doc["location"]["lng"];

            // Print location data to serial monitor
            Serial.print("Latitude: "); Serial.println(lat, 6);
            Serial.print("Longitude: "); Serial.println(lon, 6);
        } else {
            Serial.println("Error getting location."); // Print error message if request fails
        }
        http.end(); // Close HTTP connection
    }
}

void loop() {
    // --- 1. Buzzer Logic (Eye-blink detection) ---
    if (digitalRead(IR_SENSOR_PIN) == HIGH) { // If IR sensor detects prolonged eye closure
        if (blinkStartTime == 0) blinkStartTime = millis();  // Start blink timer
        if (millis() - blinkStartTime >= 1500) digitalWrite(BUZZER_PIN, HIGH);  // Activate buzzer after 1.5 seconds
    } else {
        blinkStartTime = 0;  // Reset blink timer if eyes are open
        digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
    }

    delay(50); // Short delay to avoid excessive CPU usage

    // --- 2. WiFi & Ubidots Connection ---
    if (WiFi.status() == WL_CONNECTED) {  // Check if WiFi is connected
        connectMQTT(); // Ensure MQTT is connected only once

        // Update geolocation every 10 seconds
        if (millis() - lastGeolocationUpdate >= 10000) {
            getGeolocation();
            lastGeolocationUpdate = millis();
        }

        // Send data to Ubidots every 10 seconds
        if (millis() - lastUbidotsUpdate >= 10000) {
            client.add("lat", lat);
            client.add("lon", lon);
            client.ubidotsPublish(device_label);
            lastUbidotsUpdate = millis();
        }

        client.loop();  // Maintain MQTT connection
    } else {
        mqttConnected = false; // Reset MQTT connection if WiFi is lost
    }
}
