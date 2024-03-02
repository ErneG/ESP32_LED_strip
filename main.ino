#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

#define LED_PIN     13
#define NUM_LEDS    30
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
bool ledStates[NUM_LEDS] = {false}; // State tracking for LEDs

const char* ssid = "makets";
const char* password = "maketa_parole";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 LED Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { 
      background-color: #121212; 
      color: #E0E0E0; 
      font-family: Arial, sans-serif; 
      display: flex; 
      flex-direction: column; 
      align-items: center; 
      justify-content: center; 
      height: 100vh; 
      margin: 0; 
    }
    h2 {
      margin: 20px 0;
    }
    .row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      width: 80%;
      max-width: 500px;
    }
    button {
      background-color: #333333; 
      color: #FFFFFF; 
      border: none; 
      padding: 10px 20px; 
      cursor: pointer; 
      border-radius: 5px;
      transition: background-color 0.3s ease;
      flex-grow: 1; /* Make buttons grow to fill space */
    }
    button:hover {
      background-color: #555555;
    }
    input[type="color"] {
      border: 2px solid #333333; /* Make border visible */
      padding: 2px; 
      border-radius: 5px;
      background-color: #FFF; /* Ensure color visibility */
      margin-right: 10px;
      cursor: pointer;
      height: 38px; /* Match button height */
    }
  </style>
</head>
<body>
  <h2>ESP32 LED Control</h2>
  <div class="row"><input type="color" id="color1"><button onclick="toggleLED(1)">Toggle LED 1</button></div>
  <div class="row"><input type="color" id="color2"><button onclick="toggleLED(2)">Toggle LED 2</button></div>
  <div class="row"><input type="color" id="color3"><button onclick="toggleLED(3)">Toggle LED 3</button></div>
  <div class="row"><input type="color" id="color4"><button onclick="toggleLED(4)">Toggle LEDs 4 & 5</button></div>
<script>
function toggleLED(led) {
  const color = document.getElementById('color' + led).value.substring(1); // Remove '#' from color value
  fetch(`/toggle?led=${led}&color=${color}`);
}
</script>
</body>
</html>
)rawliteral";


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

// Function to toggle LED state
void toggleLEDState(int led, int r, int g, int b) {
    if (ledStates[led]) { // If already on, turn it off
        leds[led] = CRGB::Black;
    } else { // Else, turn it on with selected color
        leds[led] = CRGB(r, g, b);
    }
    ledStates[led] = !ledStates[led]; // Update state
}

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.clear();
  FastLED.show();

  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/toggle", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam("led") && request->hasParam("color")) {
      int ledNum = request->getParam("led")->value().toInt();
      String color = request->getParam("color")->value();

      long colorValue = strtol(color.c_str(), NULL, 16);
      int r = (colorValue >> 16) & 0xFF;
      int g = (colorValue >> 8) & 0xFF;
      int b = colorValue & 0xFF;

      // Apply toggle logic based on the LED number
      switch(ledNum) {
        case 1:
        case 2:
        case 3:
          toggleLEDState(ledNum - 1, r, g, b);
          break;
        case 4:
          // Apply to both LEDs 4 and 5
          toggleLEDState(3, r, g, b); // LED 4
          toggleLEDState(4, r, g, b); // LED 5
          break;
      }
      FastLED.show();
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Bad Request");
    }
  });

  server.onNotFound(notFound);

  server.begin();
}

void loop() {
   // This is supposed to be empty
}
