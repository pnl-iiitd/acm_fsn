from concurrent import futures
import logging

import grpc

# import the 2 generated py files
import ?
import ?
        
# Create a class calculator which implements the rpc method 
class Calculator(?):
    def ?(self, request, context):
        # 1. Get the inputs from the request
        # 2. Calculate the sum
        # 3. Return the result
        pass 

def serve():
    port = "50001"
    # Creates a grpc server object with 10 worker threads to handle requests
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    
    # Link this server with the GRPC service
    ?(?, server)

    # Start the server on the port
    server.add_insecure_port("[::]:" + port)
    server.start()
    print("Server started, listening on " + port)
    server.wait_for_termination()

if __name__ == "__main__":
    logging.basicConfig()
    serve()
