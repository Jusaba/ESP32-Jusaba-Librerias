/**
 * @file Configuracion.h
 * @brief Configuration template for AlarmScheduler library
 * 
 * This is an EXAMPLE template. Copy this file to your project and customize it.
 * See templates/README.md for detailed instructions.
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 */

#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Arduino.h>

// ============================================================================
// ALARMSCHEDULER CONFIGURATION
// ============================================================================
namespace Config {
    namespace Alarms {
        // JSON persistence file path
        const char JSON_FILE[] = "/customizable_alarms.json";
        
        // Load default alarms if JSON file doesn't exist
        const bool LOAD_DEFAULTS = true;
        
        // Auto-save on modifications
        const bool AUTO_SAVE = true;
        
        // Maximum customizable alarms (system uses AlarmScheduler::MAX_ALARMS = 16)
        const uint8_t MAX_CUSTOMIZABLE = 10;
    }

    namespace Storage {
        const bool FORMAT_ON_FAIL = true;
        const size_t MAX_OPEN_FILES = 5;
    }
}

#endif // CONFIGURACION_H
