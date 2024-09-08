#include "engine.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include <arpa/inet.h>
#include <unistd.h>

Reader::Reader(commonBuf& buf): buffer(buf){}
Writer::Writer(commonBuf& buf): buffer(buf){}

bool Writer::isValidInput(const std::string& str) const{
    int n = str.size();
    if (n == 0 || n > 64) return false;
    for (int i = 0; i < n; i++){
        if (!(str[i] >= '0' && str[i] <= '9')) return false;
    }
    return true;
}

std::string Writer::modifyStr(std::string& str) const{
    std::sort(str.begin(), str.end(), [](char a, char b){
        return a > b;
    });
    std::string result = "";
    result.reserve(str.size());
    for (int i = 0; i < str.size(); i++){
        if ((str[i]-'0') % 2 == 0) result += "KB";
        else result += str[i];
    }
    return result;
}

int Reader::getSum(const std::string& str) const{
    int result = 0;
    for (int i = 0; i < str.size(); i++){
        if (str[i] >= '0' && str[i] <= '9') result += (str[i] - '0');
    }
    return result;
}

void Reader::clear(){
    buffer.clearBuf();
}

void Writer::doJob(){
    while (1){
        std::cout << "Thread 1: enter a string of numbers with a maximum length of 64 characters\n";
        std::string temp;
        std::cin >> temp;
        if (isValidInput(temp)){
            buffer.write(modifyStr(temp));
        }
        else std::cerr << "Invalid input. Try again.\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void Reader::doJob() {
    while (1){
        std::string data = buffer.read();
        buffer.clearBuf();
        int sum = getSum(data);
        std::string sumStr = std::to_string(sum);
        std::cout << "Thread 2: received data: " << data << ", sending sum = " << sum << " to prog2\n\n";

        int sock, con, attempts = 5;
        while ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 && attempts-- > 0){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        if (sock == -1){
            std::cerr << "Socket creation error\n";
            continue;
        }
        attempts = 5;
        struct sockaddr_in serv_addr = {0};

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(8080);

        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
            std::cerr << "Invalid address or address not supported\n";
            close(sock);
            return;
        }

        while ((con = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) == -1 && attempts-- > 0){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        if (con == -1){
            std::cerr << "Connect error\n";
            close(sock);
            continue;
        }

        if (send(sock, sumStr.c_str(), sumStr.length()+1, 0) == -1){
            std::cerr << "Send error\n";
        }
        
        close(sock);
    }
}
