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
            
            char *getMessage() const;
        };
        
        class OutOfRange : public Exception{
        public:
            OutOfRange() : Exception((char *)"out-of-range"){};
        };
    }
}
