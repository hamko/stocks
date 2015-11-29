#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <cstdio>

#include "HoldingStock.hpp"
using namespace std;

using namespace CPPUNIT_NS;
class testHoldingStock : public TestFixture {
    CPPUNIT_TEST_SUITE(testHoldingStock);
    CPPUNIT_TEST(test_sell);
    CPPUNIT_TEST(test_buy);
    CPPUNIT_TEST(test_margin_sell);
    CPPUNIT_TEST(test_short_cover);
    CPPUNIT_TEST_SUITE_END();

    void test_buy() {
        HoldingStock toyota(7203);
        toyota.buy(100, 7200, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(100, toyota.m_num, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7200, toyota.m_price, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7190, toyota.m_sl, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7230, toyota.m_tp, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(720000, toyota.getNeededConsignmentGuaranteeMoney(), 1e-3);

        toyota.buy(100, 3600, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(200, toyota.m_num, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL((7200+3600)/2, toyota.m_price, 1e-3);
    }

    void test_sell() {
        HoldingStock toyota(7203);
        toyota.sell(100, 7200, 7210, 7190);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-100, toyota.m_num, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7200, toyota.m_price, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7210, toyota.m_sl, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(7190, toyota.m_tp, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-720000, toyota.getNeededConsignmentGuaranteeMoney(), 1e-3);

        toyota.sell(100, 3600, 7210, 7190);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-200, toyota.m_num, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL((7200+3600)/2, toyota.m_price, 1e-3);

    }

    void test_margin_sell() {
        HoldingStock toyota(7203);
        double profit;

        toyota.clear();
        profit = toyota.buy(100, 7200, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, profit, 1e-3);
        profit = toyota.sell(100, 7210, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1000, profit, 1e-3);

        toyota.clear();
        profit = toyota.buy(100, 7200, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, profit, 1e-3);
        profit = toyota.sell(200, 7210, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1000, profit, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-100, toyota.m_num, 1e-3);

        toyota.clear();
        profit = toyota.buy(200, 7200, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, profit, 1e-3);
        profit = toyota.sell(100, 7210, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1000, profit, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(100, toyota.m_num, 1e-3);
    }

    void test_short_cover() {
        HoldingStock toyota(7203);
        double profit;

        toyota.clear();
        profit = toyota.sell(100, 7210, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, profit, 1e-3);
        profit = toyota.buy(200, 7200, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1000, profit, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(100, toyota.m_num, 1e-3);

        toyota.clear();
        profit = toyota.sell(100, 7210, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, profit, 1e-3);
        profit = toyota.buy(100, 7200, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1000, profit, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, toyota.m_num, 1e-3);

        toyota.clear();
        profit = toyota.sell(200, 7210, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0, profit, 1e-3);
        profit = toyota.buy(100, 7200, 7190, 7230);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(1000, profit, 1e-3);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-100, toyota.m_num, 1e-3);


    }



    public:
    void setUp() { // 始めに呼ばれる
    }
    void tearDown() { //後で呼ばれる
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(testHoldingStock);
