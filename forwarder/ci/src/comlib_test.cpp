#include <cppunit/extensions/HelperMacros.h>
#include <cstring>
#include "comlib.hpp"

#ifdef FIXTURE_NAME
#undef FIXTURE_NAME
#endif

#define FIXTURE_NAME ComlibTest

namespace clib = comlib;

class FIXTURE_NAME : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(FIXTURE_NAME);
    CPPUNIT_TEST(test_strncpy);
    CPPUNIT_TEST(test_strcat);
    CPPUNIT_TEST(test_strncat);
    CPPUNIT_TEST(test_strchr);
    CPPUNIT_TEST(test_strchr_index);
    CPPUNIT_TEST(test_bytestol);
    CPPUNIT_TEST(test_memcpy);
    CPPUNIT_TEST(test_memmove);
    CPPUNIT_TEST(test_memset);
    CPPUNIT_TEST(test_strlen);
    CPPUNIT_TEST(test_rbyte_n);
    CPPUNIT_TEST(test_convert_nw_byte_order);
    CPPUNIT_TEST(test_atoi);
    CPPUNIT_TEST(test_uitoa);
    CPPUNIT_TEST(test_isdigit);
    CPPUNIT_TEST(test_ndigit);
    CPPUNIT_TEST(test_upow);
    CPPUNIT_TEST(test_getBit);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
protected:
    void test_strncpy();
    void test_strcat();
    void test_strncat();
    void test_strchr();
    void test_strchr_index();
    void test_bytestol();
    void test_memcpy();
    void test_memmove();
    void test_memset();
    void test_strlen();
    void test_rbyte_n();
    void test_convert_nw_byte_order();
    void test_atoi();
    void test_uitoa();
    void test_isdigit();
    void test_ndigit();
    void test_upow();
    void test_getBit();
};

CPPUNIT_TEST_SUITE_REGISTRATION(FIXTURE_NAME);

void FIXTURE_NAME::setUp(){}
void FIXTURE_NAME::tearDown(){}

void FIXTURE_NAME::test_strncpy(){
    char *str;
    
    //配列の動的確保時に()つけると0で初期化されるんですが、これってC++規格？
    //↑規格らしいです
    str =  new char[16]();
    CPPUNIT_ASSERT(str == clib::strncpy(str, (char *)"abc", 3));
    CPPUNIT_ASSERT_EQUAL(0, strcmp(str, "abc"));
    delete[] str;
    
    str = new char[16]();
    CPPUNIT_ASSERT(str == clib::strncpy(str, (char *)"abc", 2));
    CPPUNIT_ASSERT_EQUAL(0, strcmp(str, "ab"));
    delete[] str;
}

void FIXTURE_NAME::test_strcat(){
    char s1[16] = "abc";
    char s2[16] = "123";
    
    CPPUNIT_ASSERT(s1 == clib::strcat(s1, s2));
    CPPUNIT_ASSERT_EQUAL(0, strcmp(s1, "abc123"));
}

void FIXTURE_NAME::test_strncat(){
    char dst0[16] = "abc";
    char dst1[16] = "abc";
    char src[16] = "123";
    
    CPPUNIT_ASSERT(dst0 == clib::strncat(dst0, src, 2));
    CPPUNIT_ASSERT_EQUAL(0, strcmp(dst0, "abc12"));
    
    CPPUNIT_ASSERT(dst1 == clib::strncat(dst1, src, 5));
    CPPUNIT_ASSERT_EQUAL(0, strcmp(dst1, "abc123"));
}

void FIXTURE_NAME::test_strchr(){
    char str[16] = "abcdef";
    CPPUNIT_ASSERT((str + 2) == clib::strchr(str, 'c'));
    CPPUNIT_ASSERT(nullptr == clib::strchr(str, 'z'));
}

void FIXTURE_NAME::test_strchr_index(){
    char str[16] = "abcdef";
    CPPUNIT_ASSERT_EQUAL(2, clib::strchr_index(str, 'c'));
    CPPUNIT_ASSERT_EQUAL(-1, clib::strchr_index(str, 'z'));
}

void FIXTURE_NAME::test_bytestol(){
    uint8_t bytes[] = {0x11, 0x22, 0x33, 0xAA, 0xBB, 0xCC};
    CPPUNIT_ASSERT_EQUAL((uint64_t)0x112233, clib::bytestol(bytes, 3));
    CPPUNIT_ASSERT_EQUAL((uint64_t)0x112233AABBCC, clib::bytestol(bytes, 6));
}

void FIXTURE_NAME::test_memcpy(){
    uint8_t bytes[8] = {'a', 'b', 'c', 'd', 0, 0, 0, 0};
    CPPUNIT_ASSERT(bytes == clib::memcpy(bytes, (const uint8_t *)"123", 2));
    CPPUNIT_ASSERT_EQUAL(0, memcmp(bytes, "12cd\0\0\0\0", 8));
}

void FIXTURE_NAME::test_memmove(){
    uint8_t str[8] = "abcdefg";
    
    //1 byteずらす
    clib::memmove(str + 1, str, 6);
    CPPUNIT_ASSERT_EQUAL(0, memcmp(str, "aabcdef\0", 8));
}

void FIXTURE_NAME::test_memset(){
    uint8_t bytes[8] = "abc";
    CPPUNIT_ASSERT(8 == clib::memset(bytes, 0, 8));
    CPPUNIT_ASSERT_EQUAL(0, memcmp(bytes, "\0\0\0\0\0\0\0\0", 8));
    CPPUNIT_ASSERT(8 == clib::memset(bytes, 'a', 8));
    CPPUNIT_ASSERT_EQUAL(0, memcmp(bytes, "aaaaaaaa", 8));
}

void FIXTURE_NAME::test_strlen(){
    CPPUNIT_ASSERT_EQUAL(6, clib::strlen((char *)"string"));
}

void FIXTURE_NAME::test_rbyte_n(){
    CPPUNIT_ASSERT( 0xFFAA == clib::rbyte16(0xAAFF));
    CPPUNIT_ASSERT(0x44332211 == clib::rbyte32(0x11223344));
}

void FIXTURE_NAME::test_convert_nw_byte_order(){
    //endian dependent
    //test for little endian test
    
    CPPUNIT_ASSERT(0xFFAA == clib::ntohs(0xAAFF));
    CPPUNIT_ASSERT(0xFFAA == clib::htons(0xAAFF));
    
    CPPUNIT_ASSERT(0x44332211 == clib::ntohl(0x11223344));
    CPPUNIT_ASSERT(0x44332211 == clib::htonl(0x11223344));
}

void FIXTURE_NAME::test_atoi(){
    CPPUNIT_ASSERT_EQUAL(1234567890, clib::atoi((char *)"1234567890"));
    CPPUNIT_ASSERT_EQUAL(12345, clib::atoi((char *)"12345a67890"));
    CPPUNIT_ASSERT_EQUAL(0, clib::atoi((char *)"a1234567890"));
}

void FIXTURE_NAME::test_uitoa(){
    char buf[8];
    
    CPPUNIT_ASSERT(buf == clib::uitoa(123, buf, 8));
    CPPUNIT_ASSERT_EQUAL(0, strcmp("123", buf));
    CPPUNIT_ASSERT(nullptr == clib::uitoa(123, buf, 2));
    CPPUNIT_ASSERT_EQUAL(0, strcmp("123", buf));
}

void FIXTURE_NAME::test_isdigit(){
    CPPUNIT_ASSERT_EQUAL(true, clib::isdigit('1'));
    CPPUNIT_ASSERT_EQUAL(true, clib::isdigit('9'));
    CPPUNIT_ASSERT_EQUAL(false, clib::isdigit('a'));
}

void FIXTURE_NAME::test_ndigit(){
    CPPUNIT_ASSERT(1 == clib::ndigit(8));
    CPPUNIT_ASSERT(2 == clib::ndigit(89));
    CPPUNIT_ASSERT(9 == clib::ndigit(123456789));
}

void FIXTURE_NAME::test_upow(){
    CPPUNIT_ASSERT(8 == clib::upow(2, 3));
    CPPUNIT_ASSERT(10000 == clib::upow(10, 4));
    CPPUNIT_ASSERT(4096 == clib::upow(16, 3));
    CPPUNIT_ASSERT(281474976710656 == clib::upow(2, 48));
}

void FIXTURE_NAME::test_getBit(){
    CPPUNIT_ASSERT_EQUAL(0, clib::getBit(0xBC, 0));
    CPPUNIT_ASSERT_EQUAL(0, clib::getBit(0xBC, 1));
    CPPUNIT_ASSERT_EQUAL(1, clib::getBit(0xBC, 2));
    CPPUNIT_ASSERT_EQUAL(1, clib::getBit(0xBC, 3));
    CPPUNIT_ASSERT_EQUAL(1, clib::getBit(0xBC, 4));
    CPPUNIT_ASSERT_EQUAL(1, clib::getBit(0xBC, 5));
    CPPUNIT_ASSERT_EQUAL(0, clib::getBit(0xBC, 6));
    CPPUNIT_ASSERT_EQUAL(1, clib::getBit(0xBC, 7));
}
