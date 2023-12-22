FROM python:3.6

RUN apt update && apt -y install ca-certificates vim
RUN python -m pip install --upgrade pip
RUN python -m pip install grpcio grpcio-tools

ENTRYPOINT ["/bin/bash"]
