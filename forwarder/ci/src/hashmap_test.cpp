#include <cstring>
#include <cppunit/extensions/HelperMacros.h>
#include "HashMap.hpp"
#include "libtest.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME HashMapTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_core);
    CPPUNIT_TEST_SUITE_END();

private:
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
    HashMap<KeyMock, int> *map = new HashMap<KeyMock, int>(3);
    
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getSize());
    for(int i = 0; i < 10; i++){
        map->update(KeyMock(i), i + 10);
    }
    
    CPPUNIT_ASSERT_EQUAL(10, (int)map->getSize());
    
    for(int i = 0; i < 10; i++){
        CPPUNIT_ASSERT(true == map->isExist(KeyMock(i)));
        CPPUNIT_ASSERT_EQUAL(10 + i, (int)map->get(KeyMock(i)));
    }
    
    map->del(KeyMock(0)); //first
    map->del(KeyMock(4)); //middle
    map->del(KeyMock(8)); //last
    
    CPPUNIT_ASSERT_EQUAL(7, (int)map->getSize());
    
    for(int i = 0; i < 10; i++){
        if((i == 0) || (i == 4) || (i == 8)){
            CPPUNIT_ASSERT(false == map->isExist(KeyMock(i)));
        }
        else{
            CPPUNIT_ASSERT(true == map->isExist(KeyMock(i)));
            CPPUNIT_ASSERT_EQUAL(10 + i, (int)map->get(KeyMock(i)));
        }
    }
    
    //test get with not exist key(should return default value but not add map item)
    CPPUNIT_ASSERT_EQUAL(7, (int)map->getSize());
    CPPUNIT_ASSERT(false == map->isExist(KeyMock(10)));
    CPPUNIT_ASSERT_EQUAL(0, (int)map->get(KeyMock(10)));
    CPPUNIT_ASSERT_EQUAL(7, (int)map->getSize());
    CPPUNIT_ASSERT(false == map->isExist(KeyMock(10)));
    
    map->clear();
    CPPUNIT_ASSERT_EQUAL(0, (int)map->getSize());
    
    for(int i = 0; i < 10; i++){
        map->update(KeyMock(i), i + 10);
    }
    CPPUNIT_ASSERT_EQUAL(10, (int)map->getSize());
    
    delete map;
}

