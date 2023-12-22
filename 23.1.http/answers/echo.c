/** Simple echo server using TCP
 */

// Standard library for print and exit
#include <stdio.h>
#include <stdlib.h>

// Socket library for socket, bind, listen and accept
#include <sys/socket.h>
#include <sys/types.h>

// Library for struct sockaddr_in
#include <netinet/in.h>

// Close file descriptor
#include <unistd.h>

// For memcpy
#include <string.h>

// Library for threading
#include <pthread.h>

#define PORT 9091
#define BUFFER_SIZE 10000

/** TODO:
    Function to return a HTTP response
*/
void handle_HTTP_request(char* request, size_t request_len, char* response, size_t* response_len) {
  snprintf(response, BUFFER_SIZE,
	   "HTTP/1.1 200 OK\r\n"
	   "Content-Type: text/plain\r\n"
	   "Content-Length: 13\r\n"
	   "\r\n"
	   "Hello World!\n");
  *response_len = strlen(response);
  return;
}

/* Function to respond to each client message
   Currently response is the same as the request
*/
void handle_request(char* request, size_t request_len, char* response, size_t* response_len) {
  memcpy(response, request, request_len);
  *response_len = request_len;
  return;
}

/* Function called by each server thread
 */
void *handle_client(void* arg) {
  int client_fd = (int) arg;

  char *buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));

  /* Receive a message from the client

     ssize_t recv(int socket, void *buffer, size_t length, int flags);

     socket: Socket to read from (client_fd)
     buffer: Buffer to read into (buffer)
     length: Size of the buffer (BUFFER_SIZE)
     flags: Advanced options (Not required)

     Return: Size of the message (different from the size of the buffer which is max size)
  */
  
  ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
  printf("Received message from client: %s\n", buffer);
  if (bytes_received > 0) {
    char *response = (char*) malloc(BUFFER_SIZE * sizeof(char));
    size_t response_len;

    handle_HTTP_request(buffer, bytes_received, response, &response_len);
    printf("Response: %s", response);

    /* Send message to the socket
       ssize_t send(int socket, const void *buffer, size_t length, int flags)
       Returns: number of bytes sent
    */
    /* send(socket, response, 5, 0); */
    send(client_fd, response, response_len, 0);
    }
}


// Start from here: START
int main(int argc, char *argv[]) {
  struct sockaddr_in server_addr;

  /** Create server socket and returns file descriptor
      Sockets library - Single API to use different protocols (apart from IP)

      Try `man socket` to understand the function call:
      `int socket(int domain, int type, int protocol)`

      domain: Address/Protocol Family AF_INET, AF_UNIX (local socket)
      type: SOCK_STREAM (TCP for IP) or SOCK_DGRAM (UDP for IP)
      protocol: `man 5 protocols` or `cat /etc/protocols`

      Return: -1 if error else file descriptor
  */
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  /**
     domain = AF_INET (Internet Protocol version 4 (IPv4))
     type = SOCK_STREAM => TCP
     protocol = 0 => IP
  */

  if (server_fd < 0) {
      printf("Socket failed\n");
      exit(1);
  }
	
  // Configure socket
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  /** Bind socket to an address
     `man 2 bind`

     int bind(int socket, // File descriptor of the created socket
              struct sockaddr *address, // Socket address struct to bind to
	      socklen_t address_len) // Size of the sockaddr struct
     
   */
  int bind_ret = bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
  if (bind_ret < 0) {
      printf("Bind failed\n");
      exit(1);
  }

  /** Listen for connections

      `man listen`

      int listen(int sockfd, // File descriptor of created socket
                int backlog) // Max length of queue of pending connections
		
   */
  int listen_ret = listen(server_fd, 10);
  if (listen_ret < 0) {
      printf("Listen failed\n");
      exit(1);
    }

  printf("Server listening on port: %d\n", PORT);

  // Actual server logic
  while(1) {
    // Client information struct
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    /** Accept connection from client
	`man 2 accept`

	int accept(int socket, // Created, bound and listening socket
	           struct sockaddr address, // Filled with the address of the incoming connecting entity
		   socklen_t *restrict address_len);

	Creates a new socket to talk to the client, original socket remains
	
     */
    int client_fd = accept(server_fd, 
		      (struct sockaddr*) &client_addr, 
		       &client_addr_len);
    if (client_fd < 0) {
	printf("Accept failed\n");
	continue;
    }
    printf("Accepted connection from client\n");

    /* Create a new thread to handle the client request
       Does not matter if you don't understand the below code

       The only important part is handle_client is the function that each thread will call and the client_fd argument is passed to it
    */

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client, (void*) client_fd);
    pthread_detach(thread_id);
  }

  close(server_fd);
  return 0;
}

