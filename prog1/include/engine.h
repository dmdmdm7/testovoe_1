#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "buffer.h"

class Reader{
public:
    Reader(commonBuf& buf);
    void doJob();
    void clear();
private:
    commonBuf& buffer;
    int getSum(const std::string& str) const;
};

class Writer{
public:
    Writer(commonBuf& buf);
    void doJob();
private:
    commonBuf& buffer;
    bool isValidInput(const std::string& str) const;
    std::string modifyStr(std::string& str) const;
};
    
#endif