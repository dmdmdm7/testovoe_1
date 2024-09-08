#include "buffer.h"

void commonBuf::write(const std::string& data){
    std::unique_lock<std::mutex> ul(mtx);
    buffer = data;
    readyFlag = true;
    cv.notify_one();
}

std::string commonBuf::read(){
    std::unique_lock<std::mutex> ul(mtx);
    cv.wait(ul, [this]{return readyFlag;});
    readyFlag = false;
    return buffer;    
}

void commonBuf::clearBuf(){
    buffer.clear();
}