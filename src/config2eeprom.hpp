#ifndef config2eeprom_H
#define config2eeprom_H

#include "Arduino.h"
#include <EEPROM.h>
#include <ArduinoJson.h>

#define DEBUG_EEPROM

#define EEPROM_SIZE 4000
#define EEPROM_DOC_SIZE 254

// StaticJsonDocument type for EEPROM_DOC_SIZE
typedef StaticJsonDocument<EEPROM_DOC_SIZE> staticConfigDoc;

// Config2EEPROM class simplify usage of EEPROM as config storage
class config2eeprom
{
public:
    config2eeprom();
    config2eeprom(unsigned int beginByte);
    void save(staticConfigDoc doc);
    bool load(staticConfigDoc &doc);
    void clear();

private:
    // Starting address
    unsigned int _beginByte;
    unsigned int _docSize{254};
};

#endif // config2eeprom_H header guard