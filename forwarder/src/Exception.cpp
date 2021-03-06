#include "Exception.hpp"

namespace sfwdr{
    namespace Exception{
        void Exception::_init(const char *str){
            sfwdr::ssize_t len = comlib::strlen(str) + 1;
            this->message = new char[len]();
            comlib::strncpy(this->message, str, len);
        }
        
        Exception::Exception(){
            _init("");
        }
        
        Exception::Exception(const char *str){
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
        
        void Exception::setMessage(const char *str){
            delete[] message;
            
            sfwdr::ssize_t len = comlib::strlen(str) + 1;
            this->message = new char[len]();
            comlib::strncpy(this->message, str, len);
        }
        
        char *Exception::getMessage() const{
            return(message);
        }
        
        InvalidEthType::InvalidEthType(const char *str){
            char msg_head[] = "InvalidEthType : ";
            char *tmp = new char[comlib::strlen(str) + comlib::strlen(msg_head) + 1]();
            comlib::strcat(tmp, msg_head);
            comlib::strcat(tmp, str);
            setMessage(tmp);
            
            delete[] tmp;
        }
        
        InvalidIPAddress::InvalidIPAddress(const char *str){
            char msg_head[] = "InvalidIPAddress : ";
            char *tmp = new char[comlib::strlen(str) + comlib::strlen(msg_head) + 1]();
            comlib::strcat(tmp, msg_head);
            comlib::strcat(tmp, str);
            setMessage(tmp);
            
            delete[] tmp;
        }
        
        InvalidIPNetmask::InvalidIPNetmask(const char *str){
            char msg_head[] = "InvalidIPNetmask : ";
            char *tmp = new char[comlib::strlen(str) + comlib::strlen(msg_head) + 1]();
            comlib::strcat(tmp, msg_head);
            comlib::strcat(tmp, str);
            setMessage(tmp);
            
            delete[] tmp;
        }
        
        InvalidIPNetwork::InvalidIPNetwork(const char *str){
            char msg_head[] = "InvalidIPNetwork : ";
            char *tmp = new char[comlib::strlen(str) + comlib::strlen(msg_head) + 1]();
            comlib::strcat(tmp, msg_head);
            comlib::strcat(tmp, str);
            setMessage(tmp);
            
            delete[] tmp;
        }
        
    }
}

