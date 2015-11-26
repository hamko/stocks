#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <cstdio>

using namespace CPPUNIT_NS;
class test : public TestFixture {
    CPPUNIT_TEST_SUITE(test);
    CPPUNIT_TEST(test_plus);
    CPPUNIT_TEST_SUITE_END();

    void test_plus() {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(8, 2.2+5.8, 1e-3);
    }

    public:
    void setUp() { // 始めに呼ばれる
    }
    void tearDown() { //後で呼ばれる
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(test);
