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
    CPPUNIT_TEST(test_route_core);
    CPPUNIT_TEST(test_route_invalid);
    CPPUNIT_TEST_SUITE_END();

private:
    void copy_constructor_test(Route target, Route *expect);
public:
    void setUp();
    void tearDown();
protected:
    void test_route_constructor();
    void test_route_core();
    void test_route_invalid();
    void test_route_table_core();
};

void FIXTURE_NAME::copy_constructor_test(Route target, Route *expect){
    //WIP
}

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp() {}

void FIXTURE_NAME::tearDown() {}

void FIXTURE_NAME::test_route_constructor(){
    Route r1("10.0.0.0/8");
    
    //str
    Array<IPAddress> bests;
    Array<RouteType> route_types;
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getNexthops(RTYPE_LOCAL).getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getBestNexthops(bests));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, bests.getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getBestNexthops().getSize());
    CPPUNIT_ASSERT_EQUAL(RTYPE_INVALID, r1.getBestRouteType());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getRouteTypes(route_types));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, route_types.getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1.getRouteTypes().getSize());
    
    CPPUNIT_ASSERT_EQUAL(0, strcmp("10.0.0.0/8", r1.getNetwork().toStr()));
    
    
    //IPNetwork
    IPNetwork nw("20.0.0.0/16");
    Route r2(nw);
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getNexthops(RTYPE_LOCAL).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getNexthops(RTYPE_CONNECTED).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getNexthops(RTYPE_STATIC).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getNexthops(RTYPE_RIP).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getNexthops(RTYPE_INVALID).getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getBestNexthops(bests));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, bests.getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getBestNexthops().getSize());
    CPPUNIT_ASSERT_EQUAL(RTYPE_INVALID, r2.getBestRouteType());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getRouteTypes(route_types));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, route_types.getSize());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r2.getRouteTypes().getSize());
    
    CPPUNIT_ASSERT_EQUAL(0, strcmp("20.0.0.0/16", r2.getNetwork().toStr()));
    
    //copy constructor
    
}

void FIXTURE_NAME::test_route_core(){
    Route *r1;
    r1 = new Route("10.0.0.0/8");
    r1->addNexthop(RTYPE_LOCAL, IPAddress("1.1.1.1"));
    
    Array<IPAddress> bests;
    Array<RouteType> route_types;
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getNexthops(RTYPE_LOCAL).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1->getNexthops(RTYPE_CONNECTED).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1->getNexthops(RTYPE_STATIC).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1->getNexthops(RTYPE_RIP).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1->getNexthops(RTYPE_INVALID).getSize());
    
    CPPUNIT_ASSERT_EQUAL(IPAddress("1.1.1.1").touInt(), r1->getNexthops(RTYPE_LOCAL).get(0).touInt());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getBestNexthops(bests));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, bests.getSize());
    CPPUNIT_ASSERT_EQUAL(IPAddress("1.1.1.1").touInt(), bests.get(0).touInt());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getBestNexthops().getSize());
    CPPUNIT_ASSERT_EQUAL(RTYPE_LOCAL, r1->getBestRouteType());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getRouteTypes(route_types));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, route_types.getSize());
    CPPUNIT_ASSERT_EQUAL(RTYPE_LOCAL, route_types.get(0));
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getRouteTypes().getSize());
    
    
    
    r1->addNexthop(RTYPE_CONNECTED, IPAddress("2.2.2.2"));
    r1->addNexthop(RTYPE_STATIC, IPAddress("3.3.3.3"));
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getNexthops(RTYPE_LOCAL).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getNexthops(RTYPE_CONNECTED).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getNexthops(RTYPE_STATIC).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1->getNexthops(RTYPE_RIP).getSize());
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)0, r1->getNexthops(RTYPE_INVALID).getSize());
    
    CPPUNIT_ASSERT_EQUAL(IPAddress("1.1.1.1").touInt(), r1->getNexthops(RTYPE_LOCAL).get(0).touInt());
    CPPUNIT_ASSERT_EQUAL(IPAddress("2.2.2.2").touInt(), r1->getNexthops(RTYPE_CONNECTED).get(0).touInt());
    CPPUNIT_ASSERT_EQUAL(IPAddress("3.3.3.3").touInt(), r1->getNexthops(RTYPE_STATIC).get(0).touInt());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getBestNexthops(bests));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, bests.getSize());
    CPPUNIT_ASSERT_EQUAL(IPAddress("1.1.1.1").touInt(), bests.get(0).touInt());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)1, r1->getBestNexthops().getSize());
    CPPUNIT_ASSERT_EQUAL(RTYPE_LOCAL, r1->getBestRouteType());
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)3, r1->getRouteTypes(route_types));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)3, route_types.getSize());
    CPPUNIT_ASSERT_EQUAL(RTYPE_LOCAL, route_types.get(0));
    CPPUNIT_ASSERT_EQUAL(RTYPE_CONNECTED, route_types.get(1));
    CPPUNIT_ASSERT_EQUAL(RTYPE_STATIC, route_types.get(2));
    
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)3, r1->getRouteTypes().getSize());
    
    
    r1->addNexthop(RTYPE_STATIC, IPAddress("4.4.4.4"));
    CPPUNIT_ASSERT_EQUAL((sfwdr::size_t)2, r1->getNexthops(RTYPE_STATIC).getSize());
    CPPUNIT_ASSERT_EQUAL(IPAddress("3.3.3.3").touInt(), r1->getNexthops(RTYPE_STATIC).get(0).touInt());
    CPPUNIT_ASSERT_EQUAL(IPAddress("4.4.4.4").touInt(), r1->getNexthops(RTYPE_STATIC).get(1).touInt());
    
    delete r1;
}

void FIXTURE_NAME::test_route_invalid(){
    Route *r1;
    
    CPPUNIT_ASSERT_THROW(r1 = new Route("a"), sfwdr::Exception::InvalidIPNetwork);
}


void test_route_table_core(){
    //WIP
}
