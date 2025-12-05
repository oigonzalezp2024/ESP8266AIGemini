/**
 * @file ApiGatewayESP8266.cpp
 * @brief Implementación concreta de la conexión HTTP/S usando ESP8266HTTPClient.
 * Esta lógica de infraestructura está aislada del Núcleo de la aplicación.
 */

#include "ApiGatewayESP8266.h"
#include <memory> 

/**
 * @brief Realiza la petición HTTP/S POST real a la API de Gemini.
 * @param model El modelo.
 * @param apiKey La clave.
 * @param payload El JSON.
 * @return El JSON de respuesta o un error de conexión/HTTP.
 */
String ApiGatewayESP8266::sendRequest(const char* model, const char* apiKey, const String& payload) {
    // Uso de BearSSL para cliente seguro (HTTPS)
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    
    // setInsecure() se usa para simplificar el manejo de certificados en ESP8266.
    client->setInsecure(); 
    
    HTTPClient http;
    // Construcción de la URL completa
    String url = API_BASE_URL + String(model) + ":generateContent?key=" + String(apiKey);

    if (!http.begin(*client, url)) {
        return "ERROR CRITICO: Fallo en http.begin() (no se pudo configurar la URL/cliente).";
    }

    // Configuración del encabezado Content-Type
    http.addHeader("Content-Type", "application/json");

    // Envío de la Petición POST
    int httpCode = http.POST(payload);
    String response = "";

    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) { 
            response = http.getString();
        } else {
            // Manejo de errores HTTP con cuerpo de respuesta de la API
            response = http.getString(); 
            if (response.length() == 0) {
                 response = "ERROR API: HTTP Fallido, codigo: " + String(httpCode) + " - " + http.errorToString(httpCode);
            } else {
                 response = "ERROR API DETECTADO (" + String(httpCode) + "): " + response;
            }
        }
    } else {
        // Errores de conexión de bajo nivel (TCP/SSL)
        response = "ERROR CONEXION: Fallida (codigo BearSSL/HTTPClient: " + String(httpCode) + ") - " + http.errorToString(httpCode);
    }

    http.end(); // Liberar recursos
    return response;
}
