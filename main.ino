#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

#define LED_PIN     13
#define NUM_LEDS    55
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
bool ledStates[NUM_LEDS] = {false};

const char* ssid = "THE_NAME_OF_YOUR_WIFI_NETWORK";
const char* password = "THE_PASSWORD_OF_YOUR_WIFI_NETWORK";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML><html>
<head>
  <title>Maketa apgaismojums</title>
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
  <h2>Maketa apgaismojums</h2>
  <div class="row"><input type="color" id="color1" value="#00FF00"><button onclick="toggleLED(1)">Viesnīca</button></div>
  <div class="row"><input type="color" id="color2" value="#00FF00"><button onclick="toggleLED(2)">Tirgus</button></div>
  <div class="row"><input type="color" id="color3" value="#00FF00"><button onclick="toggleLED(3)">Skatu tornis</button></div>
  <div class="row"><input type="color" id="color4" value="#00FF00"><button onclick="toggleLED(4,5)">Mākslinieku kvartāls</button></div>
<script>
function toggleLED(...leds) {
  const color = document.getElementById('color' + leds[0]).value.substring(1); // Remove '#' from color value
  fetch(`/toggle?leds=${leds.join(',')}&color=${color}`);
}
</script>
</body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void toggleLEDState(const int ledArray[], int arraySize, int r, int g, int b) {
    bool tempStates[NUM_LEDS] = {false};
    for (int i = 0; i < arraySize; i++) {
        int led = ledArray[i];
        if (led >= 0 && led < NUM_LEDS) {
            tempStates[led] = true;
            if (ledStates[led]) {
                leds[led] = CRGB::Black;
                ledStates[led] = false;
            } else {
                leds[led] = CRGB(r, g, b);
                ledStates[led] = true;
            }
        }
    }
    for (int i = 0; i < NUM_LEDS; i++) {
        if (!tempStates[i] && ledStates[i]) {
            leds[i] = CRGB::Black;
            ledStates[i] = false;
        }
    }
    FastLED.show();
}

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.clear();
  FastLED.show();

  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.println(WiFi.softAPIP());

  // Define server routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/toggle", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam("leds") && request->hasParam("color")) {
      String ledString = request->getParam("leds")->value();
      String color = request->getParam("color")->value();

      long colorValue = strtol(color.c_str(), NULL, 16);
      int r = (colorValue >> 16) & 0xFF;
      int g = (colorValue >> 8) & 0xFF;
      int b = colorValue & 0xFF;

      std::vector<int> ledIndexes;
      int startIndex = 0, endIndex = 0;
      while ((endIndex = ledString.indexOf(',', startIndex)) != -1) {
        ledIndexes.push_back(ledString.substring(startIndex, endIndex).toInt() - 1);
        startIndex = endIndex + 1;
      }
      ledIndexes.push_back(ledString.substring(startIndex).toInt() - 1);

      int ledArray[ledIndexes.size()];
      std::copy(ledIndexes.begin(), ledIndexes.end(), ledArray);

      toggleLEDState(ledArray, ledIndexes.size(), r, g, b);
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Bad Request");
    }
  });

  server.onNotFound(notFound);

  server.begin();
}

void loop() {
  // Intentionally left empty
}
