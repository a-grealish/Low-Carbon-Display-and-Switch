#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <NetworkClientSecure.h>


// Expiry: Sun, 22 Jun 2036 00:00:00 GMT
const char* root_cert = "-----BEGIN CERTIFICATE-----\n"
"MIICCTCCAY6gAwIBAgINAgPlwGjvYxqccpBQUjAKBggqhkjOPQQDAzBHMQswCQYD\n"
"VQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEUMBIG\n"
"A1UEAxMLR1RTIFJvb3QgUjQwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAwMDAw\n"
"WjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2Vz\n"
"IExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjQwdjAQBgcqhkjOPQIBBgUrgQQAIgNi\n"
"AATzdHOnaItgrkO4NcWBMHtLSZ37wWHO5t5GvWvVYRg1rkDdc/eJkTBa6zzuhXyi\n"
"QHY7qca4R9gq55KRanPpsXI5nymfopjTX15YhmUPoYRlBtHci8nHc8iMai/lxKvR\n"
"HYqjQjBAMA4GA1UdDwEB/wQEAwIBhjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQW\n"
"BBSATNbrdP9JNqPV2Py1PsVq8JQdjDAKBggqhkjOPQQDAwNpADBmAjEA6ED/g94D\n"
"9J+uHXqnLrmvT/aDHQ4thQEd0dlq7A/Cr8deVl5c1RxYIigL9zC2L7F8AjEA8GE8\n"
"p/SgguMh1YQdc4acLa/KNJvxn7kjNuK8YAOdgLOaVsjh4rsUecrNIdSUtUlD\n"
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

  // TODO: Include the region filter
  if (https.begin(client, "https://api.electricitymap.org/v3/carbon-intensity/latest" )) {
    Serial.println("[HTTPS] GET..");

    int httpCode = https.GET();

    Serial.println("[HTTPS] Add auth headers");
    https.addHeader("auth-token", api_key);

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