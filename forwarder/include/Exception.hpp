#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION

#include "comlib.hpp"

namespace sfwdr{
    namespace Exception{
        class Exception{
        private:
            char *message;
            
            void _init(char *str);
            
        public:
            Exception();
            Exception(char *str);
            ~Exception();
            
            Exception(const Exception &except);
            Exception &operator=(const Exception &except);
            
            void setMessage(char *str);
            char *getMessage() const;
        };
        
        class OutOfRange : public Exception{
        public:
            OutOfRange() : Exception((char *)"out-of-range"){};
        };
        
        class InvalidEthType : public Exception{
        public:
            InvalidEthType(char *str);
        };
        
        class InvalidIPAddress : public Exception{
        public:
            InvalidIPAddress(const char *str);
        };
        
        class InvalidIPNetmask : public Exception{
        public:
            InvalidIPNetmask(char *str);
        };
        
        class InvalidIPNetwork : public Exception{
        public:
            InvalidIPNetwork(char *str);
        };
    }
}

#endif
