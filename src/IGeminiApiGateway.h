/**
 * src\IGeminiApiGateway.h
 * @file IGeminiApiGateway.h
 * @brief Interfaz (Puerto) para el acceso a la API de Google Gemini.
 * Define el contrato que cualquier implementación de Gateway (WiFiClient, HTTPClient, Fake)
 * debe seguir para enviar y recibir peticiones.
 */

#ifndef I_GEMINI_API_GATEWAY_H
#define I_GEMINI_API_GATEWAY_H

// ------------------------------------------------------------------
// CRUCIAL: Usamos compilación condicional.
// Si estamos en modo de prueba (PLATFORMIO_UNIT_TEST), usamos el mock.
// Si estamos compilando para el ESP8266, usamos el Arduino real.
// ------------------------------------------------------------------
#if defined(PLATFORMIO_UNIT_TEST)
    // Usamos comillas dobles, el compilador lo buscará en las rutas (-Isrc, -Itest)
    #include "ArduinoMock.h" 
#else
    // Usamos corchetes angulares, lo buscará en las librerías del framework
    #include <Arduino.h>
#endif

class IGeminiApiGateway {
public:
    // **CRUCIAL:** Destructor virtual para asegurar la correcta limpieza al usar polimorfismo.
    virtual ~IGeminiApiGateway() = default;

    /**
     * @brief Envía la solicitud de contenido a la API de Google Gemini.
     * @param model El nombre del modelo a usar (e.g., "gemini-2.5-flash").
     * @param apiKey La clave de autenticación de Google AI Studio.
     * @param payload El JSON que contiene el prompt y la configuración.
     * @return String La respuesta JSON completa del servidor o un mensaje de error.
     */
    virtual String sendRequest(
        const char* model, 
        const char* apiKey, 
        const String& payload
    ) = 0; // Método puramente virtual
};

#endif // I_GEMINI_API_GATEWAY_H