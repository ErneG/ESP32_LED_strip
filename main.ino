#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>

#define LED_PIN     13
#define NUM_LEDS    30
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

const char* ssid = "makets";
const char* password = "maketa_parole";

AsyncWebServer server(80);

// Updated HTML & JavaScript content with four buttons and color inputs
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 LED Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h2>ESP32 LED Control</h2>
  <p>
    <input type="color" id="color1">
    <button onclick="toggleLED(1)">Toggle LED 1</button>
  </p>
  <p>
    <input type="color" id="color2">
    <button onclick="toggleLED(2)">Toggle LED 2</button>
  </p>
  <p>
    <input type="color" id="color3">
    <button onclick="toggleLED(3)">Toggle LED 3</button>
  </p>
  <p>
    <input type="color" id="color4">
    <button onclick="toggleLED(4)">Toggle LEDs 4 & 5</button>
  </p>
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

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  // Start the ESP32 as an Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP()); // Print the IP address

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Handle LED toggle with color
  server.on("/toggle", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam("led") && request->hasParam("color")) {
      int led = request->getParam("led")->value().toInt();
      String color = request->getParam("color")->value();
      
      // Convert color from String to long integer, then to RGB
      long colorValue = strtol(color.c_str(), NULL, 16);
      int r = (colorValue >> 16) & 0xFF;
      int g = (colorValue >> 8) & 0xFF;
      int b = colorValue & 0xFF;

      // Apply color to the selected LED(s)
      switch(led) {
        case 1:
        case 2:
        case 3:
          leds[led - 1] = CRGB(r, g, b);
          break;
        case 4:
          leds[3] = CRGB(r, g, b);
          leds[4] = CRGB(r, g, b);
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
