#include <Arduino.h>
#include <ArduinoJson.h>
#include <constants.h>

#define NUM_CODES "num-codes"
#define SSID_PARAMETER "ssid"
#define PASSWORD_PARAMETER "password"
#define CODE_NAME_PREFIX "code"

std::vector<String> *_codes = new std::vector<String>();

unsigned int getIrCodesSize() {
  return _codes->size();  
}

void getIrCodes(String codes[]) {
  std::copy(_codes->begin(), _codes->end(), codes);
}

void setupCodes() {
  setupSettings();

  if (hasSetting(NUM_CODES))
  {
    unsigned int numCodes = getSetting(NUM_CODES, "0").toInt();

    for (unsigned int i = 0; i < numCodes; i++)
    {
      if (hasSetting(CODE_NAME_PREFIX, i))
      {
        String code = getSetting(CODE_NAME_PREFIX, i, "");
        if (code.length() > 0)
        {
          Serial.println("Add code from EEPROM" + code);
          _codes->push_back(code);
        }
      }
    }
  }
}

void setCodes(JsonArray codes) {
    size_t numCodes = codes.size();
    if(numCodes > 0) {
        _codes->clear();
        setSetting(NUM_CODES, numCodes);

        for (unsigned int i = 0; i < numCodes; i++) {
            String codeName = CODE_NAME_PREFIX + String(i);
            String code = codes[i];

            Serial.println("Add code: " + code);
            _codes->push_back(code);
            setSetting(codeName, code);
        }
    }
}

void setWifiCredentials(String ssid, String password) {    
      setSetting(SSID_PARAMETER, ssid);
      setSetting(PASSWORD_PARAMETER, password);
      Serial.println("Set WiFi credentials");
      ESP.restart();
}

String getSsid() {
  return getSetting(SSID_PARAMETER, DEFAULT_SSID);
}

String getWifiPassword() {
  return getSetting(PASSWORD_PARAMETER, DEFAULT_WIFI_PASSWORD);
}