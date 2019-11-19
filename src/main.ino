#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <constants.h>
#include <ArduinoJson.h>

#include "static/index.html.gz.h"

IRrecv irrecv(IRIN);
decode_results results;


void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn();
  delay(400);
  setupCodes();

  jw.setHostname(getBoardCode().c_str());
  jw.subscribe(infoCallback);
  jw.enableAP(false);
  jw.enableAPFallback(true);
  jw.enableSTA(true);

  String ssid = getSsid();
  String password = getWifiPassword();

  Serial.println("Conn to " + ssid);

  jw.addNetwork(ssid.c_str(), password.c_str());
  pinMode(BUTTON_PIN, INPUT);
  pinMode(TRANSISTOR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  #ifdef TEST_ENVIRONMENT
    pinMode(2, OUTPUT);
  #endif

  webSetup();
  wsSetup();

  Serial.println("IR module");
}

void irReceiver()
{
  if (irrecv.decode(&results))
  {
    irrecv.resume();

    static unsigned long last_time = 0;
    if (millis() - last_time < IR_DEBOUNCE)
      return;
    last_time = millis();

    if (results.value < 1)
      return;
    if (results.decode_type < 1)
      return; //Remove this line if your Remote control is not supported
    if (results.bits < 1)
      return;
    char payload[32];
    snprintf_P(payload, sizeof(payload), PSTR("%u:%lu:%u"), results.decode_type, (unsigned long)results.value, results.bits);

    String codes[getIrCodesSize()]; 
    getIrCodes(codes);

    Serial.println(payload);
    sendCode(payload);    

    for (unsigned int i = 0; i < sizeof(codes); i++)
    {
      if (codes[i] == payload)
      {
        Serial.println("Toggle transistor");
        toggleTransistor();
        break;
      }
    }

    irrecv.resume(); // Receive the next value
  }
}

void blinkLed()
{
  if (LED_BLINK_SUPPORT)
  {
    static unsigned long last_time = 0;
    if (millis() - last_time < LED_BLINK_TIME)
      return;
    last_time = millis();
    toggleLed();
  }
}

void production()
{
  irReceiver();
  /*
  if (digitalRead(BUTTON_PIN))
  {
    Serial.println("Button restart");
    ESP.restart();
  }*/

  jw.loop();
  blinkLed();
}

#ifdef TEST_ENVIRONMENT
void test()
{
  if (digitalRead(BUTTON_PIN))
  {
    digitalWrite(2, !digitalRead(2));
    delay(200);
  }
  else
  {
    toggleLed();
    toggleTransistor();
    delay(200);
    toggleLed();
    delay(200);
    toggleLed();
    toggleTransistor();
    delay(400);
  }
}
#endif

void loop()
{
#ifdef TEST_ENVIRONMENT
  test();
#else
  production();
#endif
}