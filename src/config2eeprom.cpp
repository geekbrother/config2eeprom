#include "config2eeprom.hpp"

// Start with starting address 0 if not mentioned
config2eeprom::config2eeprom(){
    _beginByte = 0;
}
// Start with mentioned byte starting address
config2eeprom::config2eeprom(unsigned int beginByte){
    _beginByte = beginByte;
}

// Save staticDOC json hash to EEPROM
void config2eeprom::save( staticConfigDoc doc ){
    #ifdef DEBUG_EEPROM
        Serial.println("Config2EEPRom: Saving data.");
    #endif

    // Mount EEPROM partiton
    EEPROM.begin(EEPROM_DOC_SIZE);

    char serialized[EEPROM_DOC_SIZE];
    serializeMsgPack(doc, serialized);

    // Write to EEPROM byte by byte
    for(short n = 0; n < sizeof(serialized); n++) // automatically adjust for chars
    {
        EEPROM.write(n + _beginByte, serialized[n]);
    }

    // Commit and unmount
    EEPROM.commit();
    EEPROM.end();
}

// Load staticDOC json hash from EEPROM. Return false on error or empty.
bool config2eeprom::load(staticConfigDoc &doc){
    #ifdef DEBUG_EEPROM
        Serial.println("Config2EEProm: Loads from eeprom.");
    #endif
    // Mount EEPROM partiton
    EEPROM.begin(EEPROM_DOC_SIZE);

    char serialized[EEPROM_DOC_SIZE];

    // Read from EEPROM byte by byte
    for(short n = 0; n < sizeof(serialized); n++) // automatically adjust for chars
    {
        serialized[n] = EEPROM.read(n + _beginByte);
    }

    // Unmount eeprom
    EEPROM.end();

    // Deserialize from msgPack
    DeserializationError error = deserializeMsgPack(doc, serialized);

    // Check for errors
    if(error.code() != DeserializationError::Ok ){
        #ifdef DEBUG_EEPROM
            Serial.print("Config2EEProm got deserialization error:");
            Serial.println(error.c_str());
        #endif
        return false;
    }
    // Check for empty
    if(doc.size() == 0){
        #ifdef DEBUG_EEPROM
            Serial.println("Config2EEProm: got empty array from load().");
        #endif
        return false;
    }

    #ifdef DEBUG_EEPROM
        Serial.print("Config2EEProm: Deserialized document with size:");
        Serial.println(doc.size();
    #endif
    return true;
}

// Clear EEPROM
void config2eeprom::clear(){
    #ifdef DEBUG_EEPROM
        Serial.println("Config2EEProm: Clearing EEPROM.");
    #endif

    EEPROM.begin(EEPROM_DOC_SIZE);
    for (int i = 0; i < EEPROM_DOC_SIZE; i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    EEPROM.end();
}