#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <ArduinoJson.h>



// Define WiFi credentials
#define WIFI_SSID "IDwifi"
#define WIFI_PASSWORD "senhawifi"

// Define Firebase API Key, Project ID, and user credentials
#define API_KEY "suaAPI"
#define FIREBASE_PROJECT_ID "IDFirebase"
#define USER_EMAIL "emailauth"
#define USER_PASSWORD "senhaauth"

// Define Firebase Data object, Firebase authentication, and configuration
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;



int humidade;
boolean irriga_on;
int minUmid;
int maxUmid;



int ledPin1 = 2;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);

  pinMode(ledPin1, OUTPUT); // Define o D7 como saída

  digitalWrite(ledPin1, LOW);


  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Print Firebase client version
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  // Assign the API key
  config.api_key = API_KEY;

  // Assign the user sign-in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the callback function for the long-running token generation task
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

  // Begin Firebase with configuration and authentication
  Firebase.begin(&config, &auth);

  // Reconnect to Wi-Fi if necessary
  Firebase.reconnectWiFi(true);
}

void loop() {

  String path = "nodemcu/limites";
  String mask = "limites";

  Serial.print("Get entire collection... ");
  if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", path.c_str(),""))
  {

    
    Serial.println("ok");
    Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
    StaticJsonDocument<384> doc;

    DeserializationError error = deserializeJson(doc, fbdo.payload().c_str());

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    const char* name = doc["name"];

    JsonObject fields = doc["fields"];

    const char* fields_max_integerValue = fields["max"]["integerValue"]; // "12"
    maxUmid = atoi(fields_max_integerValue);
    const char* fields_min_stringValue = fields["min"]["integerValue"]; // "8"
    minUmid = atoi(fields_min_stringValue);

    const char* createTime = doc["createTime"]; // "2023-12-10T21:59:47.202759Z"
    const char* updateTime = doc["updateTime"]; // "2023-12-12T02:22:16.621387Z"


  }
















    // Define the path to the Firestore document
    String documentPath = "nodemcu/valores";

    // Create a FirebaseJson object for storing data
    FirebaseJson content;


    humidade = map(analogRead(A0), 0, 1023, 0, 100);


    if (humidade < minUmid) {
      digitalWrite(ledPin1, HIGH);
      irriga_on = true;
    }
    if (humidade > maxUmid) {
      digitalWrite(ledPin1, LOW);
      irriga_on = false;
    }
    // Print temperature and humidity values
    //  Serial.println(temperature);
    //  Serial.println(humidity);

    // Check if the values are valid (not NaN)
    if (!isnan(humidade) && !isnan(irriga_on)) {
      // Set the 'Temperature' and 'Humidity' fields in the FirebaseJson object
      content.set("fields/humidade/integerValue", humidade );
      content.set("fields/irrigacao/booleanValue", irriga_on);

      Serial.print("Update/Add DHT Data... ");

      // Use the patchDocument method to update the Temperature and Humidity Firestore document
      if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "humidade") && Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.raw(), "irrigacao")) {
        Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      } else {
        Serial.println(fbdo.errorReason());
      }
    } else {
      Serial.println("Failed to read DHT data.");
    }

    // Delay before the next reading
    delay(5000);
  }
