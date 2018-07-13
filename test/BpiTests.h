#include "gtest/gtest.h"
#include "bpistats.h"

#include <tuple>

using bpistats::BpiStats;
using bpistats::Date;
using bpistats::PriceAndDate;
using bpistats::PriceByDay;

/**
 * Constants to make testing easier to read.
 */
constexpr Date sunday{2018, 07, 01};
constexpr Date monday{2018, 07, 02};
constexpr Date tuesday{2018, 07, 03};
constexpr Date wednesday{2018, 07, 04};
constexpr Date thursday{2018, 07, 05};
constexpr Date friday{2018, 07, 06};
constexpr Date saturday{2018, 07, 07};

constexpr double tolerance = 0.0001;

TEST(BpiStatsTest, ShouldSayIsValidWhenTimeSpanNotEmpty) {
    BpiStats stats({{friday, 0.0}});

    EXPECT_TRUE(stats.isValid());
}

TEST(BpiStatsTest, ShouldSayNotValidWhenTimeIsEmpty) {
    BpiStats stats({});

    EXPECT_FALSE(stats.isValid());
}

TEST(BpiStatsTest, ShouldGetNumberOfDataPoints) {
    PriceByDay priceByDay {
        {sunday, 6765.8013},
        {monday, 6877.1825},
        {saturday, 6548.33}
    };

    BpiStats stats{priceByDay};

    EXPECT_EQ(3, stats.numPoints());
}

TEST(BpiStatsTest, ShouldComplainWhenAveragingAnEmptyTimeSpan) {
    BpiStats stats({});
    
    EXPECT_THROW(stats.average(), bpistats::EmptyTimeSpan);
}

TEST(BpiStatsTest, ShouldReturnLowestAndHighestPoints) {
    auto low = 100.0;
    Date lowDate{friday};

    auto high = 300.0;
    Date highDate{wednesday};

    BpiStats stats({
        {highDate, high},
        {thursday, 200.0},
        {lowDate, low}
    });

    auto lowAndHigh = stats.getLowAndHigh();
    PriceAndDate expectedLow {low, lowDate};
    PriceAndDate actualLow = std::get<0>(lowAndHigh);

    PriceAndDate expectedHigh {high, highDate};
    PriceAndDate actualHigh = std::get<1>(lowAndHigh);

    EXPECT_EQ(expectedLow, actualLow);
    EXPECT_EQ(expectedHigh, actualHigh);
}

TEST(BpiStatsTest, LowAndHighShouldBeTheSameWithOnlyOneDataPoint) {
    auto value = 200.1234;
    auto date = sunday;

    BpiStats stats({{date, value}});

    auto lowAndHigh = stats.getLowAndHigh();
    PriceAndDate expected {value, date};

    PriceAndDate actualLow = std::get<0>(lowAndHigh);
    PriceAndDate actualHigh = std::get<1>(lowAndHigh);

    EXPECT_EQ(expected, actualLow);
    EXPECT_EQ(expected, actualHigh);
}

TEST(BpiStatsTest, AveragePrice) {
    BpiStats stats({
        {monday, 1.0},
        {tuesday, 2.0},
        {wednesday, 3.0},
        {thursday, 4.0},
        {friday, 5.0}
    });

    auto average = stats.average();

    EXPECT_EQ(3.0, average);
}

TEST(BpiStatsTest, CalculateMedianGivenAnEvenNumberOfPoints) {
    BpiStats stats({
        {monday, 1.0},
        {tuesday, 2.0},
        {wednesday, 3.0},
        {thursday, 4.0},
    });

    auto median = stats.median();
    EXPECT_EQ(2.5, median);
}

TEST(BpiStatsTest, CalculateMedianGivenAnOddNumberOfPoints) {
    BpiStats stats({
        {monday, 1.0},
        {tuesday, 2.0},
        {wednesday, 3.0},
    });

    auto median = stats.median();
    EXPECT_EQ(2.0, median);
}

TEST(BpiStatsTest, CalculateStandardDeviation) {
    BpiStats stats({
        {monday, 1.0},
        {tuesday, 2.0},
        {wednesday, 3.0},
        {thursday, 4.0},
        {friday, 5.0}
    });

    EXPECT_NEAR(1.4142135623731, stats.standardDeviation(), tolerance);
}