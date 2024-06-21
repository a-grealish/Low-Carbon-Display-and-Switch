#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <NetworkClientSecure.h>


const char* server = "api.electricitymap.org";

// Expiry: Mon, 17 Sep 2040 16:00:00 GMT
const char* root_cert = "-----BEGIN CERTIFICATE-----\n"
"MIICGzCCAaGgAwIBAgIQQdKd0XLq7qeAwSxs6S+HUjAKBggqhkjOPQQDAzBPMQsw\n"
"CQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJuZXQgU2VjdXJpdHkgUmVzZWFyY2gg\n"
"R3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBYMjAeFw0yMDA5MDQwMDAwMDBaFw00\n"
"MDA5MTcxNjAwMDBaME8xCzAJBgNVBAYTAlVTMSkwJwYDVQQKEyBJbnRlcm5ldCBT\n"
"ZWN1cml0eSBSZXNlYXJjaCBHcm91cDEVMBMGA1UEAxMMSVNSRyBSb290IFgyMHYw\n"
"EAYHKoZIzj0CAQYFK4EEACIDYgAEzZvVn4CDCuwJSvMWSj5cz3es3mcFDR0HttwW\n"
"+1qLFNvicWDEukWVEYmO6gbf9yoWHKS5xcUy4APgHoIYOIvXRdgKam7mAHf7AlF9\n"
"ItgKbppbd9/w+kHsOdx1ymgHDB/qo0IwQDAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0T\n"
"AQH/BAUwAwEB/zAdBgNVHQ4EFgQUfEKWrt5LSDv6kviejM9ti6lyN5UwCgYIKoZI\n"
"zj0EAwMDaAAwZQIwe3lORlCEwkSHRhtFcP9Ymd70/aTSVaYgLXTWNLxBo1BfASdW\n"
"tL4ndQavEi51mI38AjEAi/V3bNTIZargCyzuFJ0nN6T5U6VR5CmD1/iQMVtCnwr1\n"
"/q4AaOeMSQ+2b1tbFfLn\n"
"-----END CERTIFICATE-----\n";


int get_latest_carbon_intensity(char* region, char* api_key){
  Serial.println("Getting latest Carbon Intensities");

  signed int ci = -1;

  NetworkClientSecure client;
  client.setCACert(root_cert);

  HTTPClient https;
  Serial.println("[HTTPS] begin...");

  // The way we're using the JSON parse doesn't support chuncked responses
  // Forcing http 1.0 fixes this
  https.useHTTP10(true);

  // TODO: Include the API_KEY and the region filter

  if (https.begin(client, "https://api.electricitymap.org/v3/carbon-intensity/latest" )) {
    Serial.println("[HTTPS] GET..");

    int httpCode = https.GET();

    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      // Example response from the `/latest` endpoint
      // {
      //   "zone": "IT-NO",
      //   "carbonIntensity": 306,
      //   "datetime": "2024-06-19T21:00:00.000Z",
      //   "updatedAt": "2024-06-19T21:45:05.094Z",
      //   "createdAt": "2024-06-16T21:48:10.094Z",
      //   "emissionFactorType": "lifecycle",
      //   "isEstimated": true,
      //   "estimationMethod": "TIME_SLICER_AVERAGE"
      // }

      // Parse response
      Serial.println("Deserialising");
      JsonDocument doc;
      deserializeJson(doc, https.getStream());

      // Read values
      Serial.println("Read Values");

      if (doc.containsKey("carbonIntensity")){
        ci = doc["carbonIntensity"].as<signed int>();
        Serial.printf("Carbon Intensity: %d\n", ci);
      } else {
        Serial.println("[HTTPS] Error, could not find carbonIntensity key in response");
      }

    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.println("[HTTPS] Unable to connect");
  }

  client.stop();

  return ci;
};