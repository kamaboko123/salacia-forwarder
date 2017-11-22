#include "Exception.hpp"

namespace sfwdr{
    namespace Exception{
        void Exception::_init(char *str){
            sfwdr::ssize_t len = comlib::strlen(str) + 1;
            this->message = new char[len]();
            comlib::strncpy(this->message, str, len);
        }
        
        Exception::Exception(){
            _init((char *)"");
        }
        
        Exception::Exception(char *str){
            _init(str);
        }
        
        Exception::Exception(const Exception &except){
            _init(except.getMessage());
        }
        
        Exception &Exception::operator=(const Exception &except){
            if(this != &except){
                delete[] message;
                _init(except.getMessage());
            }
            return(*this);
        }
        
        Exception::~Exception(){
            delete[] message;
        }
        
        char *Exception::getMessage() const{
            return(message);
        }
    }
}

