#include <cppunit/extensions/HelperMacros.h>
#include "Array.hpp"

class ArrayTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ArrayTest);
    CPPUNIT_TEST(test_init);
    CPPUNIT_TEST(test_add);
    CPPUNIT_TEST_SUITE_END();

protected:
    Array<int> *array;

public:
    void setUp();
    void tearDown();
protected:
    void test_init();
    void test_add();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ArrayTest);

void ArrayTest::setUp() {
  array = new Array<int>();
}

void ArrayTest::tearDown() {
    delete array;
}


void ArrayTest::test_init() {
    CPPUNIT_ASSERT_EQUAL(0, (int)array->getSize());
}

void ArrayTest::test_add(){
    int n = (int)array->getSize();
    array->add(10);
    CPPUNIT_ASSERT_EQUAL(n + 1, (int)array->getSize());
}


