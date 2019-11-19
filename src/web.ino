#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer * _server;
char last_modified[50];

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/html", "Not found");
}

void onHome(AsyncWebServerRequest *request) {
    // Check if the client already has the same version and respond with a 304 (Not modified)
    if (request->header("If-Modified-Since").equals(last_modified)) {
        request->send(304);

    } else {
        // Dump the byte array in PROGMEM with a 200 HTTP code (OK)
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", index_html_gz, index_html_gz_len);
        // Tell the browswer the contemnt is Gzipped
        response->addHeader("Content-Encoding", "gzip");
        // And set the last-modified datetime so we can check if we need to send it again next time or not
        response->addHeader("Last-Modified", last_modified);
        request->send(response);
    }
}

AsyncWebServer * webServer() {
    return _server;
}

void webSetup() {
  _server = new AsyncWebServer(80);

  sprintf(last_modified, "%s %s GMT", __DATE__, __TIME__);
  _server->rewrite("/", "/index.html");
  _server->on("/index.html", HTTP_GET, onHome);
  _server->onNotFound(notFound);
  _server->begin();
}