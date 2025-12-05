## ESP8266AIGemini

**ESP8266: Cliente de la API de Gemini (Google AI) implementado con Arquitectura Hexagonal y Triple Inyección de Dependencias (DI).**

-----

### 💡 Concepto Clave: Arquitectura Hexagonal

La librería **`ESP8266AIGemini`** implementa el *Núcleo Hexagonal* de la aplicación. Esto la hace **independiente** de detalles externos como la conectividad WiFi, la librería JSON o el método HTTP de comunicación. La clase principal coordina **Puertos de Salida** inyectados (`IGeminiValidator`, `IGeminiApiGateway`, `IGeminiParser`) para lograr:

1.  **Validación** (p.ej., WiFi).
2.  **Preparación del Payload** (JSON).
3.  **Comunicación** (HTTP/S).
4.  **Análisis de la Respuesta** (Parsing JSON).

-----

<a name="release"></a>
[](https://www.google.com/search?q=https://github.com/oigonzalezp2024/ESP8266AIGemini)
[](https://www.google.com/search?q=https://github.com/oigonzalezp2024/ESP8266AIGemini/blob/main/LICENSE)
[](https://en.wikipedia.org/wiki/Hexagonal_architecture_\(software\))

[](https://www.espressif.com/en/products/socs/esp8266)
[](https://ai.google.dev/gemini-api/docs)

-----

## 📖 Contents

  - [Overview](https://www.google.com/search?q=%23overview-gemini)
  - [How It Works (Hexagonal Core)](https://www.google.com/search?q=%23how-it-works-gemini)
  - [Quick Start](https://www.google.com/search?q=%23quick-start-gemini)
      - [Installation](https://www.google.com/search?q=%23installation-gemini)
      - [Basic Usage](https://www.google.com/search?q=%23basic-usage-gemini)
  - [API Documentation](https://www.google.com/search?q=%23api-documentation-gemini)
  - [Dependencies (Ports)](https://www.google.com/search?q=%23dependencies)
  - [Contributions](https://www.google.com/search?q=%23contributions)

-----

## Overview 
https://www.google.com/search?q=%23overview-gemini

Esta librería proporciona un *Core de Aplicación* para interactuar con la **API de Google Gemini** desde un módulo **ESP8266**. Su característica principal es el **fuerte desacoplamiento** a través de la Arquitectura Hexagonal, lo que permite intercambiar fácilmente las implementaciones de:

  * **Validación de Entorno** (p.ej., verificar WiFi).
  * **Comunicación con la API** (p.ej., usar `WiFiClientSecure` o mockear para pruebas).
  * **Serialización/Deserialización** (p.ej., usar `ArduinoJson` o cualquier otro *parser*).

Esto garantiza un código **limpio, modular y altamente testeable**.

## How It Works (Hexagonal Core) 
https://www.google.com/search?q=%23how-it-works-gemini

La clase `ESP8266AIGemini` actúa como el **adaptador primario** de la aplicación, ejecutando la lógica de negocio central (flujo de solicitud de la API). No sabe **cómo** se validan las cosas, **cómo** se envía la solicitud HTTP, ni **cómo** se analiza el JSON; simplemente **llama a sus interfaces inyectadas (Puertos de Salida)**.

El flujo para una solicitud `generateContent` es:

1.  **`_validator->validate(...)`**: Verifica precondiciones.
2.  **`_parser->createPayload(...)`**: Construye el JSON de la solicitud.
3.  **`_gateway->sendRequest(...)`**: Envía la solicitud HTTP/S.
4.  **`_parser->parseResponse(...)`**: Extrae la respuesta de texto.

-----

## 🚀 Quick Start 
https://www.google.com/search?q=%23quick-start-gemini

### Installation 
https://www.google.com/search?q=%23installation-gemini

Debido a su naturaleza basada en inyección de dependencias, esta librería requiere que el usuario proporcione las implementaciones de sus interfaces (Puertos).

1.  **Descargar/Clonar** el repositorio de la librería `ESP8266AIGemini`.
2.  **Implementar las Interfaces (Puertos)**: Necesitarás implementar las clases que heredan de `IGeminiValidator`, `IGeminiApiGateway`, y `IGeminiParser` (o usar las implementaciones de ejemplo proporcionadas, como `ApiGatewayESP8266`).

### Basic Usage 
https://www.google.com/search?q=%23basic-usage-gemini


El uso se centra en la **Triple Inyección de Dependencias** en el constructor.

```cpp
#include "ESP8266AIGemini.h"
#include "GeminiValidator.h" 
#include "ApiGatewayESP8266.h"
#include "GeminiJsonParser.h"
// ... (Configuración de WiFi, etc.)

// 1. Instanciar las Implementaciones de los Puertos (Adaptadores Secundarios)
// Estos objetos contienen la lógica concreta (WiFi, JSON, HTTP).
GeminiValidator realValidator; 
ApiGatewayESP8266 apiGateway;
GeminiJsonParser jsonParser;

// 2. Inyectar las Dependencias al Core (Clase principal)
ESP8266AIGemini gemini(
    GEMINI_API_KEY, 
    &realValidator,  // Puerto de Validación
    &apiGateway,     // Puerto de Comunicación (Gateway)
    &jsonParser      // Puerto de Parsing
);

void setup() {
    // ... Conexión WiFi
    gemini.setModel("gemini-2.5-flash"); // Opcional, modelo por defecto
}

void loop() {
    // Ejemplo de uso
    String prompt = "Dime un dato curioso sobre Colombia.";
    String response = gemini.generateContent(prompt);
    
    Serial.println("Respuesta de Gemini:");
    Serial.println(response);
    
    delay(5000); 
}
```

-----

## API Documentation 
https://www.google.com/search?q=%23api-documentation-gemini

La clase principal es **`ESP8266AIGemini`**.

### Constructor

| Declaración | `ESP8266AIGemini(const char* apiKey, IGeminiValidator* validator, IGeminiApiGateway* gateway, IGeminiParser* parser)` |
| :--- | :--- |
| **Descripción** | Inicializa el Núcleo inyectando las dependencias necesarias. **Requiere** las implementaciones de los Puertos. |
| **`apiKey`** | Clave de la API de Google Gemini. |
| **`validator`** | Puntero a la implementación de `IGeminiValidator`. |
| **`gateway`** | Puntero a la implementación de `IGeminiApiGateway`. |
| **`parser`** | Puntero a la implementación de `IGeminiParser`. |

### Métodos Principales

| Método | Declaración | Descripción |
| :--- | :--- | :--- |
| **`generateContent()`** | `String generateContent(const String& prompt)` | Envía una solicitud de **solo texto** al modelo Gemini. |
| **`imgToProcess()`** | `String imgToProcess(const String& prompt, const String& img_BASE64, const String& mimeType)` | Envía una solicitud **multimodal** (texto + **imagen en Base64**). |
| **`videoToProcess()`** | `String videoToProcess(const String& prompt, const String& video_cloud_url)` | Envía una solicitud **multimodal** (texto + **URI de video**). (Asume `video/mp4` y URL válida). |
| **`setModel()`** | `void setModel(const char* modelName)` | Establece el modelo de Gemini a utilizar (p.ej., `"gemini-2.5-flash"`). Por defecto: `"gemini-2.5-flash"`. |

-----

## Dependencies (Ports) 
https://www.google.com/search?q=%23dependencies

El núcleo depende de las siguientes **interfaces (Puertos)**. Para usar la librería, se deben crear y suministrar sus implementaciones concretas:

### 1\. IGeminiValidator

Se encarga de las **validaciones previas** al envío de la solicitud.

  * `validateWiFiConnected()`: Verifica el estado de la conexión WiFi.
  * `validateApiKey()`: Verifica la clave API.
  * `validateMimeType()`: Verifica formatos de imagen/video.

### 2\. IGeminiApiGateway

Se encarga de la **comunicación** con el *endpoint* de la API de Gemini.

  * `sendRequest(model, apiKey, payload)`: Maneja la conexión HTTP/S y el envío del JSON.

### 3\. IGeminiParser

Se encarga de la **serialización y deserialización**.

  * `createPayload(...)`: Construye el JSON (texto o multimodal).
  * `parseResponse(...)`: Extrae el texto de la respuesta JSON de Gemini.

-----

## Contributions

Este proyecto fue desarrollado por **Oscar Gonzalez**. ¡Se agradecen contribuciones y sugerencias\!

-----
