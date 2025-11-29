/**
 * @file BasicAlarms.ino
 * @brief Basic example demonstrating AlarmScheduler library usage
 * 
 * This example shows:
 * - Basic alarm setup with external callbacks
 * - Fixed time alarms
 * - Wildcard alarms (every hour, every minute)
 * - Interval-based alarms
 * - Day mask usage
 * - SPIFFS integration
 * - Debug output
 * 
 * @note Requires:
 *       - ESP32 board
 *       - Time configured (NTP or RTC)
 *       - ArduinoJson library
 *       - SPIFFS initialized
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 */

#include <WiFi.h>
#include <time.h>
#include <SPIFFS.h>
#include <AlarmScheduler.h>

// WiFi credentials - CHANGE THESE!
const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASSWORD = "YourWiFiPassword";

// NTP configuration
const char* NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 3600;      // GMT+1 (adjust for your timezone)
const int DAYLIGHT_OFFSET_SEC = 3600;  // DST +1 hour

// Create scheduler instance
AlarmScheduler scheduler;

// ============================================================================
// ALARM CALLBACK FUNCTIONS
// ============================================================================

/**
 * @brief Morning bell callback
 * @param duration Duration in seconds
 */
void morningBell(uint16_t duration) {
    Serial.println("========================================");
    Serial.printf("⏰ MORNING BELL! Duration: %u seconds\n", duration);
    Serial.println("========================================");
    // Your code here: activate bell, send notification, etc.
}

/**
 * @brief Evening bell callback
 * @param duration Duration in seconds
 */
void eveningBell(uint16_t duration) {
    Serial.println("========================================");
    Serial.printf("🔔 EVENING BELL! Duration: %u seconds\n", duration);
    Serial.println("========================================");
    // Your code here: activate bell, send notification, etc.
}

/**
 * @brief Hourly chime callback (no parameters)
 */
void hourlyChime() {
    Serial.println("⏱️  Hourly chime!");
    // Your code here: brief sound, LED flash, etc.
}

/**
 * @brief Status report callback
 * @param reportType Type of report to generate
 */
void statusReport(uint16_t reportType) {
    Serial.println("\n📊 === STATUS REPORT ===");
    Serial.printf("Report type: %u\n", reportType);
    Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
    Serial.printf("Uptime: %lu seconds\n", millis() / 1000);
    
    // Print alarm statistics
    Serial.printf("Total alarms: %u/%u\n", scheduler.count(), AlarmScheduler::MAX_ALARMS);
    
    Serial.println("========================\n");
}

/**
 * @brief Interval check callback (runs every 15 minutes)
 * @param checkType Type of check to perform
 */
void intervalCheck(uint16_t checkType) {
    Serial.printf("🔄 Interval check (type %u) - %02u:%02u\n", 
                  checkType, scheduler.t.tm_hour, scheduler.t.tm_min);
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n");
    Serial.println("========================================");
    Serial.println("  AlarmScheduler - Basic Example");
    Serial.println("========================================\n");
    
    // Initialize SPIFFS
    Serial.print("Initializing SPIFFS... ");
    if (!SPIFFS.begin(true)) {
        Serial.println("FAILED!");
        Serial.println("Cannot continue without SPIFFS");
        while (1) delay(1000);
    }
    Serial.println("OK");
    
    // Connect to WiFi
    Serial.printf("Connecting to WiFi '%s'... ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" CONNECTED");
        Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println(" FAILED");
        Serial.println("⚠️  Running without WiFi - time may not be accurate");
    }
    
    // Configure time
    Serial.print("Configuring time via NTP... ");
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
    
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        Serial.println("OK");
        Serial.printf("Current time: %04d-%02d-%02d %02d:%02d:%02d\n",
                     timeinfo.tm_year + 1900,
                     timeinfo.tm_mon + 1,
                     timeinfo.tm_mday,
                     timeinfo.tm_hour,
                     timeinfo.tm_min,
                     timeinfo.tm_sec);
    } else {
        Serial.println("FAILED");
        Serial.println("⚠️  Time not synchronized - alarms may not work correctly");
    }
    
    Serial.println();
    
    // Initialize scheduler
    Serial.println("Initializing AlarmScheduler...");
    scheduler.begin(false);  // false = don't create default alarms
    Serial.println("Scheduler initialized\n");
    
    // ========================================================================
    // ADD EXAMPLE ALARMS
    // ========================================================================
    
    Serial.println("Adding example alarms:\n");
    
    // 1. Morning bell - Every day at 08:00
    Serial.println("1. Morning bell - Every day at 08:00");
    scheduler.addExternal(
        DOW_ALL,        // Every day
        8,              // 08:00
        0,              // 00 minutes
        0,              // No interval
        morningBell,    // Callback
        10,             // Parameter: 10 seconds duration
        true            // Enabled
    );
    
    // 2. Evening bell - Every day at 20:00
    Serial.println("2. Evening bell - Every day at 20:00");
    scheduler.addExternal(
        DOW_ALL,        // Every day
        20,             // 20:00
        0,              // 00 minutes
        0,              // No interval
        eveningBell,    // Callback
        5,              // Parameter: 5 seconds duration
        true            // Enabled
    );
    
    // 3. Hourly chime - Every hour at XX:00
    Serial.println("3. Hourly chime - Every hour at XX:00");
    scheduler.addExternal0(
        DOW_ALL,           // Every day
        ALARM_WILDCARD,    // Any hour (*)
        0,                 // 00 minutes
        0,                 // No interval
        hourlyChime,       // Callback (no parameters)
        true               // Enabled
    );
    
    // 4. Status report - Weekdays at 12:00
    Serial.println("4. Status report - Monday to Friday at 12:00");
    scheduler.addExternal(
        DOW_MONDAY | DOW_TUESDAY | DOW_WEDNESDAY | DOW_THURSDAY | DOW_FRIDAY,
        12,             // 12:00
        0,              // 00 minutes
        0,              // No interval
        statusReport,   // Callback
        1,              // Parameter: report type 1
        true            // Enabled
    );
    
    // 5. Interval check - Every 15 minutes, anchored at XX:00
    Serial.println("5. Interval check - Every 15 minutes");
    scheduler.addExternal(
        DOW_ALL,           // Every day
        ALARM_WILDCARD,    // Any hour
        0,                 // Anchor at XX:00
        15,                // Repeat every 15 minutes
        intervalCheck,     // Callback
        42,                // Parameter: check type 42
        true               // Enabled
    );
    
    // 6. Weekend morning - Saturday and Sunday at 09:30
    Serial.println("6. Weekend morning - Saturday and Sunday at 09:30");
    scheduler.addExternal(
        DOW_SATURDAY | DOW_SUNDAY,
        9,              // 09:00
        30,             // 30 minutes
        0,              // No interval
        morningBell,    // Callback (reuse)
        15,             // Parameter: 15 seconds duration
        true            // Enabled
    );
    
    Serial.println();
    
    // Print all alarms
    scheduler.printAllAlarms();
    
    Serial.println("========================================");
    Serial.println("Setup complete! Monitoring alarms...");
    Serial.println("========================================\n");
}

// ============================================================================
// LOOP
// ============================================================================

void loop() {
    // Check and execute alarms
    scheduler.check();
    
    // Print current time every 30 seconds (for demonstration)
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 30000) {
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            Serial.printf("⏰ Current time: %02d:%02d:%02d | Alarms: %u active\n",
                         timeinfo.tm_hour,
                         timeinfo.tm_min,
                         timeinfo.tm_sec,
                         scheduler.count());
        }
        lastPrint = millis();
    }
    
    delay(1000);  // Check every second
}
