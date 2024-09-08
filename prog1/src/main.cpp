#include <iostream>
#include <thread>
#include "engine.h"
#include "buffer.h"

void readerFun(commonBuf& buffer){
    Reader rd(buffer);
    rd.doJob();
}

void writerFun(commonBuf& buffer){
    Writer wr(buffer);
    wr.doJob();
}

int main(){
    commonBuf buffer;
    std::thread t1(readerFun, std::ref(buffer));
    std::thread t2(writerFun, std::ref(buffer));
    t1.join();
    t2.join();
}