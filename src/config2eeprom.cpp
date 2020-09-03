#include "config2eeprom.hpp"

// Start with starting address 0 if not mentioned
config2eeprom::config2eeprom()
{
    _beginByte = 0;
}
// Start with mentioned byte starting address
config2eeprom::config2eeprom(unsigned int beginByte)
{
    _beginByte = beginByte;
}

// Save staticDOC json hash to EEPROM
void config2eeprom::save(staticConfigDoc doc)
{
#ifdef DEBUG_EEPROM
    Serial.print("Config2EEPRom: Saving data from begin byte:");
    Serial.println(_beginByte);
#endif

    // Mount EEPROM partiton
    EEPROM.begin(EEPROM_SIZE);

    char serialized[EEPROM_DOC_SIZE];
    serializeMsgPack(doc, serialized);

    // Write to EEPROM byte by byte
    for (short n = 0; n < sizeof(serialized); n++) // automatically adjust for chars
    {
        EEPROM.write(n + _beginByte, serialized[n]);
    }

    // Commit and unmount
    if (!EEPROM.commit())
    {
        Serial.print("Config2EEPRom: Error while commit to eeprom.");
    }
    EEPROM.end();
}

// Load staticDOC json hash from EEPROM. Return false on error or empty.
bool config2eeprom::load(staticConfigDoc &doc)
{
#ifdef DEBUG_EEPROM
    Serial.print("Config2EEPRom: Loading data from begin byte:");
    Serial.println(_beginByte);
#endif

    // Mount EEPROM partiton
    EEPROM.begin(EEPROM_SIZE);

    char serialized[EEPROM_DOC_SIZE];

    // Read from EEPROM byte by byte
    for (short n = 0; n < sizeof(serialized); n++) // automatically adjust for chars
    {
        serialized[n] = EEPROM.read(n + _beginByte);
    }

    // Unmount eeprom
    EEPROM.end();

#ifdef DEBUG_EEPROM
    Serial.println("Config2EEPRom: Got bytes from eeprom:");
    Serial.println(serialized);
#endif

    // Deserialize from msgPack
    DeserializationError error = deserializeMsgPack(doc, serialized);

    // Check for errors
    if (error.code() != DeserializationError::Ok)
    {
#ifdef DEBUG_EEPROM
        Serial.print("Config2EEProm got deserialization error:");
        Serial.println(error.c_str());
#endif
        return false;
    }
    // Check for empty
    if (doc.size() == 0)
    {
#ifdef DEBUG_EEPROM
        Serial.println("Config2EEProm: got empty array from load().");
#endif
        return false;
    }

#ifdef DEBUG_EEPROM
    Serial.print("Config2EEProm: Deserialized document with size:");
    Serial.println(doc.size());
#endif
    return true;
}

// Clear EEPROM
void config2eeprom::clear()
{
#ifdef DEBUG_EEPROM
    Serial.print("Config2EEPRom: Clearing EEPROM begin byte:");
    Serial.println(_beginByte);
#endif

    EEPROM.begin(EEPROM_SIZE);
    for (int i = 0; i < EEPROM_DOC_SIZE; i++)
    {
        EEPROM.write(i + _beginByte, 0);
    }
    if (!EEPROM.commit())
    {
        Serial.print("Config2EEPRom: Error while commit to eeprom.");
    }
    EEPROM.end();
}