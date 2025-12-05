/**
 * @file ESP8266AIGemini.cpp
 * @brief Implementación estricta del Núcleo Hexagonal.
 * Se coordina la interacción entre los Puertos de Validación, Comunicación y Parsing.
 */

#include "ESP8266AIGemini.h"
#include "IGeminiValidator.h" 
#include "IGeminiApiGateway.h"
#include "IGeminiParser.h" 

// Constructor con DI
ESP8266AIGemini::ESP8266AIGemini(const char* apiKey, 
                             IGeminiValidator* validator,
                             IGeminiApiGateway* gateway,
                             IGeminiParser* parser)
    : _apiKey(apiKey), _validator(validator), _gateway(gateway), _parser(parser) {}

void ESP8266AIGemini::setModel(const char* modelName) {
    _model = modelName;
}

// Implementación del método principal para solo texto
String ESP8266AIGemini::generateContent(const String& prompt) {
    // 1. Llama al Puerto de Validación
    String validationError = _validator->validateWiFiConnected();
    if (validationError.length() > 0) return validationError;
    validationError = _validator->validateApiKey(_apiKey);
    if (validationError.length() > 0) return validationError;

    // 2. Llama al Puerto de Parsing para crear el payload
    String payload = _parser->createPayload(prompt);
    if (payload.length() == 0) return "ERROR: Fallo al crear el JSON.";

    // 3. Llama al Puerto de Gateway para enviar la solicitud
    String jsonResponse = _gateway->sendRequest(_model, _apiKey, payload);
    
    // 4. Llama al Puerto de Parsing para analizar la respuesta
    return _parser->parseResponse(jsonResponse);
}

// Implementación para multimodal (Imagen)
String ESP8266AIGemini::imgToProcess(const String& prompt, const String& img_BASE64, const String& mimeType) {
    // 1. Validación
    String validationError = _validator->validateWiFiConnected();
    if (validationError.length() > 0) return validationError;
    validationError = _validator->validateApiKey(_apiKey);
    if (validationError.length() > 0) return validationError;
    validationError = _validator->validateMimeType(mimeType);
    if (validationError.length() > 0) return validationError;

    // 2. Creación de Payload Multimodal
    String payload = _parser->createPayloadMultimodal(prompt, mimeType, img_BASE64);
    if (payload.length() == 0) return "ERROR: Fallo al crear el JSON multimodal.";

    // 3. Envío
    String jsonResponse = _gateway->sendRequest(_model, _apiKey, payload);
    
    // 4. Parseo
    return _parser->parseResponse(jsonResponse);
}

// Implementación para multimodal (Video URI)
/**
 * @brief Envía un prompt y la URL de un video remoto.
 */
String ESP8266AIGemini::videoToProcess(const String& prompt, const String& video_cloud_url) {
    // 1. Validación (Puerto de Validación)
    String validationError = _validator->validateWiFiConnected();
    if (validationError.length() > 0) return validationError;
    validationError = _validator->validateApiKey(_apiKey);
    if (validationError.length() > 0) return validationError;
    
    // El mimeType de video se infiere como "video/mp4" o debe ser conocido.
    // Usaremos una validación base para demostrar el uso del puerto.
    // NOTA: Para videos alojados en Google Cloud Storage, el tipo MIME es crucial.
    const String videoMimeType = "video/mp4"; 
    validationError = _validator->validateMimeType(videoMimeType);
    if (validationError.length() > 0) return validationError;


    // 2. Creación de Payload Multimodal (Puerto de Parsing)
    // Se utiliza video_cloud_url como el 'data' y el tipo MIME de video.
    String payload = _parser->createPayloadMultimodal(prompt, videoMimeType, video_cloud_url);
    if (payload.length() == 0) return "ERROR: Fallo al crear el JSON multimodal para video.";

    // 3. Envío (Puerto de Gateway)
    String jsonResponse = _gateway->sendRequest(_model, _apiKey, payload);
    
    // 4. Parseo (Puerto de Parsing)
    return _parser->parseResponse(jsonResponse);
}
