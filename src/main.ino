#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <constants.h>

IRrecv irrecv(IRIN);
decode_results results;

AsyncWebServer server(80);
AsyncWebSocket _ws("/ws");  

std::vector<String> _codes;

#define NUM_CODES                "num-codes"
#define SSID_PARAMETER           "ssid"
#define PASSWORD_PARAMETER       "password"
#define CODE_NAME_PREFIX         "code"

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/html", "Not found");
} 

void setupCodes() {
  setupSettings();

  if(hasSetting(NUM_CODES)) {
    unsigned int numCodes = getSetting(NUM_CODES, "0").toInt();

    for(unsigned int i = 0; i < numCodes; i++) {
      if(hasSetting(CODE_NAME_PREFIX, i)) {
        String code = getSetting(CODE_NAME_PREFIX, i, "");
        if(code.length() > 0) {
          Serial.println("Add code from EEPROM" + code);
          _codes.push_back(code);
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();
  delay(400);  
  setupCodes();

  jw.setHostname(getBoardCode().c_str());
  jw.subscribe(infoCallback);
  jw.enableAP(false);
  jw.enableAPFallback(true);
  jw.enableSTA(true);

  String ssid = getSetting(SSID_PARAMETER, DEFAULT_SSID);
  String password = getSetting(PASSWORD_PARAMETER, DEFAULT_WIFI_PASSWORD);

  Serial.println("Conn to "+ ssid);

  jw.addNetwork(ssid.c_str(), password.c_str());  
  pinMode(BUTTON_PIN, INPUT);
  pinMode(TRANSISTOR_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  #ifdef TEST_ENVIRONMENT
  pinMode(2, OUTPUT);
  #endif

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", getIndex());
  });

  server.on("/relay", HTTP_POST, [](AsyncWebServerRequest *request){
      toggleTransistor();              
      request->send(200, "text/html", getIndex());
  });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/setwifi", HTTP_POST, [](AsyncWebServerRequest *request){               
        if (request->hasParam(SSID_PARAMETER, true) && request->hasParam(PASSWORD_PARAMETER, true)) {
            String ssid = request->getParam(SSID_PARAMETER, true)->value();
            String password = request->getParam(PASSWORD_PARAMETER, true)->value();  
            setSetting(SSID_PARAMETER, ssid);      
            setSetting(PASSWORD_PARAMETER, password);
            Serial.println("Setted new wifi credentials");
            ESP.restart();
        }
        request->send(200, "text/html", getIndex());
    });

    server.on("/setcodes", HTTP_POST, [](AsyncWebServerRequest *request){      
        if(request->hasParam(NUM_CODES, true)) {        
          String requestCode = request->getParam(NUM_CODES, true)->value();                    
          unsigned int numCodes = requestCode.toInt();          
          if(numCodes > 0) {                        
            _codes.clear();
            setSetting(NUM_CODES, numCodes);
            for(unsigned int i = 0; i < numCodes; i++)  {
              String codeName = CODE_NAME_PREFIX + String(i);              
              if(request->hasParam(codeName, true)) {
                String code = request->getParam(codeName, true)->value();
                Serial.println("Add code: "+code);
                _codes.push_back(code);
                setSetting(codeName, code);
              }               
            }        
          }
        }
        request->send(200, "text/html", getIndex());
    });
    server.onNotFound(notFound);
    _ws.onEvent(_onEvent);
    server.addHandler(&_ws);
    server.begin();

    Serial.println("IR module");
}

void irReceiver() {
  if (irrecv.decode(&results)) {       
    irrecv.resume();

     static unsigned long last_time = 0;
     if (millis() - last_time < IR_DEBOUNCE) return;
     last_time = millis();

    if (results.value < 1) return;    
    if (results.decode_type < 1) return;  //Remove this line if your Remote control is not supported
    if (results.bits < 1) return;
    char payload[32];
    snprintf_P(payload, sizeof(payload), PSTR("%u:%lu:%u"), results.decode_type, (unsigned long) results.value, results.bits);

    Serial.println(payload);
    _ws.textAll(payload);   
    
    for(unsigned int i=0; i < _codes.size(); i++) {      
      if(_codes[i] == payload) {
        Serial.println("Toggle transistor");
        toggleTransistor();                
        break;
      }
    }

    irrecv.resume();  // Receive the next value
  }
}

void blinkLed() {
  if(LED_BLINK_SUPPORT) {     
    static unsigned long last_time = 0;
    if (millis() - last_time < LED_BLINK_TIME) return;
    last_time = millis();
    toggleLed();  
  }
}

void production() {
  irReceiver();
  
  if(digitalRead(BUTTON_PIN)) {
    Serial.println("Button restart");
    ESP.restart();
  } 
  
  jw.loop();
  blinkLed();
}

#ifdef TEST_ENVIRONMENT
void test() {  
  if(digitalRead(BUTTON_PIN)) {
    digitalWrite(2, !digitalRead(2));
    delay(200);
  } else {    
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

void loop() {  
  #ifdef TEST_ENVIRONMENT
    test();
  #else
    production();
  #endif
}