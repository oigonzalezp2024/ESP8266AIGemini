## 🇺🇸 ESP8266AIGemini: Hexagonal Gemini API Client

This is a brief README for the `ESP8266AIGemini` library.

### Overview

This library provides a highly **decoupled** core for interacting with the **Google Gemini API** from an **ESP8266**. It uses **Hexagonal Architecture** and **Triple Dependency Injection (DI)**, making it independent of specific external details (WiFi, HTTP library, JSON library).

### 💡 Core Concept: Hexagonal Architecture (Ports & Adapters)

The main class (`ESP8266AIGemini`) acts as the application core and only interacts with injected interfaces (**Output Ports**):

  * `IGeminiValidator`: For pre-request checks (e.g., WiFi status).
  * `IGeminiApiGateway`: For HTTP/S communication with the Gemini endpoint.
  * `IGeminiParser`: For JSON serialization (creating payload) and deserialization (parsing response).

### Quick Start (Triple DI)

The main usage involves injecting the concrete implementations of the three ports into the core class's constructor:

```cpp
// 1. Instantiate the concrete implementations (Adapters)
GeminiValidator validator; 
ApiGatewayESP8266 gateway;
GeminiJsonParser parser;

// 2. Inject dependencies into the Core
ESP8266AIGemini gemini(
    GEMINI_API_KEY, 
    &validator,  // IGeminiValidator
    &gateway,    // IGeminiApiGateway
    &parser      // IGeminiParser
);

// 3. Use the core functionality
String prompt = "Tell me a fun fact.";
String response = gemini.generateContent(prompt);
```

### Main Methods

| Method | Description |
| :--- | :--- |
| `generateContent(prompt)` | Sends a text-only request to the model. |
| `imgToProcess(prompt, img_BASE64, mimeType)` | Sends a multimodal request (text + Base64 image). |
| `videoToProcess(prompt, video_cloud_url)` | Sends a multimodal request (text + video URI). |
| `setModel(modelName)` | Sets the target Gemini model (Default: `"gemini-2.5-flash"`). |

### 🛠️ Dependencies

To use this library, you must provide your own classes that implement the following interfaces:

1.  **`IGeminiValidator`** (e.g., check WiFi).
2.  **`IGeminiApiGateway`** (e.g., handle HTTP using `WiFiClientSecure`).
3.  **`IGeminiParser`** (e.g., handle JSON using `ArduinoJson`).

-----

Would you like a more detailed explanation of any of the concepts, like the Hexagonal Architecture?
