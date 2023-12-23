# Websocket Chatroom application

Install dependencies:
```
pip install -r requirements.txt
```

Run:
```
uvicorn backend:app --host 0.0.0.0 --port 8001
```

To test, without frontend:
```
python3 -m websockets ws://localhost:8001/messages
```

To go via UI, startup server and browse to `http://localhost:8001/index.html`
