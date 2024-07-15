#include <Arduino.h>

#include <SPI.h>
#include <Ethernet.h>

#include <WebSocketsClient.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif

WebSocketsClient webSocket;
WiFiClient client;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

	switch(type) {
		case WStype_DISCONNECTED:
			USE_SERIAL.printf("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED: {
			USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);

			// send message to server when Connected
			webSocket.sendTXT("Connected");
		}
			break;
		case WStype_TEXT:
			USE_SERIAL.printf("[WSc] get text: %s\n", payload);

			// send message to server
			// webSocket.sendTXT("message here");
			break;
		case WStype_BIN:
			USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);

			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
        case WStype_PING:
            // pong will be send automatically
            USE_SERIAL.printf("[WSc] get ping\n");
            break;
        case WStype_PONG:
            // answer to a ping we send
            USE_SERIAL.printf("[WSc] get pong\n");
            break;
    }

}
int led = 13;
char *ssid1 = "Rishab's Galaxy M31";
char *pass1 = "ciqk4548";

char *ssid2 = "dugar 4g";
char *pass2 = "rishab0430";

char path[] = "/";
char host[] = "robota-lock.herokuapp.com";

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);

    Serial.println("Starting up...");
    DEBUG_MSG("bootup...\n");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Disconnect from an AP if it was previously connected
    // WiFi.disconnect();
    Serial.println("Connecting WiFi ");

    WiFi.begin(ssid2, pass2);
    Serial.print("connecting.");
    
   
    while(WiFi.status() != WL_CONNECTED){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(125);
      digitalWrite(LED_BUILTIN, LOW);
      delay(125);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(125);
      digitalWrite(LED_BUILTIN, LOW);
      delay(125);
      Serial.print(".");
    }

    Serial.println("");
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("connected");

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Starting Socket Connection...");

    // Connect to the websocket server ->
    // if (client.connect("echo.websocket.org", 80)) {
    //   Serial.println("Connected");
    // } 
    // else {
    //   Serial.println("Connection failed.");
    // }

    // webSocket.beginSSL(host, 80);
    // webSocket.beginSocketIO(host, 80, "/socket.io/?EIO=4", "https");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
    webSocket.enableHeartbeat( 25000,  25000,  60000);
    webSocket.beginSocketIOSSL(host, 443, "/socket.io/?EIO=4");
   
    // webSocket.setAuthorization();
    // webSocket.setExtraHeaders();
    
    // webSOcket.loop();
}

void loop()
{ 
    
    if(!webSocket.isConnected()){
          webSocket.loop();
    }
    // DEBUG_MSG("loop %d\n", millis());
    // delay(30000);
    // delay(3000);
}
