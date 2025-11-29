/**
 * @file AlarmScheduler.cpp
 * @brief Implementation of advanced alarm scheduling system
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 * @version 1.0.0
 */

#include "AlarmScheduler.h"

// ============================================================================
// PUBLIC METHOD IMPLEMENTATIONS
// ============================================================================

bool AlarmScheduler::begin(bool loadDefaults) {
    clear();
    
    DBG_ALM("[ALARM] Loading customizable alarms from SPIFFS...");
    loadCustomizablesFromJSON();
    
    if (loadDefaults && _num == 0) {
        DBG_ALM("[ALARM] No alarms found, creating defaults...");
        _createDefaultCustomizableAlarms();
    }
    
    DBG_ALM_PRINTF("[ALARM] System initialized with %u alarms\n", _num);
    return true;
}

uint8_t AlarmScheduler::add(uint8_t dayMask,
                            uint8_t hour,
                            uint8_t minute,
                            uint16_t intervalMin,
                            void (AlarmScheduler::*action)(uint16_t),
                            uint16_t parameter,
                            bool enabled)
{
    if (_num >= MAX_ALARMS) {
        DBG_ALM_PRINTF("[ALARM] Error: Maximum alarms reached (%u)\n", MAX_ALARMS);
        return 255;
    }
    
    Alarm &alarm = _alarms[_num];
    alarm.enabled        = enabled;
    alarm.dayMask        = (dayMask ? dayMask : DOW_ALL);
    alarm.hour           = hour;
    alarm.minute         = minute;
    alarm.intervalMin    = intervalMin;
    alarm.lastYearDay    = -1;
    alarm.lastMinute     = 255;
    alarm.lastHour       = 255;
    alarm.lastExecution  = 0;
    alarm.action         = action;
    alarm.externalAction = nullptr;
    alarm.externalAction0 = nullptr;
    alarm.parameter      = parameter;
    alarm.isCustomizable = false;
    alarm.webId          = -1;
    strcpy(alarm.typeString, "SYSTEM");
    
    DBG_ALM_PRINTF("[ALARM] Added method alarm idx=%u, days=0x%02X, %02u:%02u, interval=%u min, param=%u\n",
                   _num, dayMask, hour, minute, intervalMin, parameter);
    
    return _num++;
}

uint8_t AlarmScheduler::addExternal(uint8_t dayMask,
                                    uint8_t hour,
                                    uint8_t minute,
                                    uint16_t intervalMin,
                                    void (*ext)(uint16_t),
                                    uint16_t parameter,
                                    bool enabled)
{
    if (_num >= MAX_ALARMS) {
        DBG_ALM_PRINTF("[ALARM] Error: Maximum alarms reached (%u)\n", MAX_ALARMS);
        return 255;
    }
    
    Alarm &alarm = _alarms[_num];
    alarm.enabled        = enabled;
    alarm.dayMask        = (dayMask ? dayMask : DOW_ALL);
    alarm.hour           = hour;
    alarm.minute         = minute;
    alarm.intervalMin    = intervalMin;
    alarm.lastYearDay    = -1;
    alarm.lastMinute     = 255;
    alarm.lastHour       = 255;
    alarm.lastExecution  = 0;
    alarm.action         = nullptr;
    alarm.externalAction = ext;
    alarm.externalAction0 = nullptr;
    alarm.parameter      = parameter;
    alarm.isCustomizable = false;
    alarm.webId          = -1;
    strcpy(alarm.typeString, "SYSTEM");
    
    DBG_ALM_PRINTF("[ALARM] Added external alarm idx=%u, days=0x%02X, %02u:%02u, interval=%u min, param=%u\n",
                   _num, dayMask, hour, minute, intervalMin, parameter);
    
    return _num++;
}

uint8_t AlarmScheduler::addExternal0(uint8_t dayMask,
                                     uint8_t hour,
                                     uint8_t minute,
                                     uint16_t intervalMin,
                                     void (*ext0)(),
                                     bool enabled)
{
    if (_num >= MAX_ALARMS) {
        DBG_ALM_PRINTF("[ALARM] Error: Maximum alarms reached (%u)\n", MAX_ALARMS);
        return 255;
    }
    
    Alarm &alarm = _alarms[_num];
    alarm.enabled        = enabled;
    alarm.dayMask        = (dayMask ? dayMask : DOW_ALL);
    alarm.hour           = hour;
    alarm.minute         = minute;
    alarm.intervalMin    = intervalMin;
    alarm.lastYearDay    = -1;
    alarm.lastMinute     = 255;
    alarm.lastHour       = 255;
    alarm.lastExecution  = 0;
    alarm.action         = nullptr;
    alarm.externalAction = nullptr;
    alarm.externalAction0 = ext0;
    alarm.parameter      = 0;
    alarm.isCustomizable = false;
    alarm.webId          = -1;
    strcpy(alarm.typeString, "SYSTEM");
    
    DBG_ALM_PRINTF("[ALARM] Added external0 alarm idx=%u, days=0x%02X, %02u:%02u, interval=%u min\n",
                   _num, dayMask, hour, minute, intervalMin);
    
    return _num++;
}

void AlarmScheduler::check() {
    if (!getLocalTime(&t)) return;
    
    uint8_t currentHour     = t.tm_hour;
    uint8_t currentMinute   = t.tm_min;
    uint8_t currentDayMask  = _dayMaskFromWeekday(t.tm_wday);
    int     currentYearDay  = t.tm_yday;
    time_t  now             = time(nullptr);

    for (uint8_t i = 0; i < _num; ++i) {
        Alarm &alarm = _alarms[i];
        
        if (!alarm.enabled) continue;
        if (!(alarm.dayMask & currentDayMask)) continue;

        bool trigger = false;

        // Interval alarm logic
        if (alarm.intervalMin > 0) {
            if (alarm.lastExecution == 0) {
                // First execution: check anchor
                bool anchorOk = true;
                if (alarm.hour   != ALARM_WILDCARD && alarm.hour   != currentHour)   anchorOk = false;
                if (alarm.minute != ALARM_WILDCARD && alarm.minute != currentMinute) anchorOk = false;
                if (anchorOk) trigger = true;
            } else if ((now - alarm.lastExecution) >= (time_t)(alarm.intervalMin * 60)) {
                trigger = true;
            }
        } 
        // Fixed/wildcard alarm logic
        else {
            bool matchHour = (alarm.hour == ALARM_WILDCARD || alarm.hour == currentHour);
            bool matchMinute = (alarm.minute == ALARM_WILDCARD || alarm.minute == currentMinute);

            if (matchHour && matchMinute) {
                bool alreadyExecuted = false;
                
                if (alarm.hour == ALARM_WILDCARD) {
                    alreadyExecuted = (alarm.lastYearDay == currentYearDay && 
                                      alarm.lastMinute == currentMinute &&
                                      alarm.lastHour == currentHour);
                } else {
                    alreadyExecuted = (alarm.lastYearDay == currentYearDay && 
                                      alarm.lastMinute == currentMinute);
                }
                
                if (!alreadyExecuted) {
                    trigger = true;
                }
            }
        }

        if (!trigger) continue;

        // Execute appropriate action
        if (alarm.action) {
            (this->*alarm.action)(alarm.parameter);
            DBG_ALM_PRINTF("[ALARM] idx=%u executed - member method, param=%u\n", i, alarm.parameter);
        } else if (alarm.externalAction) {
            alarm.externalAction(alarm.parameter);
            DBG_ALM_PRINTF("[ALARM] idx=%u executed - external function, param=%u\n", i, alarm.parameter);
        } else if (alarm.externalAction0) {
            alarm.externalAction0();
            DBG_ALM_PRINTF("[ALARM] idx=%u executed - external function no params\n", i);
        }

        // Update cache
        alarm.lastYearDay    = currentYearDay;
        alarm.lastMinute     = currentMinute;
        alarm.lastHour       = currentHour;
        alarm.lastExecution  = now;
    }
}

void AlarmScheduler::disable(uint8_t idx) { 
    if (idx < _num) {
        _alarms[idx].enabled = false;
        DBG_ALM_PRINTF("[ALARM] Alarm idx=%u disabled\n", idx);
    }
}

void AlarmScheduler::enable(uint8_t idx) { 
    if (idx < _num) {
        _alarms[idx].enabled = true;
        DBG_ALM_PRINTF("[ALARM] Alarm idx=%u enabled\n", idx);
    }
}

void AlarmScheduler::clear() { 
    _num = 0; 
    _nextWebId = 1;
    DBG_ALM("[ALARM] All alarms cleared\n");
}

uint8_t AlarmScheduler::count() const { 
    return _num; 
}

const Alarm* AlarmScheduler::get(uint8_t idx) const { 
    return (idx < _num) ? &_alarms[idx] : nullptr; 
}

Alarm* AlarmScheduler::getMutable(uint8_t idx) { 
    return (idx < _num) ? &_alarms[idx] : nullptr; 
}

void AlarmScheduler::resetCache() {
    for (uint8_t i = 0; i < _num; ++i) {
        _alarms[i].lastYearDay = -1;
        _alarms[i].lastMinute = 255;
        _alarms[i].lastHour = 255;
        _alarms[i].lastExecution = 0;
    }
    DBG_ALM_PRINTF("[ALARM] Cache of %u alarms reset\n", _num);
}

// ============================================================================
// CUSTOMIZABLE ALARM MANAGEMENT - SPANISH NAMES
// ============================================================================

uint8_t AlarmScheduler::addPersonalizable(const char* nombre, const char* descripcion,
                                         uint8_t mascaraDias, uint8_t hora, uint8_t minuto,
                                         const char* tipoString, uint16_t parametro,
                                         void (*callback)(uint16_t), bool habilitada) {
    if (_num >= MAX_ALARMS) {
        DBG_ALM("Error: Maximum alarms reached");
        return MAX_ALARMS;
    }
    
    Alarm& alarma = _alarms[_num];
    alarma.enabled = habilitada;
    alarma.dayMask = mascaraDias;
    alarma.hour = hora;
    alarma.minute = minuto;
    alarma.intervalMin = 0;
    alarma.parameter = parametro;
    alarma.externalAction = callback;
    alarma.action = nullptr;
    alarma.externalAction0 = nullptr;
    
    strncpy(alarma.name, nombre, sizeof(alarma.name) - 1);
    alarma.name[sizeof(alarma.name) - 1] = '\0';
    
    strncpy(alarma.description, descripcion, sizeof(alarma.description) - 1);
    alarma.description[sizeof(alarma.description) - 1] = '\0';
    
    strncpy(alarma.typeString, tipoString, sizeof(alarma.typeString) - 1);
    alarma.typeString[sizeof(alarma.typeString) - 1] = '\0';
    
    alarma.isCustomizable = true;
    alarma.webId = _generateNewWebId();
    
    uint8_t idx = _num;
    _num++;
    
    DBG_ALM_PRINTF("Customizable alarm created - Index: %d, Web ID: %d", idx, alarma.webId);
    
    saveCustomizablesToJSON();
    
    return idx;
}

bool AlarmScheduler::modificarPersonalizable(int idWeb, const char* nombre, const char* descripcion,
                                           uint8_t mascaraDias, uint8_t hora, uint8_t minuto,
                                           const char* tipoString, bool habilitada,
                                           void (*callback)(uint16_t), uint16_t parametro) {
    uint8_t idx = _findIndexByWebId(idWeb);
    if (idx >= MAX_ALARMS) {
        DBG_ALM("Error: Alarm not found");
        return false;
    }
    
    Alarm& alarma = _alarms[idx];
    
    if (!alarma.isCustomizable) {
        DBG_ALM("Error: Alarm is not customizable");
        return false;
    }
    
    if (callback == nullptr) {
        DBG_ALM("Error: Callback is NULL");
        return false;
    }
    
    alarma.enabled = habilitada;
    alarma.dayMask = mascaraDias;
    alarma.hour = hora;
    alarma.minute = minuto;
    alarma.externalAction = callback;
    alarma.parameter = parametro;
    alarma.action = nullptr;
    alarma.externalAction0 = nullptr;
    
    strncpy(alarma.name, nombre, sizeof(alarma.name) - 1);
    alarma.name[sizeof(alarma.name) - 1] = '\0';
    
    strncpy(alarma.description, descripcion, sizeof(alarma.description) - 1);
    alarma.description[sizeof(alarma.description) - 1] = '\0';
    
    strncpy(alarma.typeString, tipoString, sizeof(alarma.typeString) - 1);
    alarma.typeString[sizeof(alarma.typeString) - 1] = '\0';
    
    alarma.lastYearDay = -1;
    alarma.lastMinute = 255;
    alarma.lastHour = 255;
    alarma.lastExecution = 0;
    
    saveCustomizablesToJSON();
    
    return true;
}

bool AlarmScheduler::eliminarPersonalizable(int idWeb) {
    uint8_t idx = _findIndexByWebId(idWeb);
    if (idx >= MAX_ALARMS) {
        DBG_ALM("Error: Alarm not found");
        return false;
    }
    
    if (!_alarms[idx].isCustomizable) {
        DBG_ALM("Error: Alarm is not customizable");
        return false;
    }
    
    for (uint8_t i = idx; i < _num - 1; i++) {
        _alarms[i] = _alarms[i + 1];
    }
    
    _alarms[_num - 1] = Alarm();
    _num--;
    
    DBG_ALM("Customizable alarm deleted");
    
    saveCustomizablesToJSON();
    
    return true;
}

bool AlarmScheduler::habilitarPersonalizable(int idWeb, bool estado) {
    uint8_t idx = _findIndexByWebId(idWeb);
    if (idx >= MAX_ALARMS) {
        DBG_ALM("Error: Alarm not found");
        return false;
    }
    
    if (!_alarms[idx].isCustomizable) {
        DBG_ALM("Error: Alarm is not customizable");
        return false;
    }
    
    _alarms[idx].enabled = estado;
    
    if (estado) {
        _alarms[idx].lastYearDay = -1;
        _alarms[idx].lastMinute = 255;
        _alarms[idx].lastHour = 255;
        _alarms[idx].lastExecution = 0;
    }
    
    DBG_ALM_PRINTF("Customizable alarm %s", estado ? "enabled" : "disabled");
    
    saveCustomizablesToJSON();
    
    return true;
}

String AlarmScheduler::obtenerPersonalizablesJSON() {
    JsonDocument doc;
    doc["version"] = "1.0";
    doc["timestamp"] = millis();
    
    uint8_t customizable = 0;
    for (uint8_t i = 0; i < _num; i++) {
        if (_alarms[i].isCustomizable) {
            customizable++;
        }
    }
    
    doc["total"] = customizable;
    
    JsonArray alarmsArray = doc.createNestedArray("alarms");
    
    for (uint8_t i = 0; i < _num; i++) {
        const Alarm& alarm = _alarms[i];
        
        if (!alarm.isCustomizable) continue;
        
        JsonObject alarmObj = alarmsArray.createNestedObject();
        
        alarmObj["id"] = alarm.webId;
        alarmObj["name"] = alarm.name;
        alarmObj["description"] = alarm.description;
        
        int day = 0;
        if (alarm.dayMask == DOW_ALL) {
            day = 0;
        } else {
            for (int d = 0; d < 7; d++) {
                if (alarm.dayMask & (1 << d)) {
                    day = d + 1;
                    break;
                }
            }
        }
        
        alarmObj["day"] = day;
        alarmObj["dayName"] = _dayToString(day);
        alarmObj["hour"] = alarm.hour;
        alarmObj["minute"] = alarm.minute;
        alarmObj["action"] = alarm.typeString;
        alarmObj["parameter"] = alarm.parameter;
        alarmObj["enabled"] = alarm.enabled;
        
        char timeFormatted[8];
        sprintf(timeFormatted, "%02d:%02d", alarm.hour, alarm.minute);
        alarmObj["timeText"] = timeFormatted;
        
        alarmObj["arrayIndex"] = i;
    }
    
    String result;
    serializeJson(doc, result);
    
    return result;
}

String AlarmScheduler::obtenerEstadisticasJSON() {
    JsonDocument doc;
    
    doc["module"] = "AlarmScheduler";
    doc["version"] = "1.0";
    doc["timestamp"] = millis();
    
    uint8_t system = 0, customizable = 0, enabled = 0, disabled = 0;
    
    for (uint8_t i = 0; i < _num; i++) {
        if (_alarms[i].isCustomizable) {
            customizable++;
        } else {
            system++;
        }
        
        if (_alarms[i].enabled) {
            enabled++;
        } else {
            disabled++;
        }
    }
    
    doc["totalAlarms"] = _num;
    doc["system"] = system;
    doc["customizable"] = customizable;
    doc["enabled"] = enabled;
    doc["disabled"] = disabled;
    doc["freeSpace"] = MAX_ALARMS - _num;
    doc["maxAlarms"] = MAX_ALARMS;
    doc["nextWebId"] = _nextWebId;
    doc["jsonFile"] = "/customizable_alarms.json";
    doc["fileExists"] = SPIFFS.exists("/customizable_alarms.json");
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        doc["currentTime"]["valid"] = true;
        doc["currentTime"]["hour"] = timeinfo.tm_hour;
        doc["currentTime"]["minute"] = timeinfo.tm_min;
        doc["currentTime"]["weekday"] = timeinfo.tm_wday;
        doc["currentTime"]["yearday"] = timeinfo.tm_yday;
    } else {
        doc["currentTime"]["valid"] = false;
    }
    
    String result;
    serializeJson(doc, result);
    
    return result;
}

bool AlarmScheduler::cargarPersonalizablesDesdeJSON() {
    const char* file = "/customizable_alarms.json";
    
    if (!SPIFFS.exists(file)) {
        DBG_ALM("Alarm file doesn't exist, creating defaults");
        _createDefaultCustomizableAlarms();
        return saveCustomizablesToJSON();
    }
    
    File f = SPIFFS.open(file, "r");
    if (!f) {
        DBG_ALM("Error opening alarm file");
        return false;
    }
    
    String content = f.readString();
    f.close();
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, content);
    
    if (error) {
        DBG_ALM_PRINTF("Error parsing JSON: %s", error.c_str());
        return false;
    }
    
    // Remove existing customizable alarms
    for (int i = _num - 1; i >= 0; i--) {
        if (_alarms[i].isCustomizable) {
            for (uint8_t j = i; j < _num - 1; j++) {
                _alarms[j] = _alarms[j + 1];
            }
            _num--;
        }
    }
    
    JsonArray alarmsArray = doc["alarms"];
    int loaded = 0;
    
    for (JsonObject alarmObj : alarmsArray) {
        if (_num >= MAX_ALARMS) {
            DBG_ALM("Maximum alarms reached, ignoring remaining");
            break;
        }
        
        const char* name = alarmObj["name"] | "";
        const char* description = alarmObj["description"] | "";
        int day = alarmObj["day"] | 0;
        uint8_t hour = alarmObj["hour"] | 0;
        uint8_t minute = alarmObj["minute"] | 0;
        const char* typeString = alarmObj["action"] | "SYSTEM";
        bool enabled = alarmObj["enabled"] | true;
        int webId = alarmObj["id"] | -1;
        
        if (strlen(name) == 0 || hour > 23 || minute > 59 || webId <= 0) {
            DBG_ALM_PRINTF("Invalid alarm ignored: %s", name);
            continue;
        }
        
        uint8_t dayMask;
        if (day == 0) {
            dayMask = DOW_ALL;
        } else {
            dayMask = 1 << (day - 1);
        }
        
        Alarm& alarm = _alarms[_num];
        alarm.enabled = enabled;
        alarm.dayMask = dayMask;
        alarm.hour = hour;
        alarm.minute = minute;
        alarm.intervalMin = 0;
        alarm.parameter = alarmObj["parameter"] | 0;
        
        strncpy(alarm.name, name, sizeof(alarm.name) - 1);
        alarm.name[sizeof(alarm.name) - 1] = '\0';
        
        strncpy(alarm.description, description, sizeof(alarm.description) - 1);
        alarm.description[sizeof(alarm.description) - 1] = '\0';
        
        strncpy(alarm.typeString, typeString, sizeof(alarm.typeString) - 1);
        alarm.typeString[sizeof(alarm.typeString) - 1] = '\0';
        
        alarm.isCustomizable = true;
        alarm.webId = webId;
        alarm.externalAction = nullptr;
        
        if (webId >= _nextWebId) {
            _nextWebId = webId + 1;
        }
        
        _num++;
        loaded++;
        
        DBG_ALM_PRINTF("Alarm loaded: %s (%s %02d:%02d)", 
                      name, _dayToString(day).c_str(), hour, minute);
    }
    
    DBG_ALM_PRINTF("Customizable alarms loaded: %d", loaded);
    return true;
}

bool AlarmScheduler::guardarPersonalizablesEnJSON() {
    const char* file = "/customizable_alarms.json";
    
    JsonDocument doc;
    doc["version"] = "1.0";
    doc["timestamp"] = millis();
    
    uint8_t customizable = 0;
    for (uint8_t i = 0; i < _num; i++) {
        if (_alarms[i].isCustomizable) {
            customizable++;
        }
    }
    
    doc["total"] = customizable;
    
    JsonArray alarmsArray = doc.createNestedArray("alarms");
    
    for (uint8_t i = 0; i < _num; i++) {
        const Alarm& alarm = _alarms[i];
        
        if (!alarm.isCustomizable) continue;
        
        JsonObject alarmObj = alarmsArray.createNestedObject();
        
        alarmObj["id"] = alarm.webId;
        alarmObj["name"] = alarm.name;
        alarmObj["description"] = alarm.description;
        
        int day = 0;
        if (alarm.dayMask == DOW_ALL) {
            day = 0;
        } else {
            for (int d = 0; d < 7; d++) {
                if (alarm.dayMask & (1 << d)) {
                    day = d + 1;
                    break;
                }
            }
        }
        
        alarmObj["day"] = day;
        alarmObj["hour"] = alarm.hour;
        alarmObj["minute"] = alarm.minute;
        alarmObj["action"] = alarm.typeString;
        alarmObj["enabled"] = alarm.enabled;
        alarmObj["parameter"] = alarm.parameter;
    }
    
    File f = SPIFFS.open(file, "w");
    if (!f) {
        DBG_ALM("Error creating JSON file");
        return false;
    }
    
    size_t bytesWritten = serializeJson(doc, f);
    f.close();
    
    if (bytesWritten == 0) {
        DBG_ALM("Error writing JSON - 0 bytes written");
        return false;
    }
    
    DBG_ALM_PRINTF("JSON saved successfully: %d alarms, %d bytes", customizable, bytesWritten);
    
    return true;
}

// ============================================================================
// CUSTOMIZABLE ALARM MANAGEMENT - ENGLISH ALIASES
// ============================================================================

uint8_t AlarmScheduler::addCustomizable(const char* name, const char* description,
                                        uint8_t dayMask, uint8_t hour, uint8_t minute,
                                        const char* typeString, uint16_t parameter,
                                        void (*callback)(uint16_t), bool enabled) {
    return addPersonalizable(name, description, dayMask, hour, minute, typeString, parameter, callback, enabled);
}

bool AlarmScheduler::modifyCustomizable(int webId, const char* name, const char* description,
                                        uint8_t dayMask, uint8_t hour, uint8_t minute,
                                        const char* typeString, bool enabled,
                                        void (*callback)(uint16_t), uint16_t parameter) {
    return modificarPersonalizable(webId, name, description, dayMask, hour, minute, typeString, enabled, callback, parameter);
}

bool AlarmScheduler::deleteCustomizable(int webId) {
    return eliminarPersonalizable(webId);
}

bool AlarmScheduler::enableCustomizable(int webId, bool state) {
    return habilitarPersonalizable(webId, state);
}

String AlarmScheduler::getCustomizablesJSON() {
    return obtenerPersonalizablesJSON();
}

String AlarmScheduler::getStatisticsJSON() {
    return obtenerEstadisticasJSON();
}

bool AlarmScheduler::loadCustomizablesFromJSON() {
    return cargarPersonalizablesDesdeJSON();
}

bool AlarmScheduler::saveCustomizablesToJSON() {
    return guardarPersonalizablesEnJSON();
}

// ============================================================================
// PRIVATE HELPER METHODS
// ============================================================================

uint8_t AlarmScheduler::_dayMaskFromWeekday(int weekday) {
    return (weekday >= 0 && weekday <= 6) ? (1 << weekday) : 0;
}

uint8_t AlarmScheduler::_findIndexByWebId(int webId) {
    for (uint8_t i = 0; i < _num; i++) {
        if (_alarms[i].isCustomizable && _alarms[i].webId == webId) {
            return i;
        }
    }
    return MAX_ALARMS;
}

int AlarmScheduler::_generateNewWebId() {
    int maxId = 0;
    for (uint8_t i = 0; i < _num; i++) {
        if (_alarms[i].isCustomizable && _alarms[i].webId > maxId) {
            maxId = _alarms[i].webId;
        }
    }
    return maxId + 1;
}

String AlarmScheduler::_dayToString(int day) {
    switch (day) {
        case 0: return "Every day";
        case 1: return "Sunday";
        case 2: return "Monday";
        case 3: return "Tuesday";
        case 4: return "Wednesday";
        case 5: return "Thursday";
        case 6: return "Friday";
        case 7: return "Saturday";
        default: return "Invalid day";
    }
}

void AlarmScheduler::_createDefaultCustomizableAlarms() {
    DBG_ALM("Not creating default alarms - will be created from web interface");
}

void AlarmScheduler::printAllAlarms() {
    Serial.println("\n========== ALARM LIST ==========");
    Serial.printf("Total registered alarms: %u/%u\n", _num, MAX_ALARMS);
    Serial.printf("Next Web ID: %d\n", _nextWebId);
    Serial.println();
    
    if (_num == 0) {
        Serial.println("No alarms registered");
        return;
    }
    
    for (uint8_t i = 0; i < _num; i++) {
        const Alarm& alarm = _alarms[i];
        
        Serial.printf("========== ALARM INDEX: %u ==========\n", i);
        Serial.printf("Web ID: %d\n", alarm.webId);
        Serial.printf("Name: '%s'\n", alarm.name);
        Serial.printf("Description: '%s'\n", alarm.description);
        Serial.printf("Type: '%s'\n", alarm.typeString);
        Serial.printf("Customizable: %s\n", alarm.isCustomizable ? "YES" : "NO");
        Serial.printf("Hour: %u\n", alarm.hour);
        Serial.printf("Minute: %u\n", alarm.minute);
        Serial.printf("Interval (min): %u\n", alarm.intervalMin);
        Serial.printf("Day Mask: 0x%02X\n", alarm.dayMask);
        Serial.printf("Enabled: %s\n", alarm.enabled ? "YES" : "NO");
        Serial.printf("Parameter: %u\n", alarm.parameter);
        Serial.printf("Has callback: %s\n", alarm.externalAction ? "YES" : "NO");
        Serial.println();
    }
    
    Serial.println("========== END ALARM LIST ==========\n");
}
