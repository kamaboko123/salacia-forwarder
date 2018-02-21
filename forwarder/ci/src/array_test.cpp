#include <cppunit/extensions/HelperMacros.h>
#include "Array.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif
#define FIXTURE_NAME ArrayTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
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

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {
  array = new Array<int>();
}

void FIXTURE_NAME::tearDown() {
    delete array;
}


void FIXTURE_NAME::test_init() {
    CPPUNIT_ASSERT_EQUAL(0, (int)array->getSize());
}

void FIXTURE_NAME::test_add(){
    int n = (int)array->getSize();
    array->add(10);
    CPPUNIT_ASSERT_EQUAL(n + 1, (int)array->getSize());
}


