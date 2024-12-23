#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "None of your business";
const char* password = "Subscribe!";

// Number of switches/outputs
#define NUM_SWITCHES 6

// GPIO pins for switches and outputs
const int switchPins[NUM_SWITCHES] = {19, 21, 22, 23, 14, 16};
const int outputPins[NUM_SWITCHES] = {12, 27, 26, 25, 33, 32};

// Output states
bool outputStates[NUM_SWITCHES] = {false, false, false, false, false, false};

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// Handle physical switch presses
void handleSwitchPresses() {
  for (int i = 0; i < NUM_SWITCHES; i++) {
    if (digitalRead(switchPins[i]) == LOW) { // LOW when pressed
      delay(50); // Debounce
      if (digitalRead(switchPins[i]) == LOW) { 
        outputStates[i] = !outputStates[i];
        digitalWrite(outputPins[i], outputStates[i] ? HIGH : LOW);
        delay(300); // Prevent multiple toggles
      }
    }
  }
}

// Generate HTML for the webpage
String generateHTML() {
  String html = "<!DOCTYPE html><html><head><title>ESP32 Control</title></head><body>";
  html += "<h1>ESP32 Output Control</h1>";
  for (int i = 0; i < NUM_SWITCHES; i++) {
    html += "<p>Output " + String(i + 1) + ": ";
    html += "<span>" + String(outputStates[i] ? "ON" : "OFF") + "</span>";
    html += "<button onclick=\"toggleOutput(" + String(i) + ")\">Toggle</button></p>";
  }
  html += "<script>function toggleOutput(index) {fetch('/toggle?output=' + index)}</script>";
  html += "</body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);

  // Initialize GPIO pins
  for (int i = 0; i < NUM_SWITCHES; i++) {
    pinMode(switchPins[i], INPUT_PULLUP); // Momentary switches
    pinMode(outputPins[i], OUTPUT);      // Outputs
    digitalWrite(outputPins[i], LOW);    // Start with outputs LOW
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Serve the webpage
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", generateHTML());
  });

  // Handle toggle requests
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->hasParam("output")) {
      int index = request->getParam("output")->value().toInt();
      if (index >= 0 && index < NUM_SWITCHES) {
        outputStates[index] = !outputStates[index];
        digitalWrite(outputPins[index], outputStates[index] ? HIGH : LOW);
      }
    }
    request->send(204); // No content
  });

  server.begin();
}

void loop() {
  handleSwitchPresses();
}
