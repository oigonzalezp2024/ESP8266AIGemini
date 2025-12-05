/**
 * src\IGeminiParser.h
 * @file IGeminiParser.h
 * @brief Puerto de Salida para el manejo de la estructura JSON.
 * Aísla el Núcleo de la librería ArduinoJson o cualquier otra librería de parsing.
 */

#ifndef I_GEMINI_PARSER_H
#define I_GEMINI_PARSER_H

// ********** CAMBIO CRUCIAL AQUÍ **********
#if defined(PLATFORMIO_UNIT_TEST)
    #include "ArduinoMock.h"
#else
    #include <Arduino.h>
#endif
// *****************************************

class IGeminiParser {
public:
    virtual ~IGeminiParser() = default;

    /**
     * @brief Construye el payload JSON para una petición de solo texto.
     * @param prompt El texto de entrada.
     * @return El JSON serializado o String vacío en caso de error.
     */
    virtual String createPayload(const String& prompt) = 0;

    /**
     * @brief Construye el payload JSON para una petición multimodal.
     * @param prompt El texto.
     * @param mimeType El tipo MIME.
     * @param data La data (Base64 para imagen, URI para video).
     * @return El JSON serializado o String vacío en caso de error.
     */
    virtual String createPayloadMultimodal(const String& prompt, const String& mimeType, const String& data) = 0;

    /**
     * @brief Parsea la respuesta JSON de la API.
     * @param jsonResponse La respuesta JSON.
     * @return El texto de respuesta del modelo o un mensaje de error detallado.
     */
    virtual String parseResponse(const String& jsonResponse) = 0;
};

#endif // I_GEMINI_PARSER_H
