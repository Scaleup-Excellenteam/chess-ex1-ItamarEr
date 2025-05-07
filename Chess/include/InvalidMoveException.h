//
// Created by Erez on 07/05/2025.
//

#ifndef INVALIDMOVEEXCEPTION_H
#define INVALIDMOVEEXCEPTION_H

#include <exception>
#include <string>

class InvalidMoveException : public std::exception {
    std::string message;
    int errorCode;
public:
    explicit InvalidMoveException(const int code) :errorCode(code) {
        setErrorMessage();
    }
    const char* what() const noexcept override { return message.c_str(); }
    int getErrorCode() const { return errorCode; }
    void setErrorMessage() {
        switch (errorCode) {
            case 11:
                message = "There is no piece at the source";
                break;
            case 12:
                message = "The piece in the source is piece of your opponent";
                break;
            case 13:
                message = "There one of your pieces at the destination";
            break;
            case 21:
                message = "Illegal movement of that piece";
            break;
            case 31:
                message = "This movement will cause checkmate";
            break;

            default:
                break;
        }
    }
};


#endif //INVALIDMOVEEXCEPTION_H
