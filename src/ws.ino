#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <algorithm>
#include <lib/WebSocketIncommingBuffer.h>

AsyncWebSocket _ws("/ws");
std::vector<uint32_t> _client_ids;

DynamicJsonDocument getStatus()
{
    const size_t capacity = JSON_OBJECT_SIZE(2);
    DynamicJsonDocument root(capacity + 50);

    root["relay"] = getTransistorStatus();
    String boardName = getBoardCode();
    Serial.println("Boardname: " + boardName);
    root["boardName"] = boardName;

    return root;
}

void _wsConnected(uint32_t client_id)
{
    DynamicJsonDocument root = getStatus();

    _client_ids.push_back(client_id);
    wsSend(client_id, root);
}

void _wsParse(AsyncWebSocketClient *client, uint8_t *payload, size_t length)
{
    uint32_t client_id = client->id();

    if ((length == 0) || (length == 1))
    {
        return;
    }

    if ((length == 3) && (strcmp((char *)payload, "{}") == 0))
    {
        return;
    }

    DynamicJsonDocument root(512);
    auto error = deserializeJson(root, (char *)payload);
    if (error)
    {
        Serial.print("[WEBSOCKET] JSON parsing error with code: ");
        Serial.println(error.c_str());
        return;
    }

    const char *action = root["action"];
    if (action)
    {
        Serial.printf("[WEBSOCKET] Requested action: %s \n", action);

        if (strcmp(action, "wifi") == 0)
        {
            String ssid = root["ssid"];
            String password = root["password"];
            sendRestart(client_id);
            setWifiCredentials(ssid, password);
        }
        else if (strcmp(action, "codes") == 0)
        {
            JsonArray codes = root["codes"];
            setCodes(codes);
        }
        else if (strcmp(action, "relay") == 0)
        {
            toggleTransistor();
            wsSend(client_id, getStatus());
        }
    }
}

void _onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        client->_tempObject = nullptr;
        IPAddress ip = client->remoteIP();
        Serial.printf("New client id: %u connected IP %d.%d.%d.%d", client->id(), ip[0], ip[1], ip[2], ip[3]);
        _wsConnected(client->id());
        client->_tempObject = new WebSocketIncommingBuffer(_wsParse, true);
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        //Remove client id with erase-remove idiom: https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
        _client_ids.erase(std::remove(_client_ids.begin(), _client_ids.end(), client->id()), _client_ids.end());
        Serial.printf("Client %u disconnected", client->id());

        if (client->_tempObject)
        {
            delete (WebSocketIncommingBuffer *)client->_tempObject;
        }
    }
    else if (type == WS_EVT_ERROR)
    {
        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t *)arg), (char *)data);
    }
    else if (type == WS_EVT_DATA)
    {
        Serial.println("Event data comming");
        if (!client->_tempObject)
            return;
        WebSocketIncommingBuffer *buffer = (WebSocketIncommingBuffer *)client->_tempObject;
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        buffer->data_event(client, info, data, len);
    }
}

void wsSend(uint32_t client_id, DynamicJsonDocument root)
{
    Serial.printf("Send message to %u \n", client_id);
    AsyncWebSocketClient *client = _ws.client(client_id);
    if (client == nullptr)
        return;

    size_t len = measureJson(root);
    AsyncWebSocketMessageBuffer *buffer = _ws.makeBuffer(len);

    if (buffer)
    {
        serializeJson(root, reinterpret_cast<char *>(buffer->get()), len + 1);
        client->text(buffer);
        root.clear();
    }
}

void sendRestart(uint32_t client_id)
{
    const size_t capacity = JSON_OBJECT_SIZE(1);
    DynamicJsonDocument root(capacity + 30);

    root["restart"] = true;

    wsSend(client_id, root);
    root.clear();
}

void sendCode(String code)
{
    if (_client_ids.size() == 0)
        return;

    const size_t capacity = JSON_OBJECT_SIZE(1);
    DynamicJsonDocument doc(capacity + 40);
    doc["code"] = code;

    for (unsigned int i = 0; i < _client_ids.size(); i++)
    {
        uint32_t client_id = _client_ids[i];
        wsSend(client_id, doc);
    }
}

AsyncWebSocket wsSetup()
{
    _ws.onEvent(_onEvent);
    webServer()->addHandler(&_ws);
    return _ws;
}