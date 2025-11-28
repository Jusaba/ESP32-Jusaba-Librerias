/**
 * BasicNTPSync.ino
 * 
 * Ejemplo básico de sincronización NTP con RTCManager
 * 
 * Este ejemplo demuestra:
 * - Conexión WiFi básica
 * - Sincronización NTP con múltiples servidores
 * - Verificación de estado de sincronización
 * - Lectura y formateo de hora actual
 * 
 * @author Julian Salas Bartolomé
 * @date 2025-11-28
 */

#include <WiFi.h>
#include <RTCManager.h>

// Configuración WiFi
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n=================================");
    Serial.println("RTCManager - Ejemplo Básico NTP");
    Serial.println("=================================\n");

    // Conectar WiFi
    Serial.print("Conectando a WiFi");
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\n✅ WiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
    // Sincronizar con servidores NTP
    Serial.println("\n📡 Iniciando sincronización NTP...");
    
    if (RTC::beginConMultiplesServidores(30000)) {
        Serial.println("✅ Sincronización NTP exitosa!");
        Serial.print("🕐 Hora actual: ");
        Serial.println(RTC::getTimeStr());
    } else {
        Serial.println("❌ Error en sincronización NTP");
        Serial.println("⚠️ Verifica conexión a Internet y servidores NTP");
    }
    
    Serial.println("\n=================================\n");
}

void loop() {
    // Verificar estado de sincronización
    if (RTC::isNtpSync()) {
        Serial.print("🕐 ");
        Serial.println(RTC::getTimeStr());
    } else {
        Serial.println("⚠️ Sin sincronización NTP");
    }
    
    delay(5000);  // Mostrar hora cada 5 segundos
}
