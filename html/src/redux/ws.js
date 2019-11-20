import { store } from './store'
import {
    updateRelay,
    addCode,
    boardRestart
} from './action'

var websock;

export const startWs = () => {
    var url = new URL("ws", process.env.production ? window.location : "http://localhost");
    url.protocol = "ws:";

    websock = new WebSocket(url.href);
    websock.onmessage = function (evt) {
        decodeMessage(evt.data);
    };
}

const decodeMessage = (stringMessage) => {
    console.log("Message -> " + stringMessage);

    const message = JSON.parse(stringMessage)
    if ('relay' in message && 'boardName' in message) {
        store.dispatch(updateRelay(message["relay"], message["boardName"]))
    } else if ('code' in message) {
        store.dispatch(addCode(message['code']));
    } else if ('restart' in message) {
        store.dispatch(boardRestart(true));
    }
}

export const sendWifi = (ssid, password) => {
    websock.send(JSON.stringify({
        action: "wifi",
        ssid,
        password
    }));
}

export const sendCodes = (codes) => {
    websock.send(JSON.stringify({
        action: "codes",
        codes
    }));
}

export const toggleRelay = () => {
    websock.send(JSON.stringify({ action: "relay" }));
}