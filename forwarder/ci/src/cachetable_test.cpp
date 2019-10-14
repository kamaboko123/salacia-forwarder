#include <cppunit/extensions/HelperMacros.h>
#include <unistd.h>
#include "CacheTable.hpp"
#include "libtest.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif
#define FIXTURE_NAME CacheTableTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_core);
    CPPUNIT_TEST(test_with_pointer_wrapper);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_core();
    void test_with_pointer_wrapper();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_core(){
    CacheTable<KeyMock, int> tbl;
    CPPUNIT_ASSERT_EQUAL(0, (int)tbl.getSize());
    
    tbl.update(KeyMock(0), 10, true);
    tbl.update(KeyMock(1), 11);
    tbl.update(KeyMock(2), 12);
    tbl.update(KeyMock(3), 13);
    
    CPPUNIT_ASSERT_EQUAL(10, (int)tbl.get(KeyMock(0)));
    CPPUNIT_ASSERT_EQUAL(11, (int)tbl.get(KeyMock(1)));
    CPPUNIT_ASSERT_EQUAL(12, (int)tbl.get(KeyMock(2)));
    CPPUNIT_ASSERT_EQUAL(13, (int)tbl.get(KeyMock(3)));
    
    CPPUNIT_ASSERT_EQUAL(4, (int)tbl.getSize());
    
    sleep(6);
    tbl.refresh();
    
    CPPUNIT_ASSERT(true == tbl.isExist(KeyMock(0)));
    CPPUNIT_ASSERT(true == tbl.isExist(KeyMock(1)));
    CPPUNIT_ASSERT(true == tbl.isExist(KeyMock(2)));
    CPPUNIT_ASSERT(true == tbl.isExist(KeyMock(3)));
    
    CPPUNIT_ASSERT_EQUAL(11, (int)tbl.get(KeyMock(1)));
    CPPUNIT_ASSERT_EQUAL(12, (int)tbl.get(KeyMock(2)));
    CPPUNIT_ASSERT_EQUAL(13, (int)tbl.get(KeyMock(3)));
    
    tbl.update(KeyMock(4), 14);
    
    sleep(7);
    tbl.refresh();
    
    CPPUNIT_ASSERT_EQUAL(2, (int)tbl.getSize());
    
    CPPUNIT_ASSERT(false == tbl.isExist(KeyMock(1)));
    CPPUNIT_ASSERT(false == tbl.isExist(KeyMock(2)));
    CPPUNIT_ASSERT(false == tbl.isExist(KeyMock(3)));
    
    CPPUNIT_ASSERT(true == tbl.isExist(KeyMock(4)));
    CPPUNIT_ASSERT_EQUAL(14, (int)tbl.get(KeyMock(4)));
    CPPUNIT_ASSERT(true == tbl.isExist(KeyMock(0)));
    CPPUNIT_ASSERT_EQUAL(10, (int)tbl.get(KeyMock(0)));
}

void FIXTURE_NAME::test_with_pointer_wrapper(){
    CacheTable<KeyMock, PointerWrapper<int>> tbl;
    
    int *i1 = (int *)malloc(sizeof(int));
    int *i2 = (int *)malloc(sizeof(int));
    int *i3 = (int *)malloc(sizeof(int));
    
    tbl.update(KeyMock(1), PointerWrapper<int>(i1));
    tbl.update(KeyMock(2), PointerWrapper<int>(i2));
    tbl.update(KeyMock(3), PointerWrapper<int>(i3));
    
    CPPUNIT_ASSERT(i1 == tbl.get(KeyMock(1)).unwrap());
    CPPUNIT_ASSERT(i2 == tbl.get(KeyMock(2)).unwrap());
    CPPUNIT_ASSERT(i3 == tbl.get(KeyMock(3)).unwrap());
    
    CPPUNIT_ASSERT(nullptr == tbl.get(KeyMock(4)).unwrap());
    
    free(i1);
    free(i2);
    free(i3);
    
}
