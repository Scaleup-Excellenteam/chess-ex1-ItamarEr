//
// Created by Erez on 07/05/2025.
//

#ifndef PRIORITYQUEUEEXCEPTION_H
#define PRIORITYQUEUEEXCEPTION_H


#include <exception>
#include <string>
class PriorityQueueException : public std::exception {

    std::string message;
public:
    explicit PriorityQueueException(const std::string& message ) :message(message) {}
    const char* what() const noexcept override { return message.c_str(); }

};

#endif //PRIORITYQUEUEEXCEPTION_H
