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
    CPPUNIT_TEST(test_copy_constructor);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_core();
    void test_copy_constructor();
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
    
    //[test]add
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
    
    //[test]delete first item
    ar.del(0);
    
    CPPUNIT_ASSERT_EQUAL(4, (int)ar.getSize());
    CPPUNIT_ASSERT(i1 == ar.get(0));
    CPPUNIT_ASSERT(i2 == ar.get(1));
    CPPUNIT_ASSERT(i3 == ar.get(2));
    CPPUNIT_ASSERT(i4 == ar.get(3));
    
    //[test]delete middle item
    ar.del(1);
    
    CPPUNIT_ASSERT_EQUAL(3, (int)ar.getSize());
    CPPUNIT_ASSERT(i1 == ar.get(0));
    CPPUNIT_ASSERT(i3 == ar.get(1));
    CPPUNIT_ASSERT(i4 == ar.get(2));
    
    //[test]delete last item
    ar.del(2);
    
    CPPUNIT_ASSERT_EQUAL(2, (int)ar.getSize());
    CPPUNIT_ASSERT(i1 == ar.get(0));
    CPPUNIT_ASSERT(i3 == ar.get(1));
    
    //[test]del all item
    ar.del(0);
    ar.del(0);
    
    CPPUNIT_ASSERT_EQUAL(0, (int)ar.getSize());
    
    //[test]set
    
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
    
    //[test] get and mem access
    CPPUNIT_ASSERT_EQUAL(*ar.get(2), *i3);
    
    //[test] clear
    ar.clear();
    
    delete i0;
    delete i1;
    delete i2;
    delete i3;
    delete i4;
}


void FIXTURE_NAME::test_copy_constructor(){
    Array<int *> ar;
    Array<int *> ar_copy;
    
    int *i0 = new int(0);
    int *i1 = new int(1);
    int *i2 = new int(2);
    
    int *j = new int(10);
    
    ar.add(i0);
    ar.add(i1);
    ar.add(i2);
    
    ar_copy.add(j);
    
    CPPUNIT_ASSERT_EQUAL(3, (int)ar.getSize());
    CPPUNIT_ASSERT_EQUAL(1, (int)ar_copy.getSize());
    
    ar_copy = ar;
    
    CPPUNIT_ASSERT_EQUAL(3, (int)ar_copy.getSize());
    CPPUNIT_ASSERT(i0 == ar_copy.get(0));
    CPPUNIT_ASSERT(i1 == ar_copy.get(1));
    CPPUNIT_ASSERT(i2 == ar_copy.get(2));
    
    ar.clear();
    ar_copy.clear();
    
    delete i0;
    delete i1;
    delete i2;
    delete j;
}
