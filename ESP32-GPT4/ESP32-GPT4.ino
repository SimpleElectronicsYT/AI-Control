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

// Custom labels for outputs
const char* outputLabels[NUM_SWITCHES] = {
  "Bench 1 Lights", 
  "Bench 2 Lights", 
  "Aux 1", 
  "Aux 2", 
  "Aux 3", 
  "Aux 4"
};

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
        body { 
            font-family: Arial, sans-serif; 
            max-width: 600px; 
            margin: 0 auto; 
            padding: 20px; 
            background-color: #121212; 
            color: white; 
        }
        .output-row { 
            display: flex; 
            align-items: center; 
            margin-bottom: 10px; 
        }
        .output-label {
            flex-grow: 1;
            margin-right: 10px;
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
        }
        .output-status { 
            width: 120px; 
            padding: 5px; 
            text-align: center; 
            background-color: #333; 
            margin-right: 10px; 
            min-width: 120px; /* Keep static width */
        }
        .output-status.on { 
            background-color: #4CAF50; 
            color: white; 
        }
        .output-status.off { 
            background-color: #f44336; 
            color: white; 
        }
        button {
            background-color: #555;
            color: white;
            border: none;
            padding: 5px 10px;
            cursor: pointer;
        }
        button:hover {
            background-color: #666;
        }
        h1 {
            color: white;
            text-align: center;
        }
    </style>
</head>
<body>
    <h1>ESP32 Output Control</h1>
    )rawliteral";

  for (int i = 0; i < NUM_SWITCHES; i++) {
    html += "<div class='output-row'>";
    html += "<div class='output-label'>" + String(outputLabels[i]) + "</div>";
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

// Rest of the code remains the same as in the previous version...
// (Keep the entire previous implementation, just replace the generateHTML() function above)
