/**
 * src\IGeminiValidator.h
 * @file IGeminiValidator.h
 * @brief Interfaz abstracta para la capa de validación.
 */

#ifndef I_GEMINI_VALIDATOR_H
#define I_GEMINI_VALIDATOR_H

// ********** CAMBIO CRUCIAL AQUÍ **********
#if defined(PLATFORMIO_UNIT_TEST)
    #include "ArduinoMock.h"
#else
    #include <Arduino.h>
#endif
// *****************************************

/**
 * @brief Interfaz (Clase Abstracta) que define los métodos de validación
 * que deben ser implementados por cualquier validador de Gemini.
 */
class IGeminiValidator {
public:
    // Destructor virtual es crucial para clases base
    virtual ~IGeminiValidator() = default; 

    /**
     * @brief Verifica la conectividad WiFi.
     * @return String vacío si OK, mensaje de error si falla.
     */
    virtual String validateWiFiConnected() = 0; 

    /**
     * @brief Verifica la clave API.
     * @param apiKey La clave API a verificar.
     * @return String vacío si OK, mensaje de error si falla.
     */
    virtual String validateApiKey(const char* apiKey) = 0;
    
    /**
     * @brief Verifica si el tipo MIME es reconocido.
     * @param mimeType El tipo MIME proporcionado.
     * @return String vacío si OK, mensaje de error si falla.
     */
    virtual String validateMimeType(const String& mimeType) = 0;
};

#endif // I_GEMINI_VALIDATOR_H
