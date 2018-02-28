#include <cppunit/extensions/HelperMacros.h>
#include "Array.hpp"
#include "Exception.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif
#define FIXTURE_NAME ArrayTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_core);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_core();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_core(){
    Array<int *> ar;
    
    int *i0 = new int(0);
    int *i1 = new int(1);
    int *i2 = new int(2);
    int *i3 = new int(3);
    int *i4 = new int(4);
    
    CPPUNIT_ASSERT_EQUAL(0, (int)ar.getSize());
    CPPUNIT_ASSERT_THROW(ar.get(0), sfwdr::Exception::OutOfRange);
    
    ar.add(i0);
    ar.add(i1);
    ar.add(i2);
    ar.add(i3);
    ar.add(i4);
    
    CPPUNIT_ASSERT_EQUAL(5, (int)ar.getSize());
    CPPUNIT_ASSERT(i0 == ar.get(0));
    CPPUNIT_ASSERT(i1 == ar.get(1));
    CPPUNIT_ASSERT(i2 == ar.get(2));
    CPPUNIT_ASSERT(i3 == ar.get(3));
    CPPUNIT_ASSERT(i4 == ar.get(4));
    
    ar.del(0);
    
    CPPUNIT_ASSERT_EQUAL(4, (int)ar.getSize());
    CPPUNIT_ASSERT(i1 == ar.get(0));
    CPPUNIT_ASSERT(i2 == ar.get(1));
    CPPUNIT_ASSERT(i3 == ar.get(2));
    CPPUNIT_ASSERT(i4 == ar.get(3));
    
    ar.del(1);
    
    CPPUNIT_ASSERT_EQUAL(3, (int)ar.getSize());
    CPPUNIT_ASSERT(i1 == ar.get(0));
    CPPUNIT_ASSERT(i3 == ar.get(1));
    CPPUNIT_ASSERT(i4 == ar.get(2));
    
    ar.del(2);
    
    CPPUNIT_ASSERT_EQUAL(2, (int)ar.getSize());
    CPPUNIT_ASSERT(i1 == ar.get(0));
    CPPUNIT_ASSERT(i3 == ar.get(1));
    
    ar.del(0);
    ar.del(0);
    
    CPPUNIT_ASSERT_EQUAL(0, (int)ar.getSize());
    
    ar.add(i0);
    ar.add(i1);
    ar.add(i2);
    ar.add(i3);
    ar.add(i4);
    
    ar.set(0, i3);
    ar.set(1, i3);
    ar.set(2, i3);
    
    
    CPPUNIT_ASSERT_EQUAL(5, (int)ar.getSize());
    CPPUNIT_ASSERT(i3 == ar.get(0));
    CPPUNIT_ASSERT(i3 == ar.get(1));
    CPPUNIT_ASSERT(i3 == ar.get(2));
    
    ar.clear();
    
    CPPUNIT_ASSERT_EQUAL(0, (int)ar.getSize());
    
    delete i0;
    delete i1;
    delete i2;
    delete i3;
    delete i4;
}


