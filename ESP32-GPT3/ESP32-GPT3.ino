#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "myWifi";
const char* password = "myPassword";

// Number of switches/outputs
#define NUM_SWITCHES 6

// GPIO pins for switches and outputs
const int switchPins[NUM_SWITCHES] = {19, 21, 22, 23, 14, 16};
const int outputPins[NUM_SWITCHES] = {12, 27, 26, 25, 33, 32};

// Output states
bool outputStates[NUM_SWITCHES] = {false, false, false, false, false, false};

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);
// Create an AsyncWebSocket
AsyncWebSocket ws("/ws");

// Generate HTML for the webpage with WebSocket support
String generateHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Control</title>
    <style>
        body { font-family: Arial, sans-serif; max-width: 600px; margin: 0 auto; padding: 20px; }
        .output-row { display: flex; align-items: center; margin-bottom: 10px; }
        .output-status { 
            width: 100px; 
            padding: 5px; 
            text-align: center; 
            background-color: #f1f1f1; 
            margin-right: 10px; 
        }
        .output-status.on { background-color: #4CAF50; color: white; }
        .output-status.off { background-color: #f44336; color: white; }
    </style>
</head>
<body>
    <h1>ESP32 Output Control</h1>
    )rawliteral";

  for (int i = 0; i < NUM_SWITCHES; i++) {
    html += "<div class='output-row'>";
    html += "<div id='output" + String(i) + "' class='output-status'>Output " + String(i + 1) + ": OFF</div>";
    html += "<button onclick='toggleOutput(" + String(i) + ")'>Toggle</button>";
    html += "</div>";
  }

  html += R"rawliteral(
    <script>
    var socket = new WebSocket('ws://' + window.location.hostname + '/ws');
    
    socket.onmessage = function(event) {
        var data = JSON.parse(event.data);
        for (var i = 0; i < data.length; i++) {
            var outputElement = document.getElementById('output' + i);
            outputElement.textContent = 'Output ' + (i + 1) + ': ' + (data[i] ? 'ON' : 'OFF');
            outputElement.classList.toggle('on', data[i]);
            outputElement.classList.toggle('off', !data[i]);
        }
    };

    function toggleOutput(index) {
        fetch('/toggle?output=' + index);
    }
    </script>
</body>
</html>
)rawliteral";

  return html;
}

// Handle WebSocket events
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *payload, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("WebSocket client connected: %u\n", client->id());
    
    // Send current output states when a client connects
    String jsonStates = "[";
    for (int i = 0; i < NUM_SWITCHES; i++) {
      jsonStates += outputStates[i] ? "true" : "false";
      if (i < NUM_SWITCHES - 1) jsonStates += ",";
    }
    jsonStates += "]";
    client->text(jsonStates);
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("WebSocket client disconnected: %u\n", client->id());
  }
}

// Broadcast output states to all connected WebSocket clients
void broadcastOutputStates() {
  String jsonStates = "[";
  for (int i = 0; i < NUM_SWITCHES; i++) {
    jsonStates += outputStates[i] ? "true" : "false";
    if (i < NUM_SWITCHES - 1) jsonStates += ",";
  }
  jsonStates += "]";
  ws.textAll(jsonStates);
}

// Handle physical switch presses
void handleSwitchPresses() {
  for (int i = 0; i < NUM_SWITCHES; i++) {
    if (digitalRead(switchPins[i]) == LOW) { // LOW when pressed
      delay(50); // Debounce
      if (digitalRead(switchPins[i]) == LOW) { 
        outputStates[i] = !outputStates[i];
        digitalWrite(outputPins[i], outputStates[i] ? HIGH : LOW);
        broadcastOutputStates(); // Broadcast state change
        delay(300); // Prevent multiple toggles
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Allow time for Serial Monitor to open
  Serial.println("Starting ESP32...");

  // Initialize GPIO pins
  for (int i = 0; i < NUM_SWITCHES; i++) {
    pinMode(switchPins[i], INPUT_PULLUP); // Momentary switches
    pinMode(outputPins[i], OUTPUT);      // Outputs
    digitalWrite(outputPins[i], LOW);    // Start with outputs LOW
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // WebSocket setup
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

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
        broadcastOutputStates(); // Broadcast state change
      }
    }
    request->send(204); // No content
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  ws.cleanupClients(); // Cleanup disconnected clients
  handleSwitchPresses();
}
