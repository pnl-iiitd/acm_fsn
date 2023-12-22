```
docker build . -t ws/grpc
```

`exercises/` contains the skeleton code that will also be in the Dockerfile

Run the docker image as:

```
docker run -it --rm --name grpc ws/grpc
```

And can exec into it again as:

```
docker exec -it grpc bash
```
