#include <cstring>
#include <cppunit/extensions/HelperMacros.h>
#include "RouteTable.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME RouteTableTest

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_route_constructor);
    CPPUNIT_TEST(test_route_invalid);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_route_constructor();
    void test_route_invalid();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_route_constructor(){
    Route r1("10.0.0.0/8");
    
    //str
    Array<IPAddress> bests;
    Array<RouteType> best_types;
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getNexthops(RTYPE_LOCAL).getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getBestNexthops(bests));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, bests.getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getBestNexthops().getSize());
    CPPUNIT_ASSERT_EQUAL(RTYPE_INVALID, r1.getBestRouteType());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getRouteTypes(best_types));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, best_types.getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getRouteTypes().getSize());
    
    CPPUNIT_ASSERT_EQUAL(0, strcmp("10.0.0.0/8", r1.getNetwork().toStr()));
    
    
    //IPNetwork
    IPNetwork nw("20.0.0.0/16");
    Route r2(nw);
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getNexthops(RTYPE_LOCAL).getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getBestNexthops(bests));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, bests.getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getBestNexthops().getSize());
    CPPUNIT_ASSERT_EQUAL(RTYPE_INVALID, r2.getBestRouteType());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getRouteTypes(best_types));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, best_types.getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getRouteTypes().getSize());
    
    CPPUNIT_ASSERT_EQUAL(0, strcmp("20.0.0.0/16", r2.getNetwork().toStr()));
    
}

void FIXTURE_NAME::test_route_invalid(){
    Route *r;
    
    CPPUNIT_ASSERT_THROW(r = new Route("a"), sfwdr::Exception::InvalidIPNetwork);
}
