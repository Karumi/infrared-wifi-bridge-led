#include "Embedis.h"
#include <EEPROM_Rotate.h>

EEPROM_Rotate EEPROMr;

void setupSettings() {
    EEPROMr.begin(SPI_FLASH_SEC_SIZE);
    
    Embedis::dictionary( F("EEPROM"),
        SPI_FLASH_SEC_SIZE,
        [](size_t pos) -> char { return EEPROMr.read(pos); },
        [](size_t pos, char value) { EEPROMr.write(pos, value); },
        []() { EEPROMr.commit(); }
    );

}

template<typename T> String getSetting(const String& key, T defaultValue) {
    String value;
    if (!Embedis::get(key, value)) value = String(defaultValue);
    return value;
}

template<typename T> String getSetting(const String& key, unsigned int index, T defaultValue) {
    return getSetting(key + String(index), defaultValue);
}

template<typename T> bool setSetting(const String& key, T value) {
    return Embedis::set(key, String(value));
}

template<typename T> bool setSetting(const String& key, unsigned int index, T value) {
    return setSetting(key + String(index), value);
}

bool delSetting(const String& key) {
    return Embedis::del(key);
}

bool delSetting(const String& key, unsigned int index) {
    return delSetting(key + String(index));
}

bool hasSetting(const String& key) {
    return getSetting(key, "").length() != 0;
}

bool hasSetting(const String& key, unsigned int index) {
    return getSetting(key, index, "").length() != 0;
}