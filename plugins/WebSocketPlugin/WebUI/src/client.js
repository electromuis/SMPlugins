import { v4 as uuidv4 } from 'uuid';

let requests = {}
export const client = new WebSocket("ws://192.168.2.2:8081");

client.onmessage = (e) => {
    const message = JSON.parse(e.data);
    console.log("Received: ", message);
    
    if(message.id && requests[message.id]) {
        const req = requests[message.id]
        if(message.success) {
            req.callback(message)
        }
        else {
            req.reject(message)
        }
    }
};

export function sendRequest(action, data) {
    return new Promise((resolve, reject) => {
        data.id = uuidv4()
        data.action = action
    
        requests[data.id] = {
            request: data,
            response: null,
            callback: resolve,
            reject
        }

        client.send(JSON.stringify(data))
    })
}

