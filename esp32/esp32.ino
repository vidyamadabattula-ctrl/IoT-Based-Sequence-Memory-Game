#include <WiFi.h>
#include <HTTPClient.h>

// ---------- Wi-Fi ----------
const char* ssid = "GITAM";
const char* password = "Gitam$$123";

// ---------- ThingSpeak ----------
const char* writeAPIKey = "K8G7KJENYL99YI1I";

// ---------- UNO → ESP32 ----------
#define RX_PIN 16
#define TX_PIN 17

void sendToThingSpeak(int correct, int wrong, int attempts)
{
  int accuracy = (correct * 100) / attempts;

  String url = "http://api.thingspeak.com/update?api_key=";
  url += writeAPIKey;

  url += "&field1=";
  url += correct;

  url += "&field2=";
  url += wrong;

  url += "&field3=";
  url += attempts;

  url += "&field4=";
  url += accuracy;

  WiFiClient client;
  HTTPClient http;

  http.begin(client, url);

  int responseCode = http.GET();

  Serial.print("ThingSpeak response: ");
  Serial.println(responseCode);

  if (responseCode == 200)
  {
    Serial.println("Score sent to ThingSpeak!");
  }
  else
  {
    Serial.println("Failed to send score.");
  }

  http.end();
}

void setup()
{
  Serial.begin(115200);

  // UNO communication
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  Serial.println("ESP32 started");

  // Wi-Fi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");
  Serial.println("Waiting for UNO score...");
}

void loop()
{
  if (Serial2.available())
  {
    String message = Serial2.readStringUntil('\n');
    message.trim();

    Serial.print("Received from UNO: ");
    Serial.println(message);

    // Expected:
    // SCORE:7,3,10

    if (message.startsWith("SCORE:"))
    {
      String data = message.substring(6);

      int firstComma = data.indexOf(',');
      int secondComma = data.indexOf(',', firstComma + 1);

      if (firstComma != -1 && secondComma != -1)
      {
        int correct = data.substring(0, firstComma).toInt();

        int wrong = data.substring(
          firstComma + 1,
          secondComma
        ).toInt();

        int attempts = data.substring(
          secondComma + 1
        ).toInt();

        Serial.println("------ SCORE ------");

        Serial.print("Correct: ");
        Serial.println(correct);

        Serial.print("Wrong: ");
        Serial.println(wrong);

        Serial.print("Attempts: ");
        Serial.println(attempts);

        int accuracy = (correct * 100) / attempts;

        Serial.print("Accuracy: ");
        Serial.print(accuracy);
        Serial.println("%");

        sendToThingSpeak(correct, wrong, attempts);

        Serial.println("-------------------");
      }
    }
  }
}