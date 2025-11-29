/**
 * @file Configuracion.h
 * @brief Plantilla base de configuración para proyectos ESP32
 * 
 * @details Este es un archivo GENÉRICO con la configuración mínima indispensable
 *          para cualquier proyecto ESP32. Cópialo a tu proyecto y personalízalo.
 *          
 *          Las librerías específicas pueden tener sus propios archivos
 *          Configuracion.h en sus carpetas templates/ que puedes usar como
 *          referencia para añadir configuraciones adicionales.
 * 
 * **USO:**
 * 1. Copia este archivo a la carpeta de tu proyecto
 * 2. Personaliza los valores según tus necesidades
 * 3. Si usas librerías específicas (ej: RTCManager), copia también su
 *    Configuracion.h desde libraries/NombreLibreria/templates/ y fusiona
 *    los namespaces que necesites
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-29
 * @version 1.0.0
 */

#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Arduino.h>

namespace Config {

    // ==================== INFORMACIÓN DEL PROYECTO ====================
    /**
     * @brief Información general del proyecto
     */
    namespace Project {
        inline String NAME = "Mi Proyecto ESP32";      // Nombre del proyecto
        inline String VERSION = "1.0.0";               // Versión
        inline String AUTHOR = "Tu Nombre";            // Autor
    }

    // ==================== COMUNICACIÓN SERIAL ====================
    /**
     * @brief Configuración del puerto serial
     */
    namespace Serial {
        constexpr unsigned long BAUD_RATE = 115200;    // Velocidad estándar
    }

} // namespace Config

/*
 * INSTRUCCIONES:
 * 
 * 1. CONFIGURACIÓN BÁSICA:
 *    - Edita Config::Project::NAME con el nombre de tu proyecto
 *    - Edita Config::Project::VERSION según tu versión
 *    - Edita Config::Project::AUTHOR con tu nombre
 * 
 * 2. AÑADIR CONFIGURACIÓN DE LIBRERÍAS:
 *    Si usas RTCManager, TelegramService, u otras librerías:
 *    
 *    a) Ve a: libraries/NombreLibreria/templates/Configuracion.h
 *    b) Copia los namespaces que necesites (WiFi, Time, etc.)
 *    c) Pégalos aquí dentro de namespace Config { }
 *    
 *    Ejemplo con RTCManager:
 *    ```cpp
 *    namespace Config {
 *        // ... namespaces básicos ...
 *        
 *        // Copiado de RTCManager/templates/Configuracion.h
 *        namespace WiFi {
 *            inline String SSID = "MiWiFi";
 *            inline String PASSWORD = "mipassword";
 *        }
 *        
 *        namespace Time {
 *            inline String NTP_SERVER1 = "pool.ntp.org";
 *            constexpr long GMT_OFFSET_SEC = 3600;
 *            constexpr int DAYLIGHT_OFFSET_SEC = 3600;
 *        }
 *    }
 *    ```
 * 
 * 3. AÑADIR TU PROPIA CONFIGURACIÓN:
 *    Crea namespaces adicionales según necesites:
 *    
 *    ```cpp
 *    namespace Config {
 *        // ... namespaces existentes ...
 *        
 *        namespace MiModulo {
 *            constexpr int PIN_LED = 2;
 *            constexpr int PIN_SENSOR = 34;
 *            inline String NOMBRE = "Sensor Principal";
 *        }
 *    }
 *    ```
 * 
 * 4. USAR EN TU CÓDIGO:
 *    ```cpp
 *    #include "Configuracion.h"
 *    
 *    void setup() {
 *        Serial.begin(Config::Serial::BAUD_RATE);
 *        Serial.println(Config::Project::NAME);
 *    }
 *    ```
 */

#endif // CONFIGURACION_H
