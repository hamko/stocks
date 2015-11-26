#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <cstdio>
#include <vector>
#include "leasqr_line.hpp"
using namespace std;

using namespace CPPUNIT_NS;
class testLeasqrLine : public TestFixture {
    CPPUNIT_TEST_SUITE(testLeasqrLine);
    CPPUNIT_TEST(test_safe);
    CPPUNIT_TEST(test_twopoints_line);
    CPPUNIT_TEST(test_threepoints_line);
    CPPUNIT_TEST_SUITE_END();

    void test_twopoints_line() {
        vector<double> x, y;
        double a, b;

        x.clear(); y.clear();
        x = {-1, 1}; y = {0, 0};
        leasqr_line(x, y, 0, 2, LEASQR_LINE_UPPER, 1, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, b, 1e-3);
        leasqr_line(x, y, 0, 2, LEASQR_LINE_LOWER, 1, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, b, 1e-3);
        leasqr_line(x, y, 0, 2, LEASQR_LINE_UPPER, 10, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, b, 1e-3);
        leasqr_line(x, y, 0, 2, LEASQR_LINE_LOWER, 10, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, b, 1e-3);

        x.clear(); y.clear();
        x = {-1, 1}; y = {-2, 2};
        leasqr_line(x, y, 0, 2, LEASQR_LINE_UPPER, 1, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(2, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, b, 1e-3);
        leasqr_line(x, y, 0, 2, LEASQR_LINE_LOWER, 1, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(2, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, b, 1e-3);
        leasqr_line(x, y, 0, 2, LEASQR_LINE_UPPER, 10, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(2, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, b, 1e-3);
        leasqr_line(x, y, 0, 2, LEASQR_LINE_LOWER, 10, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(2, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, b, 1e-3);
    }

    void test_threepoints_line() {
        vector<double> x, y;
        double a, b;

        x.clear(); y.clear();
        x = {-2, -1, 1, 2}; y = {-3, 0, 0, -3};
        leasqr_line(x, y, 0, x.size(), LEASQR_LINE_LOWER, 10, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-3, b, 1e-1);
        leasqr_line(x, y, 0, x.size(), LEASQR_LINE_UPPER, 10, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, b, 1e-1);
        leasqr_line(x, y, 0, x.size(), LEASQR_LINE_LOWER, 1, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.5, b, 1e-1);
        leasqr_line(x, y, 0, x.size(), LEASQR_LINE_UPPER, 1, &a, &b);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, a, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.5, b, 1e-1);
    }


    void test_safe() {
        vector<double> x, y;
        double a, b;

        // Blank
        x.clear(); y.clear();
        CPPUNIT_ASSERT_EQUAL(leasqr_line(x, y, 0, 2, LEASQR_LINE_UPPER, 1, &a, &b), 1);
        CPPUNIT_ASSERT_EQUAL(leasqr_line(x, y, 0, 2, LEASQR_LINE_LOWER, 1, &a, &b), 1);

        // Small
        x.clear(); y.clear();
        x = {-1, 1}; y = {-2, 2};
        CPPUNIT_ASSERT_EQUAL(leasqr_line(x, y, 0, 3, LEASQR_LINE_LOWER, 1, &a, &b), 1);
        CPPUNIT_ASSERT_EQUAL(leasqr_line(x, y, 1, 2, LEASQR_LINE_LOWER, 1, &a, &b), 1);

        // Len
        CPPUNIT_ASSERT_EQUAL(leasqr_line(x, y, 0, 1, LEASQR_LINE_LOWER, 1, &a, &b), 1);

        // Coeff Negative
        CPPUNIT_ASSERT_EQUAL(leasqr_line(x, y, 0, 2, LEASQR_LINE_LOWER, -1, &a, &b), 1);
    }

    public:
    void setUp() { // 始めに呼ばれる
    }
    void tearDown() { //後で呼ばれる
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(testLeasqrLine);
