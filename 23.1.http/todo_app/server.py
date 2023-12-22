from flask import Flask,jsonify,request
import json
app = Flask(__name__)

tasks = {"1": "Clean house", "2": "Finish assignment"}

@app.route('/tasks', methods = ['GET'])
def get_tasks():
    return jsonify({'tasks': tasks})

@app.route('/tasks/<task_id>', methods = ['GET'])
def get_task(task_id):
    if task_id in tasks:
        return jsonify({'task': tasks[task_id]})
    else:
        return jsonify({'error': 'Task does not exist'}), 404

@app.route('/tasks', methods = ['POST'])
def create_task():
    content_type = request.headers.get('Content-Type')
    if (content_type == 'application/json'):
        task = request.json
        tasks.update(task)
        return jsonify({'task': task}), 201
    else:
        return jsonify({'error': 'Please input json'}), 404
