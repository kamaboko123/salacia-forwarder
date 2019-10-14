#ifndef INCLUDED_LIBTEST
#define INCLUDED_LIBTEST

class KeyMock{
private:
    int key;
    
public:
    KeyMock();
    KeyMock(int v);
    int getHash();
    int getKey();
    bool operator==(KeyMock v);
};

#endif
