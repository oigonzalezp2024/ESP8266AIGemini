/**
 * @file GeminiJsonParser.cpp
 * @brief Implementación concreta de la serialización y deserialización JSON usando ArduinoJson.
 */

#include "GeminiJsonParser.h"

// -----------------------------------------------------------------------------
// METODOS DE SERIALIZACION (Creación de Payload)
// -----------------------------------------------------------------------------

/**
 * @brief Construye el payload JSON para una petición de solo texto.
 */
String GeminiJsonParser::createPayload(const String& prompt) {
    const size_t CAPACITY = 512; 
    StaticJsonDocument<CAPACITY> doc;

    // Estructura JSON: { "contents": [ { "role": "user", "parts": [ { "text": "..." } ] } ] }
    JsonArray contents = doc.createNestedArray("contents");
    JsonObject content_0 = contents.createNestedObject();
    content_0["role"] = "user";

    JsonArray parts = content_0.createNestedArray("parts");
    JsonObject part_0 = parts.createNestedObject();
    part_0["text"] = prompt;

    String output;
    if (serializeJson(doc, output) == 0) {
        return ""; 
    }
    return output;
}

/**
 * @brief Construye el payload JSON para una petición multimodal.
 */
String GeminiJsonParser::createPayloadMultimodal(const String& prompt, const String& mimeType, const String& data) {
    const size_t CAPACITY = 2048; 
    StaticJsonDocument<CAPACITY> doc;

    JsonArray contents = doc.createNestedArray("contents");
    JsonObject content_0 = contents.createNestedObject();
    content_0["role"] = "user";

    JsonArray parts = content_0.createNestedArray("parts");

    // 1. Parte del Archivo
    JsonObject part_file = parts.createNestedObject();
    
    if (mimeType.startsWith("image/") || mimeType.startsWith("audio/")) {
        JsonObject inlineData = part_file.createNestedObject("inlineData");
        inlineData["mimeType"] = mimeType;
        inlineData["data"] = data; 
    } else if (mimeType.startsWith("video/")) {
    // } else if (mimeType.rfind("video/", 0) == 0) {
        JsonObject fileData = part_file.createNestedObject("fileData");
        fileData["mimeType"] = mimeType;
        fileData["fileUri"] = data; 
    } else {
        return "";
    }

    // 2. Parte del Prompt (Texto)
    JsonObject part_prompt = parts.createNestedObject();
    part_prompt["text"] = prompt;

    String output;
    if (serializeJson(doc, output) == 0) {
        return ""; 
    }
    return output;
}


// -----------------------------------------------------------------------------
// METODOS DE DESERIALIZACION (Parsing de Respuesta)
// -----------------------------------------------------------------------------

/**
 * @brief Parsea la respuesta JSON de la API para extraer el texto generado o mensajes de error.
 */
String GeminiJsonParser::parseResponse(const String& jsonResponse) {
    
    if (jsonResponse.startsWith("ERROR CONEXION") || jsonResponse.startsWith("ERROR CRITICO")) {
        return jsonResponse;
    }
    
    const size_t CAPACITY = 4096; 
    DynamicJsonDocument doc(CAPACITY);

    DeserializationError error = deserializeJson(doc, jsonResponse);

    if (error) {
        return "ERROR ARDUINOJSON: Fallo al deserializar JSON: " + String(error.c_str());
    }

    // 1. Verificar ERRORES DE LA API
    if (doc.containsKey("error") && doc["error"].containsKey("message")) {
        const char* errorMessage = doc["error"]["message"];
        return "ERROR API GEMINI: " + String(errorMessage);
    }

    // 2. Verificar si hay CANDIDATES válidos
    if (doc.containsKey("candidates") && doc["candidates"].is<JsonArray>() && doc["candidates"].size() > 0) {
        const char* resultText = doc["candidates"][0]["content"]["parts"][0]["text"];
        
        if (resultText) {
            return String(resultText);
        } else {
            const char* finishReason = doc["candidates"][0]["finishReason"];
            if (finishReason) {
                 return "ADVERTENCIA: Respuesta sin texto. Razon de finalizacion: " + String(finishReason);
            }
            return "ERROR: Estructura de contenido no encontrada o vacia.";
        }
    } else {
        return "ERROR: Estructura de respuesta inesperada. No se encontro el campo 'candidates'.";
    }
}
