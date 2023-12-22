# Python Todo App using Flask

```
pip install flask
```

## Run the program

```
flask --app server run
```

or

```
FLASK_APP=server.py flask run
```

## Test the application

GET all tasks:

```
curl localhost:5000/tasks
```

Response:

```
{"tasks":{"1":"Clean house","2":"Finish assignment"}}
```


GET a particular task (using id):

```
curl localhost:5000/tasks/1
```

Response:

```
{"task":"Clean house"}
```

POST a task:

```
curl -H 'Content-Type:application/json' -X POST localhost:5000/tasks -d '{"3": "something"}'
```
