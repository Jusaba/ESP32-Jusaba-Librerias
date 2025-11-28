/**
 * @file RTCManager.h
 * @brief Sistema de sincronización temporal NTP 
 * 
 * @details Este header define la clase RTC que gestiona la sincronización temporal
 *          del sistema usando servidores NTP (Network Time Protocol):
 *          
 *          **FUNCIONALIDADES PRINCIPALES:**
 *          - Sincronización NTP con múltiples servidores para redundancia
 *          - Configuración automática de zona horaria y horario de verano
 *          - Validación de fechas recibidas para evitar datos corruptos
 *          - Timeout configurable para evitar bloqueos en sincronización
 *          - Sistema de fallback entre servidores si uno falla
 *          - Formateo y conversión de fechas/horas a strings legibles
 *          - Estado de sincronización persistente para consulta
 *          
 *          **ARQUITECTURA DEL SISTEMA:**
 *          - Clase estática RTC: No requiere instanciación
 *          - Integración con configTime() del ESP32 para NTP nativo
 *          - Soporte para hasta 3 servidores NTP simultáneos
 *          - Validación automática de fechas realistas (2020-2050)
 *          - Debug condicional con RTCMANAGER_DEBUG para troubleshooting
 *          - Estado global ntpSyncOk para verificación de sincronización
 * 
 * @note **SERVIDORES NTP CONFIGURADOS:**
 *       - Servidor 1: Definido en NTP_SERVER1 (típicamente pool.ntp.org)
 *       - Servidor 2: Definido en NTP_SERVER2 (servidor de backup)
 *       - Servidor 3: Definido en NTP_SERVER3 (servidor de backup)
 *       - Rotación automática entre servidores si hay fallos
 * 
 * @note **ZONA HORARIA:**
 *       - GMT_OFFSET_SEC: Offset en segundos respecto GMT
 *       - DAYLIGHT_OFFSET_SEC: Ajuste horario de verano en segundos
 *       - Configuración automática según ubicación del sistema
 * 
 * @warning **DEPENDENCIAS CRÍTICAS:**
 *          - WiFi.h: Conexión a Internet requerida para NTP
 *          - time.h: Funciones de tiempo del sistema ESP32
 * 
 * @warning **REQUISITOS DE RED:**
 *          - Conexión Wi-Fi estable y activa
 *          - Acceso UDP al puerto 123 (protocolo NTP)
 *          - Resolución DNS funcional para nombres de servidores
 *          - Latencia de red razonable (< 1000ms recomendado)
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-28
 * @version 1.0.0
 * 
 * @since v1.0.0 - Primera versión de librería independiente
 * 
 * @see RTCManager.cpp - Implementación de todas las funcionalidades
 */
#ifndef RTCMANAGER_H
#define RTCMANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

// Configuración de debug (comentar para desactivar)
// #define RTCMANAGER_DEBUG

#ifdef RTCMANAGER_DEBUG
    #define DBG_RTC(x) Serial.println("[RTC] " + String(x))
    #define DBG_RTC_PRINT(x) Serial.print("[RTC] " + String(x))
#else
    #define DBG_RTC(x)
    #define DBG_RTC_PRINT(x)
#endif

// Configuración por defecto para servidores NTP
#ifndef NTP_SERVER1
    #define NTP_SERVER1 "pool.ntp.org"
#endif

#ifndef NTP_SERVER2
    #define NTP_SERVER2 "es.pool.ntp.org"
#endif

#ifndef NTP_SERVER3
    #define NTP_SERVER3 "time.google.com"
#endif

#ifndef GMT_OFFSET_SEC
    #define GMT_OFFSET_SEC 3600  // GMT+1 (España)
#endif

#ifndef DAYLIGHT_OFFSET_SEC
    #define DAYLIGHT_OFFSET_SEC 3600  // Horario de verano
#endif

/**
 * @brief Clase estática para gestión de sincronización temporal NTP
 * 
 * @details Clase que encapsula todas las funcionalidades relacionadas con
 *          la sincronización de tiempo usando servidores NTP. Diseñada como
 *          clase estática para acceso global sin necesidad de instanciación.
 *          
 *          **CARACTERÍSTICAS DE DISEÑO:**
 *          - Métodos estáticos: No requiere instanciación
 *          - Estado global: ntpSyncOk accesible desde cualquier parte
 *          - Thread-safe: Operaciones atómicas en ESP32
 *          - Fallback automático: Rotación entre servidores si hay fallos
 *          - Validación inteligente: Fechas realistas y rangos válidos
 * 
 * @note **PATRÓN DE USO TÍPICO:**
 *       1. RTC::beginConMultiplesServidores() - Sincronización inicial con varios servidores
 *       2. RTC::isNtpSync() - Verificar estado de sincronización
 *       3. RTC::getTimeStr() - Obtener fecha/hora formateada
 * 
 * @warning **LIMITACIONES:**
 *          - Requiere conexión Wi-Fi activa para funcionar
 *          - No mantiene tiempo sin conexión a largo plazo
 *          - ESP32 puede perder tiempo en deep sleep
 * 
 * @since v1.0.0
 * @author Julian Salas Bartolomé
 */    
class RTC {
public:
    static void begin(const char* ntpServer = NTP_SERVER1, 
                      long gmtOffsetSec = GMT_OFFSET_SEC, 
                      int daylightOffsetSec = DAYLIGHT_OFFSET_SEC, 
                      unsigned long timeout_ms = 10000);
    
    static bool beginConMultiplesServidores(unsigned long timeout_ms = 15000);
    
    static bool isNtpSync();
    
    static String getTimeStr();

private:
    static String timeToString(const struct tm& timeinfo);
    
    static bool ValidaFecha(const struct tm& timeinfo);

    static bool ntpSyncOk;
};

#endif // RTCMANAGER_H
