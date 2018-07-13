#include "bpistats.h"
#include "nlohmann/json.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <regex>
#include <sstream>

using json = nlohmann::json;

#ifdef __APPLE__
    #include <experimental/optional>
#else
    #include <optional>
#endif

namespace bpistats {
    template <class T>
    #ifdef __APPLE__
        using optional = std::experimental::optional<T>;
    #else
        using optional = std::optional<T>;
    #endif

    constexpr double tolerance = 0.0001;

    bool Date::operator<(const Date& other) const {
        if (year != other.year) {
            return year < other.year;
        } else if (month != other.month) {
            return month < other.month;
        } else if (day != other.day) {
            return day < other.day;
        } else {
            return false;
        }
    }

    bool Date::operator==(const Date& other) const {
        return year  == other.year &&
               month == other.month &&
               day   == other.day;
    }

    std::ostream& operator<<(std::ostream& os, const Date& date) {
        os << date.year
        << "-" << std::setfill('0') << std::setw(2) << date.month
        << "-" << std::setfill('0') << std::setw(2) << date.day;

        return os;
    }

    optional<Date> parseDate(const std::string& raw) {
        std::regex regex(R"((\d{4})-(\d{2})-(\d{2}))");
        std::cmatch match;
        if (std::regex_match(raw.c_str(), match, regex)) {
            try {
                int year = std::stoi(match.str(1));
                unsigned month = std::stoi(match.str(2));
                unsigned day = std::stoi(match.str(3));

                return Date{year, month, day};
            }
            catch (std::invalid_argument) {
                std::cerr << "Warning: trouble parsing date." << std::endl;
                return {};
            }
        } else {
            return {};
        }
    }

    /**
     * TODO
     *   - Create a general loading abstraction.
     *   - Create a JSON file loader and a CURL-based loader.
     *   - Handle malformed JSON.
     */
    PriceByDay load(const std::string& filename) {
        std::ifstream file(filename);
        std::stringstream buffer;
        PriceByDay priceByDay{};

        if (file.is_open()) {
            json j;
            file >> j;

            auto const& bpi = j["bpi"];

            for(auto it = bpi.cbegin(); it != bpi.cend(); ++it) {
                auto maybeDate = parseDate(it.key());
                if (maybeDate) {
                    priceByDay.emplace(*maybeDate, it.value());
                }
            }
        } else {
            throw std::runtime_error("Could not open the file.");
        }

        return priceByDay;
    }

    std::ostream& operator<<(std::ostream& os, const PriceAndDate& pAndD) {
        os << "[" << pAndD.date << "] " << pAndD.price;

        return os;
    }

    bool PriceAndDate::operator==(const PriceAndDate& other) const {
        return date  == other.date &&
               abs(price - other.price) < tolerance;
    }

    BpiStats::BpiStats(PriceByDay priceByDay): priceByDay_(std::move(priceByDay)) {};

    unsigned BpiStats::numPoints() const {
        return priceByDay_.size();
    }

    bool BpiStats::isValid() const {
        return numPoints() > 0;
    }

    void BpiStats::preventOperationOnEmptyTimeSpan() const {
        if (priceByDay_.empty()) {
            throw EmptyTimeSpan();
        }
    }

    std::pair<PriceAndDate, PriceAndDate> BpiStats::getLowAndHigh() const {
        preventOperationOnEmptyTimeSpan();
        
        auto comparator = [](const auto &a, const auto b) {
            return a.second < b.second;
        };

        auto [itLow, itHigh] =  std::minmax_element(priceByDay_.cbegin(),
                                                    priceByDay_.cend(),
                                                    comparator);

        PriceAndDate low{itLow->second, itLow->first};
        PriceAndDate high{itHigh->second, itHigh->first};

        return std::make_pair(low, high);
    }

    double BpiStats::average() const {
        auto num = numPoints();

        preventOperationOnEmptyTimeSpan();

        auto sum = std::accumulate(
            priceByDay_.cbegin(),
            priceByDay_.cend(),
            0.0,
            [](double acc, const auto& priceByDay) {
                return acc + priceByDay.second;
            });
        
        return sum / num;
    }

    double BpiStats::median() const {
        preventOperationOnEmptyTimeSpan();

        std::vector<PriceT> prices{};
        std::transform(
            priceByDay_.cbegin(),
            priceByDay_.cend(),
            std::back_inserter(prices),
            [](const auto& pByD) { return pByD.second; });
        
        std::sort(prices.begin(), prices.end());

        auto n = numPoints();
        if (n % 2 == 0) {
            return (prices[n /2 -1] + prices[n/2]) / 2.0;
        } else {
            return prices[n/2];
        }
    }

    double BpiStats::standardDeviation() const {
        preventOperationOnEmptyTimeSpan();

        double num = numPoints();
        auto avg = average();

        auto sum = std::accumulate(
            priceByDay_.cbegin(),
            priceByDay_.cend(),
            0.0,
            [avg](double acc, const auto& priceByDay) {
                auto var = (avg - priceByDay.second);

                return acc + var * var;
            });
        
        return std::sqrt(sum / num);
    }
}
