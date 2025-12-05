/**
 * @file GeminiValidator.h
 * @brief Implementación concreta de la interfaz IGeminiValidator.
 */

#ifndef GEMINI_VALIDATOR_H
#define GEMINI_VALIDATOR_H

#include "IGeminiValidator.h"
#include <ESP8266WiFi.h>

/**
 * @brief Implementación real de las validaciones de negocio.
 */
class GeminiValidator : public IGeminiValidator {
public:
    // Implementación de los métodos de la interfaz
    String validateWiFiConnected() override {
        if (WiFi.status() != WL_CONNECTED) {
            return "ERROR: La validación falló. WiFi no está conectado.";
        }
        return "";
    }

    String validateApiKey(const char* apiKey) override {
        if (apiKey == nullptr || *apiKey == '\0') {
            return "ERROR: La validación falló. La clave API no puede ser nula o vacía.";
        }
        return "";
    }
    
    String validateMimeType(const String& mimeType) override {
        if (!mimeType.startsWith("image/") && !mimeType.startsWith("video/") && !mimeType.startsWith("audio/")) {
            return "ERROR: Tipo MIME (" + mimeType + ") no soportado o inválido.";
        }
        return "";
    }
};

#endif // GEMINI_VALIDATOR_H
