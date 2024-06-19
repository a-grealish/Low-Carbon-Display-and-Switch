#include <WiFi.h>
#include <Secrets.h>

void connect_to_wifi() {
  // We start by connecting to a WiFi network
  // To debug, please enable Core Debug Level to Verbose
  Serial.println();
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  // Auto reconnect is set true as default
  // To set auto connect off, use the following function
  //    WiFi.setAutoReconnect(false);

  // Will try for about 10 seconds (20x 500ms)
  int tryDelay = 500;
  int numberOfTries = 20;

  // Wait for the WiFi event
  while (true) {

    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL: Serial.println("[WiFi] SSID not found"); break;
      case WL_CONNECT_FAILED:
        Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
        return;
        break;
      case WL_CONNECTION_LOST: Serial.println("[WiFi] Connection was lost"); break;
      case WL_SCAN_COMPLETED:  Serial.println("[WiFi] Scan is completed"); break;
      case WL_DISCONNECTED:    Serial.println("[WiFi] WiFi is disconnected"); break;
      case WL_CONNECTED:
        Serial.println("[WiFi] WiFi is connected!");
        Serial.print("[WiFi] IP address: ");
        Serial.println(WiFi.localIP());
        return;
        break;
      default:
        Serial.print("[WiFi] WiFi Status: ");
        Serial.println(WiFi.status());
        break;
    }
    delay(tryDelay);

    if (numberOfTries <= 0) {
      Serial.print("[WiFi] Failed to connect to WiFi!");
      // Use disconnect function to force stop trying to connect
      WiFi.disconnect();
      return;
    } else {
      numberOfTries--;
    }
  }
};

void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println("Hello World!! I'm going to help lower your carbon footprint 🍃");

  // Check if wifi credentials stores
    // No: WiFi Provisioning
  // Connect to Wifi
  connect_to_wifi();

}

void loop() { 
  // Main Loop
  while(1) {
    Serial.print("Checking WiFi connection.. "); //--------------------------------
    delay(500); // just for aesthetics
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("⚠️ - ");
      Serial.print( WiFi.status());
    } else {
      Serial.print("✅");
    }
    Serial.print("\n");

    Serial.println("TODO: Check for updates from Carbon Feed"); //-----------------
    


    Serial.println("TODO: Update Display");
    Serial.println("TODO: Update Low Carbon Trigger Output");
    Serial.println("...\n...");
    delay(5000);
  };
}