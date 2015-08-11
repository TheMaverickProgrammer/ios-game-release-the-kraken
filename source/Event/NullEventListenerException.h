#ifndef NULL_EVENT_LISTENER_EXCEPTION_H
#define NULL_EVENT_LISTENER_EXCEPTION_H

#include <string>

class NullEventListenerException
{
private:
    std::string message;
public:
    
    NullEventListenerException()
    {
        this->message = "EventListener is NULL!";
    }
    
    const char* what() const throw()
    {
        return message.c_str();
    }
};

#endif