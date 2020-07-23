#ifndef config2eeprom_H
#define config2eeprom_H

#include "Arduino.h"
#include <EEPROM.h>
#include <ArduinoJson.h>

#define EEPROM_DOC_SIZE 254 // Size of the config document
#define EEPROM_STARTING_ADDR 0 // EEPROM starting address

// StaticJsonDocument type for EEPROM_DOC_SIZE
typedef StaticJsonDocument<EEPROM_DOC_SIZE> staticConfigDoc;

class config2eeprom {
    public:
        config2eeprom();
        void save(staticConfigDoc doc);
        bool load(staticConfigDoc &doc);
        void clear();
};

#endif // config2eeprom_H header guard