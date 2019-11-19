const WebSocket = require('ws');
const express = require('express');
const path = require('path');
const app = express();
const server = require('http').createServer(app);
var wss;
if(process.env.HTTP_SERVER) {
    wss = new WebSocket.Server({ server, path: "/ws" });
} else {
    wss = new WebSocket.Server({ port: 80 });
}


//Web server
app.use(express.static(path.join(__dirname, '../build')));
app.get('/', function(req, res) {
  res.sendFile(path.join(__dirname, '../build', 'index.html'));
});

//Web sockets
wss.on('connection', function connection(ws) {
    var state = false;
    var cont = 0;
    ws.on('message', function incoming(message) {
        console.log('received: %s', message);
        if (message.includes('wifi')) {
            ws.send(JSON.stringify({ restart: true }));
        }
        state = !state;
        ws.send(JSON.stringify({ relay: state, boardName: "IR-TV-BOARD" }));
    });
    const intervalId = setInterval(function () {
        ws.send(JSON.stringify({ code: "code " + cont }));
        cont++;
        if (cont > 8) {
            clearInterval(intervalId);
        }
    }, 1000);
    ws.send(JSON.stringify({ relay: state, boardName: "IR-TV-BOARD" }));
});

if(process.env.HTTP_SERVER) {
    server.listen(8080);
}
console.log('Localhost server running...');
