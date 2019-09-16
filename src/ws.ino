#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

void _onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {        
        Serial.println("New client connected");
    } else if(type == WS_EVT_DISCONNECT) {
        Serial.println("Client disconnected");
    } else if(type == WS_EVT_ERROR){        
        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
    } else if(type == WS_EVT_DATA) { 
        Serial.println("Event data comming lol");
    }
}