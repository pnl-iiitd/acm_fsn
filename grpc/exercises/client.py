import logging

import grpc

# Import the generated python files
import ?
import ?

def run():
    # Connect to the server
    with grpc.insecure_channel("localhost:50001") as channel:

        # Create request of the AddRequest type
        request = ?

        # Create a client side stub
        stub = ?

        # Call the Add method from the stub to get the response
        response = ?

        print("Client received: {}".format(response.sum))

if __name__ == "__main__":
    logging.basicConfig()
    run()

