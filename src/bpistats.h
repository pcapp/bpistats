#ifndef BPISTATS_H
#define BPISTATS_H

#include <map>

namespace bpistats {
    struct Date;
    using PriceT = double;
    using PriceByDay = std::map<Date, PriceT>;

    PriceByDay load(const std::string& filename);

    struct Date {
        int year{};
        unsigned month{};
        unsigned day{};

        bool operator<(const Date& other) const;
        bool operator==(const Date& other) const;
    };

    std::ostream& operator<<(std::ostream& os, const Date& date);
    
    struct PriceAndDate {
        PriceT price;
        Date date{};

        bool operator==(const PriceAndDate& other) const;
    };

    std::ostream& operator<<(std::ostream& os, const PriceAndDate& pAndD);

    class EmptyTimeSpan : public std::runtime_error {
    public:
        EmptyTimeSpan(): std::runtime_error("Trying to call a function requiring at least one data point on an empty time span.") {}
    };

    class BpiStats {
    public:
        explicit BpiStats(PriceByDay priceByDay);

        /**
         * Return whether or not the prices-by-day data structure
         * has a non-empty range. Statistical functions like
         * the average and standard deviation functions rely on a non-zero number of data
         * points. These functions both divide by the number of data points,
         * so a zero will result in an error.
         * 
         * @return 
         */
        bool isValid() const;

        /**
         * Return the number of data points stored in the internal data set.
         * 
         * @return The number of data points stored in the internal data set.
         */
        unsigned numPoints() const;

        /**
         * Return the day and value of the data points with the highest and 
         * lowest closing value.
         * 
         * @return A tuple where the first entry represents the day with the lowest
         * closing value. The second entry represents the day with the highest
         * closing value.
         */
        std::pair<PriceAndDate, PriceAndDate> getLowAndHigh() const;

        /**
         * Get the average bitcoin closing value over the stored time span.
         * 
         * @return the average bitcoin price over the stored time span.
         */
        double average() const;

        /**
         * Get the median bitcoin closing value over the stored time span.
         * 
         * @return The median bitcoin closing value over the stored time span.
         */
        double median() const;

        /**
         * Get the standard deviation of the closing values in the stored time span.
         * 
         * @return The standard deviation of the closing values in the stored time span.
         */
        double standardDeviation() const;
    private:
        void preventOperationOnEmptyTimeSpan() const;
    private:
        PriceByDay priceByDay_;
    };
}

#endif