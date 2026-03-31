#include<iostream>
#include<cstring>
#include<unistd.h>
#include<arpa/inet.h>

int main()
{
    //create a socket
    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    //AF_INET - IPv4 protocol
    //SOCK_STREAM - TCP protocol
    //0 - default protocol (TCP for SOCK_STREAM)
    //so server_fd will hold the file descriptor for the socket that we will use to listen for incoming connections on the server.

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//this line sets the socket options to allow the server to reuse the address and port. This is useful when you want to restart the server without waiting for the previous socket to be released.

    //define the server address
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; //bind to all interfaces
    addr.sin_port = htons(8080); //port number      

    //bind
    bind(server_fd, (sockaddr*)&addr, sizeof(addr));

    //4 listen
    listen(server_fd, 10);

    std::cout << "listening on port 8080....\n";

    //5) accept one client
    int client_fd = accept(server_fd, NULL, NULL);

    // read data from client
    char buffer[1024] = {0};
    int bytes_received = recv(client_fd, buffer, 1024, 0);
    buffer[bytes_received] = '\0';

    std::cout << "Received from client: " << buffer << std::endl;

    for (int i = 0; buffer[i]; i++) {
        if (buffer[i] >= 'a' && buffer[i] <= 'z')
            buffer[i] = std::toupper(buffer[i]);
    }

    std::string res = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 5\r\n"
        "\r\n"
        "Hello";

    send(client_fd, res.c_str(), res.length(), 0);

    close(client_fd);
    close(server_fd);

    return (0);
}