
#include "ESP8266AIGemini.h"
#include "GeminiValidator.h" 
#include "ApiGatewayESP8266.h"
#include "GeminiJsonParser.h"
#include <ESP8266WiFi.h>

const char* WIFI_SSID      = "WIFI_SSID"; // WIFI
const char* WIFI_PASS      = "WIFI_PASS"; // PASS
const char* GEMINI_API_KEY = "GEMINI_API_KEY";// -> https://ai.google.dev/gemini-api/docs/api-key?

GeminiValidator realValidator; 
ApiGatewayESP8266 apiGateway;
GeminiJsonParser jsonParser;

ESP8266AIGemini gemini(GEMINI_API_KEY, 
                     &realValidator, 
                     &apiGateway, 
                     &jsonParser);

String promptUsuario = "";

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\n--- Iniciando ESP8266 con Gemini (Arquitectura Hexagonal) ---");

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Conectando a WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n¡Conectado!");
    Serial.print("Direccion IP: ");
    Serial.println(WiFi.localIP());

    Serial.println("\n=======================================");
    Serial.println("¡Listo! Introduce tu pregunta y presiona Enter:");
    Serial.println("=======================================");
    Serial.print("Tu pregunta >> ");
}

void loop() {
    while (Serial.available()) {
        char incomingChar = Serial.read();

        if (incomingChar == '\n' || incomingChar == '\r') {
            
            if (promptUsuario.length() > 0) {
                promptUsuario.trim(); 
                
                Serial.println();
                Serial.println("\n-> Enviando prompt a Gemini...");
                Serial.print("Prompt final: ");
                Serial.println(promptUsuario);

                String response = gemini.generateContent(promptUsuario);

                Serial.println("<- Respuesta de Gemini:");
                Serial.println("---------------------------------------");
                Serial.println(response);
                Serial.println("---------------------------------------");
                
                promptUsuario = "";
                Serial.println("\n=======================================");
                Serial.println("¡Pregunta respondida! Introduce una nueva pregunta:");
                Serial.println("=======================================");
                Serial.print("Tu pregunta >> ");
            }
        } else {
            promptUsuario += incomingChar;
        }
    }

    delay(5); 
}
