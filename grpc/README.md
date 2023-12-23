# Build the docker image

```
docker build . -t ws/grpc
```

`exercises/` contains the skeleton code that will also be in the Dockerfile

Run the docker image as:

```
docker run -it --rm --name grpc -v `pwd`/exercises:/ws ws/grpc
```

And can exec into it again as:

```
docker exec -it grpc bash
```

# Compile the proto

```
python -m grpc_tools.protoc -I./protos --python_out=. --grpc_python_out=. ./protos/calculator.proto
```
