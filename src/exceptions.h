#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <stdexcept>

namespace BT {

class BehaviorTreeException : public std::exception {
public:
    explicit BehaviorTreeException(const std::string Message);
private:
    const char *Message;
};

}

#endif
