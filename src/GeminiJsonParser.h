/**
 * @file GeminiJsonParser.h
 * @brief Adaptador que implementa el Puerto IGeminiParser usando la librería ArduinoJson.
 */

#ifndef GEMINI_JSON_PARSER_H
#define GEMINI_JSON_PARSER_H

#include "IGeminiParser.h"
#include <ArduinoJson.h> // Incluye dependencia de la librería SOLO en el Adaptador

class GeminiJsonParser : public IGeminiParser {
public:
    String createPayload(const String& prompt) override;
    String createPayloadMultimodal(const String& prompt, const String& mimeType, const String& data) override;
    String parseResponse(const String& jsonResponse) override;
};

#endif // GEMINI_JSON_PARSER_H
