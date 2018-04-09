#ifndef INCLUDED_EXCEPTION
#define INCLUDED_EXCEPTION

#include "comlib.hpp"

namespace sfwdr{
    namespace Exception{
        class Exception{
        private:
            char *message;
            
            void _init(const char *str);
            
        public:
            Exception();
            Exception(const char *str);
            ~Exception();
            
            Exception(const Exception &except);
            Exception &operator=(const Exception &except);
            
            void setMessage(const char *str);
            char *getMessage() const;
        };
        
        class OutOfRange : public Exception{
        public:
            OutOfRange() : Exception("out-of-range"){};
        };
        
        class InvalidEthType : public Exception{
        public:
            InvalidEthType(const char *str);
        };
        
        class InvalidIPAddress : public Exception{
        public:
            InvalidIPAddress(const char *str);
        };
        
        class InvalidIPNetmask : public Exception{
        public:
            InvalidIPNetmask(const char *str);
        };
        
        class InvalidIPNetwork : public Exception{
        public:
            InvalidIPNetwork(const char *str);
        };
    }
}

#endif
