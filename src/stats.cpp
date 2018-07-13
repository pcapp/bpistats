#include <iostream>

#include "bpistats.h"

void report(const std::string& filename, const bpistats::BpiStats& stats) {
    auto lowAndHigh = stats.getLowAndHigh();

    std::cout << "# of days: " << stats.numPoints() << std::endl;
    std::cout << "Average value: " << stats.average() << std::endl;
    std::cout << "Median value: " << stats.median() << std::endl;
    std::cout << "Std. Deviation: " << stats.standardDeviation() << std::endl;
    std::cout << "Lowest  day and value: " << lowAndHigh.first << std::endl;
    std::cout << "Highest day and value: " << lowAndHigh.second << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: stats <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "--help") == 0) {
        std::cout << "Usage: stats <filename>" << std::endl;
        return EXIT_SUCCESS;
    }

    try {
        std::string filename {argv[1]};
        bpistats::PriceByDay priceByDay = bpistats::load(filename);
        bpistats::BpiStats stats{priceByDay};

        if (stats.numPoints() == 0) {
            std::cout << "No data points found." << std::endl;
            return EXIT_SUCCESS;
        }

        report(filename, stats);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    
    return EXIT_SUCCESS;
}