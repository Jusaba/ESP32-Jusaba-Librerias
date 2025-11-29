/**
 * @file Debug.h
 * @brief Debug template for AlarmScheduler library
 * 
 * This is an EXAMPLE template. Copy this file to your project and customize it.
 * See templates/README.md for detailed instructions.
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

// ============================================================================
// DEBUG CONFIGURATION
// ============================================================================

// Master debug switch
#define DEBUG_ENABLED 1

// AlarmScheduler debug flag
#define DEBUG_ALARMSCHEDULER 1

// ============================================================================
// ALARMSCHEDULER DEBUG MACROS
// ============================================================================

#if DEBUG_ENABLED && DEBUG_ALARMSCHEDULER
    #define DBG_ALM(x) Serial.println(String("[ALARM] ") + x)
    #define DBG_ALM_PRINT(x) Serial.print(String("[ALARM] ") + x)
    #define DBG_ALM_PRINTF(fmt, ...) Serial.printf("[ALARM] " fmt "\n", ##__VA_ARGS__)
#else
    #define DBG_ALM(x)
    #define DBG_ALM_PRINT(x)
    #define DBG_ALM_PRINTF(fmt, ...)
#endif

// ============================================================================
// DEBUG HELPER CLASS - AlarmScheduler Extensions
// ============================================================================

class DebugHelper {
public:
    /**
     * @brief Print alarm schedule summary
     * @param scheduler Pointer to AlarmScheduler instance
     */
    static void printAlarmSummary(class AlarmScheduler* scheduler) {
        if (!scheduler) return;
        
        Serial.println("\n========== ALARM SUMMARY ==========");
        Serial.printf("Total alarms: %u\n", scheduler->count());
        
        uint8_t enabled = 0, disabled = 0, customizable = 0;
        for (uint8_t i = 0; i < scheduler->count(); i++) {
            const Alarm* alarm = scheduler->get(i);
            if (alarm->enabled) enabled++;
            else disabled++;
            if (alarm->isCustomizable) customizable++;
        }
        
        Serial.printf("Enabled: %u | Disabled: %u\n", enabled, disabled);
        Serial.printf("Customizable: %u | System: %u\n", customizable, scheduler->count() - customizable);
        Serial.println("===================================\n");
    }
    
    /**
     * @brief Print day mask in human-readable format
     * @param dayMask Bitmask of days (DOW_DOMINGO | DOW_LUNES | ...)
     */
    static void printDayMask(uint8_t dayMask) {
        Serial.print("Days: ");
        
        if (dayMask == 0x7F) { // DOW_ALL
            Serial.println("Every day");
            return;
        }
        
        const char* days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        bool first = true;
        
        for (int i = 0; i < 7; i++) {
            if (dayMask & (1 << i)) {
                if (!first) Serial.print(", ");
                Serial.print(days[i]);
                first = false;
            }
        }
        Serial.println();
    }
    
    /**
     * @brief Print time in HH:MM format
     * @param hour Hour (0-23)
     * @param minute Minute (0-59)
     */
    static void printTime(uint8_t hour, uint8_t minute) {
        Serial.printf("%02u:%02u", hour, minute);
    }
    
    /**
     * @brief Print current ESP32 time info
     */
    static void printCurrentTime() {
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("Time: NOT SYNCHRONIZED");
            return;
        }
        
        Serial.println("\n========== CURRENT TIME ==========");
        Serial.printf("Date: %04d-%02d-%02d\n", 
                     timeinfo.tm_year + 1900, 
                     timeinfo.tm_mon + 1, 
                     timeinfo.tm_mday);
        Serial.printf("Time: %02d:%02d:%02d\n", 
                     timeinfo.tm_hour, 
                     timeinfo.tm_min, 
                     timeinfo.tm_sec);
        Serial.printf("Weekday: %d (0=Sun)\n", timeinfo.tm_wday);
        Serial.printf("Year day: %d\n", timeinfo.tm_yday);
        Serial.println("==================================\n");
    }
    
    /**
     * @brief Print SPIFFS information
     */
    static void printSPIFFSInfo() {
        Serial.println("\n========== SPIFFS INFO ==========");
        Serial.printf("Total bytes: %u\n", SPIFFS.totalBytes());
        Serial.printf("Used bytes: %u\n", SPIFFS.usedBytes());
        Serial.printf("Free bytes: %u\n", SPIFFS.totalBytes() - SPIFFS.usedBytes());
        Serial.printf("Usage: %.1f%%\n", 
                     (float)SPIFFS.usedBytes() / SPIFFS.totalBytes() * 100.0f);
        Serial.println("=================================\n");
    }
    
    /**
     * @brief List files in SPIFFS
     * @param path Directory path (default: "/")
     */
    static void listSPIFFSFiles(const char* path = "/") {
        Serial.println("\n========== SPIFFS FILES ==========");
        
        File root = SPIFFS.open(path);
        if (!root || !root.isDirectory()) {
            Serial.println("Failed to open directory");
            return;
        }
        
        File file = root.openNextFile();
        int count = 0;
        
        while (file) {
            Serial.printf("%s - %u bytes\n", file.name(), file.size());
            file = root.openNextFile();
            count++;
        }
        
        Serial.printf("\nTotal files: %d\n", count);
        Serial.println("==================================\n");
    }
    
    /**
     * @brief Print alarm JSON content
     * @param jsonString JSON string to print
     */
    static void printAlarmJSON(const String& jsonString) {
        Serial.println("\n========== ALARM JSON ==========");
        Serial.println(jsonString);
        Serial.println("================================\n");
    }
    
    /**
     * @brief Print free heap memory
     */
    static void printFreeHeap() {
        Serial.printf("Free Heap: %u bytes\n", ESP.getFreeHeap());
    }
    
    /**
     * @brief Print separator line
     */
    static void printSeparator(char c = '=', int length = 50) {
        for (int i = 0; i < length; i++) {
            Serial.print(c);
        }
        Serial.println();
    }
};

#endif // DEBUG_H
