#include <electricity_map_api.hpp>
#include <Secrets.h>

#include <WiFi.h>

#define RGB_BRIGHTNESS 64 // Change white brightness (max 255)

typedef struct {
  uint8_t r, g, b;
} tColor;

void connect_to_wifi() {
  // We start by connecting to a WiFi network
  // To debug, please enable Core Debug Level to Verbose
  Serial.println();
  // Todo: Remove printing of password
  Serial.printf("[WiFi] Connecting to %s : %s \n", ssid, password);


  WiFi.begin(ssid, password);
  // Auto reconnect is set true as default
  // To set auto connect off, use the following function
  //    WiFi.setAutoReconnect(false);

  // Will try for about 30 seconds (30x 1000ms)
  int tryDelay = 1000;
  int numberOfTries = 30;

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
      Serial.println("[WiFi] Failed to connect to WiFi!");
      // Use disconnect function to force stop trying to connect
      WiFi.disconnect();
      return;
    } else {
      numberOfTries--;
    }
  }
};

tColor carbon_intensity_to_color(int ci){
  // Colors to match the electricityMaps app
  // Taken from: https://github.com/electricitymaps/electricitymaps-contrib/blob/d51fc19913a0445533d831a31473b95c31e9c000/web/src/hooks/colors.ts#L3
  // const defaultCo2Scale = {
  //   steps: [0, 150, 600, 800, 1100, 1500],
  //   colors: ['#2AA364', '#F5EB4D', '#9E4229', '#381D02', '#381D02', '#000'],
  // };

  if(ci<=150){
    // #2AA364
    return {0x2a, 0xa3, 0x64};
  } else if(ci<=600){
    // #F5EB4D
    return {0xF5, 0xEB, 0x4D};
  } else if (ci<=600){
    // #9E4229
    return {0x9E, 0x42, 0x29};
  } else if (ci<=1500){
    // Merging the last two steps as it's the same color
    // #381D02
    return {0x38, 0x1D, 0x02};
  } else {
    // Return blue as the error code
    return {0, 0, 255};
  }

}

void setup() {
  // Initiate the LED to a Blue colour
  neopixelWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS);

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

    Serial.println("Checking for updates from Carbon Feed"); //-----------------
    int ci = get_latest_carbon_intensity("GB", elec_map_api_key);
    Serial.printf("Current Carbon Intensity: %d\n", ci);


    Serial.println("TODO: Update Display");
    tColor c = carbon_intensity_to_color(ci);
    neopixelWrite(RGB_BUILTIN, c.r, c.g, c.b);


    Serial.println("TODO: Update Low Carbon Trigger Output");
    Serial.println("...\n...");

    Serial.println("Sleeping for 1 hour");
    delay(60*60*1000);
  };
}