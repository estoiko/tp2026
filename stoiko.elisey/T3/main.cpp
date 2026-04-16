#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std::placeholders;

struct Point {
    int x, y;
};

struct Polygon {
    std::vector<Point> points;
};

std::istream& operator>>(std::istream& in, Point& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    char c;
    int x, y;

    in >> c;
    if (c != '(') {
        in.setstate(std::ios::failbit);
        return in;
    }

    in >> x >> c;
    if (c != ';') {
        in.setstate(std::ios::failbit);
        return in;
    }

    in >> y >> c;
    if (c != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }

    dest = {x, y};
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    Polygon tmp;

    std::size_t number_of_points;
    in >> number_of_points;

    if (!in || number_of_points < 3) {
        in.setstate(std::ios::failbit);
        return in;
    }

    tmp.points.resize(number_of_points);

    std::copy_n(
        std::istream_iterator<Point>(in),
        number_of_points,
        tmp.points.begin()
    );

    if (!in) {
        return in;
    }

    dest = std::move(tmp);
    return in;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "ERROR: No file name provided or incorrect file name\n";
        std::cerr << "Usage: ./main <filename>\n";
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "ERROR: file cannot be open or where is no file with such name\n";
        std::cerr << "Usage: ./main <filename>\n";
        return 1;
    }

    // std::vector<std::string> raw_data {std::istream_iterator<std::string>(file),
    //                                    std::istream_iterator<std::string>()};

    // std::cout << raw_data[0] << "\n";

    std::vector<Polygon> polygons;
    std::string raw;

    while (std::getline(file, raw)) {
        if (raw.empty()) continue;

        std::istringstream iss(raw);

        std::copy(
            std::istream_iterator<Polygon>(iss),
            std::istream_iterator<Polygon>(),
            std::back_inserter(polygons)
        );
    }

    return 0;
}
