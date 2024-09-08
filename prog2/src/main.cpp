#include <iostream>
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address = {0};
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024];

    int status, attempts = 5;
    while ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 && attempts-- > 0){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (server_fd == -1){
        std::cerr << "Socket creation failed\n";
        exit(EXIT_FAILURE);

    }
    attempts = 5;
    
    while ((status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) == -1 && attempts-- > 0){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (status == -1){
        std::cerr << "Socket update failed\n";
        close (server_fd);
        exit(EXIT_FAILURE);
    }
    attempts = 5;
    
    while ((status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) == -1 && attempts-- > 0){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (status == -1){
        std::cerr << "Socket update failed\n";
        close (server_fd);
        exit(EXIT_FAILURE);
    }
    attempts = 5;
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    while ((status = bind(server_fd, (struct sockaddr*)&address, sizeof(address))) == -1 && attempts-- > 0){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (status == -1){
        std::cerr << "Bind error\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    attempts = 5;

    while ((status = listen(server_fd, 3)) == -1 && attempts-- > 0){
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (status == -1){
        std::cerr << "Listen error\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    while (1){
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) == -1){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        int rd = read(new_socket, buffer, sizeof(buffer)-1);
        if (rd > 1){
            buffer[rd-1] = '\0';
            std::string data(buffer);
            if (data.length() > 2){
                if (std::stoi(data) % 32 == 0){
                    std::cerr << "Valid data: " << data << '\n';
                } else{
                    std::cerr << "Data is not a multiple of 32: " << data << '\n';
                }
            } else{
                std::cerr << "Invalid data: " << data << '\n';
            }
        } else{
            std::cerr << "Read error or empty data\n";
        }
        close(new_socket);
    }
    close(server_fd);
    return 0;
}