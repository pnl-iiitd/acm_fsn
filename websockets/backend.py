#!/usr/bin/env python3

import fastapi
from fastapi.staticfiles import StaticFiles
import names

class Room():
    def __init__(self):
        self.members = []

    def join(self, member):
        self.members.append(member)

    def exit(self, member):
        self.members.remove(member)

    async def say(self, msg):
        for m in self.members:
            await m.send_text(msg)

app = fastapi.FastAPI()
room = Room()

@app.websocket("/messages")
async def connect_room(websocket: fastapi.WebSocket):
    await websocket.accept()
    room.join(websocket)
    my_name = names.get_first_name()

    await room.say(my_name + " joined the room.")

    try:
        while True:
            msg = await websocket.receive_text()
            await room.say(my_name + ": " + msg)
    except fastapi.WebSocketDisconnect:
        room.exit(websocket)
        await room.say(my_name + " left the room.")

app.mount("/", StaticFiles(directory="frontend"), name="frontend")
