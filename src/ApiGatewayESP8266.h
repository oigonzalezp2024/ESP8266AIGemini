/**
 * @file ApiGatewayESP8266.h
 * @brief Adaptador que implementa el Puerto IGeminiApiGateway usando la infraestructura ESP8266 (HTTPS).
 */

#ifndef API_GATEWAY_ESP8266_H
#define API_GATEWAY_ESP8266_H

#include "IGeminiApiGateway.h"
// Incluye dependencias de infraestructura SOLO en el Adaptador
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

class ApiGatewayESP8266 : public IGeminiApiGateway {
public:
    /**
     * @brief Implementación del Puerto de comunicación usando HTTPS del ESP8266.
     */
    String sendRequest(const char* model, const char* apiKey, const String& payload) override;

private:
    const String API_BASE_URL = "https://generativelanguage.googleapis.com/v1/models/";
};

#endif // API_GATEWAY_ESP8266_H