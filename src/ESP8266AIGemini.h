
/**
 * @file ESP8266AIGemini.h
 * @brief Definición del Núcleo de la Aplicación (Clase ESP8266AIGemini).
 * Estrictamente desacoplado por Inyección de Múltiples Dependencias (Hexagonal).
 */

#ifndef GEMINI_ESP8266_H
#define GEMINI_ESP8266_H

#if defined(PLATFORMIO_UNIT_TEST)
    #include "ArduinoMock.h"
#else
    #include <Arduino.h> // Solo para el ESP8266 real
#endif
// Solo dependemos de las interfaces (Puertos), no de implementaciones concretas.
#include "IGeminiValidator.h" 
#include "IGeminiApiGateway.h"
#include "IGeminiParser.h"

class ESP8266AIGemini {
public:
    /**
     * @brief Constructor con Triple Inyección de Dependencias.
     * @param apiKey Clave API.
     * @param validator Dependencia del Puerto de Validación.
     * @param gateway Dependencia del Puerto de Comunicación de la API.
     * @param parser Dependencia del Puerto de Serialización/Parsing.
     */
    ESP8266AIGemini(const char* apiKey, 
                  IGeminiValidator* validator,
                  IGeminiApiGateway* gateway,
                  IGeminiParser* parser);

    /**
     * @brief Envía un prompt a la API de Gemini y retorna el texto generado.
     * @param prompt El texto de entrada para el modelo.
     * @return El texto de respuesta del modelo, o un mensaje de error detallado.
     */
    String generateContent(const String& prompt);

    /**
     * @brief Envía un prompt y una imagen codificada en BASE64 a la API de Gemini.
     * @param prompt El texto de entrada para el modelo.
     * @param img_BASE64 Imagen en Base64.
     * @param mimeType Tipo MIME de la imagen (ej: "image/jpeg").
     * @return El texto de respuesta del modelo, o un mensaje de error detallado.
     */
    String imgToProcess(const String& prompt, const String& img_BASE64, const String& mimeType);

    /**
     * @brief Envía un prompt y la URL de un video remoto.
     * @param prompt El texto de entrada para el modelo.
     * @param video_cloud_url URI del video.
     * @return El texto de respuesta del modelo, o un mensaje de error detallado.
     */
    String videoToProcess(const String& prompt, const String& video_cloud_url);

    /**
     * @brief Establece el nombre del modelo a utilizar.
     * @param modelName El nombre del modelo (ej: "gemini-2.5-flash").
     */
    void setModel(const char* modelName);

private:
    const char* _apiKey;
    const char* _model = "gemini-2.5-flash";

    // Dependencias Inyectadas (Los Puertos de Salida)
    IGeminiValidator* _validator;
    IGeminiApiGateway* _gateway; 
    IGeminiParser* _parser; 
    
    // Métodos privados
    String _createPayload(const String& prompt);
    String _createPayloadMultimodal(const String& prompt, const String& mimeType, const String& data);
    String _makeApiRequest(const String& payload);
    String _parseResponse(const String& jsonResponse); 
};
#endif // GEMINI_ESP8266_H
