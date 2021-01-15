#ifdef ARDUINO_M5Stick_C
#include <M5StickC.h>
#include "AXP192.h"
#else
#include <Arduino.h>
#endif

#include <HardwareSerial.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

#include "setup.h" //Configure your setup here
#include "mqttserial.h"
#include "converters.h"
#include "comm.h"
#include "mqtt.h"

Converter converter;
char registryIDs[32];//Holds the registrys to query
bool busy=false;

bool contains(char array[], int size, int value)
{
  for (int i = 0; i < size; i++)
  {
    if (array[i] == value)
      return true;
  }
  return false;
}

//Converts to string and add the value to the JSON message
void updateValues(char regID)
{
  LabelDef *labels[128];
  int num = 0;
  converter.getLabels(regID, labels, num);
  for (size_t i = 0; i < num; i++)
  {
    snprintf(jsonbuff + strlen(jsonbuff),MAX_MSG_SIZE - strlen(jsonbuff), "\"%s\":\"%s\",", labels[i]->label, labels[i]->asString);
  }

}

void extraLoop()
{
  client.loop();
  ArduinoOTA.handle();
  while (busy){//Stop processing during OTA
    ArduinoOTA.handle();
  }
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.printf("Connecting to %s\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    if (i++ == 100)
    {
      esp_restart();
    }
  }
  Serial.printf("Connected. IP Address: %s\n", WiFi.localIP().toString().c_str());
}

void initRegistries(){
    //getting the list of registries to query from the selected values
  int i = 0;
  for (auto &&label : labelDefs)
  {
    if (!contains(registryIDs, sizeof(registryIDs), label.registryID))
    {
      registryIDs[i++] = label.registryID;
    }
  }
  if (i == 0)
  {
    Serial.printf("ERROR - No values selected in the include file. Stopping.\n");
    while (true)
    {
      delay(200);
    }
  }
  for (; i < 32; i++) //initialize the rest to 0xFF
  {
    registryIDs[i] = 0xFF;
  }
  //calling for registry values

}

void setup()
{
  Serial.begin(115200);
#ifdef ARDUINO_M5Stick_C
  M5.begin();
  M5.Axp.EnableCoulombcounter();
#endif
  MySerial.begin(9600, SERIAL_8E1, RX_PIN, TX_PIN);
  pinMode(PIN_THERM, OUTPUT);
  digitalWrite(PIN_THERM, HIGH);

  EEPROM.begin(10);

  setup_wifi();
  ArduinoOTA.setHostname("ESPAltherma");
  ArduinoOTA.onStart([]() {
    busy = true;
  });
  ArduinoOTA.begin();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setBufferSize(MAX_MSG_SIZE); //to support large json message
  client.setCallback(callback);
  client.setServer(MQTT_SERVER, MQTT_PORT);
  mqttSerial.begin(&client, "espaltherma/log");
  reconnect();

  readEEPROM();

  initRegistries();
  mqttSerial.print("ESPAltherma started!");
}

void loop()
{
  if (!client.connected())
  { //(re)connect to MQTT if needed
    reconnect();
  }
  //Querying all registries
  for (size_t i = 0; (i < 32) && registryIDs[i] != 0xFF; i++)
  {
    char buff[64] = {0};
    int tries = 0;
    while (!queryRegistry(registryIDs[i], buff) && tries++ < 3)
    {
      mqttSerial.print("Retrying...");
      delay(1000);
    }
    if (registryIDs[i] == buff[1]) //if replied registerID is coherent with the command
    {
      converter.readRegistryValues(buff); //process all values from the register
      updateValues(registryIDs[i]);       //send them in mqtt
      unsigned long start = millis();
      while (millis() < start + 500) //wait .5sec between registries
      {
        extraLoop();
      }
    }
  }
  sendValues();//Send the full json message
  Serial.printf("Done. Waiting %d sec...\n", FREQUENCY / 1000);
  auto start = millis();
  while (millis() <= start + FREQUENCY)
  {
    extraLoop();
  }
}