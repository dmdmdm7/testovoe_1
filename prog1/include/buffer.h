#pragma once
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <condition_variable>

class commonBuf{
public:
    std::string read();
    void write(const std::string& data);
    void clearBuf();
private:
    std::string buffer;
    std::mutex mtx;
    std::condition_variable cv;
    bool readyFlag = false;
};

#endif