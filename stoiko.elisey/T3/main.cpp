#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <fstream>
#include <istream>
#include <numeric>
#include <sstream>
#include <iterator>
#include <cmath>
#include <string>
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

void out_polygons(std::vector<Polygon> &polygons) {
    for (std::size_t i = 0; i < polygons.size(); ++i) {
        std::cout << polygons[i].points.size() << " ";
        for (std::size_t j = 0; j < polygons[i].points.size(); ++j) {
            std::cout << "(" << polygons[i].points[j].x << ";" << polygons[i].points[j].y << ") ";
        }
        std::cout << "\n";
    }
}

using ll = long long;

struct GaussSum {
    const std::vector<Point> &p;
    std::size_t n;
    std::size_t i;

    explicit GaussSum(const std::vector<Point>& p)
        : p(p)
        , n(p.size())
        , i(0)
    {}

    ll operator()(ll acc, const Point &curr) {
        const Point &next = p[(i + 1) % n];

        ll term =
            static_cast<ll>(curr.x) * next.y -
            static_cast<ll>(next.x) * curr.y;

        ++i;
        return acc + term;
    }
};

double area(const Polygon &poly) {
    const std::vector<Point> &p = poly.points;
    ll sum = 0;

    sum = std::accumulate(
        p.begin(),
        p.end(),
        0LL,
        GaussSum(p)
    );

    return std::abs(sum) / 2.0;
}

struct AreaCalculator {
    double operator()(const Polygon& p) const {
        return area(p);
    }
};

struct SumArea {
    double operator()(double acc, const Polygon &p) const {
        return acc + AreaCalculator()(p);
    }
};

struct IsEven {
    bool operator()(const Polygon& p) const {
        return p.points.size() % 2 == 0;
    }
};

struct SumIfEven {
    double operator()(double acc, const Polygon &p) const {
        return IsEven()(p) ? acc + AreaCalculator()(p) : acc;
    }
};

struct SumIfOdd {
    double operator()(double acc, const Polygon &p) const {
        return !(IsEven()(p)) ? acc + AreaCalculator()(p) : acc;
    }
};

struct AreaIfVertexCount {
    std::size_t n;

    explicit AreaIfVertexCount(std::size_t n)
        : n(n)
    {}

    double operator()(double acc, const Polygon& p) const {
        return (p.points.size() == n) ? acc + AreaCalculator()(p) : acc;
    }
};

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

    out_polygons(polygons);

    std::cout << std::fixed << std::setprecision(1);

    std::string command;
    while (std::getline(std::cin, command)) {
        if (command.empty()) continue;

        std::istringstream iss(command);

        std::string main_cmd;
        iss >> main_cmd;
        if (main_cmd == "AREA") {
            std::string sub_cmd;
            iss >> sub_cmd;

            if (iss >> std::ws && !iss.eof()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            if (sub_cmd == "EVEN") {
                double area = std::accumulate(
                    polygons.begin(),
                    polygons.end(),
                    0LL,
                    SumIfEven()
                );

                std::cout << area << "\n";
            } else if (sub_cmd == "ODD") {
                double area = std::accumulate(
                    polygons.begin(),
                    polygons.end(),
                    0LL,
                    SumIfOdd()
                );

                std::cout << area << "\n";
            } else if (sub_cmd == "MEAN") {
                if (polygons.empty()) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }

                double area = std::accumulate(
                    polygons.begin(),
                    polygons.end(),
                    0LL,
                    SumArea()
                );

                std::cout << area / polygons.size() << "\n";
            } else {
                try {
                    std::size_t pos = 0;
                    std::size_t num_of_vertexes = std::stoul(sub_cmd, &pos);

                    if (pos != sub_cmd.size()) {
                        throw std::invalid_argument("ERROR: not pure number");
                    }

                    if (num_of_vertexes <= 0) {
                        throw std::invalid_argument("ERROR: incorrect number of vertexes");
                    }

                    double area = std::accumulate(
                        polygons.begin(),
                        polygons.end(),
                        0.0,
                        AreaIfVertexCount(num_of_vertexes)
                    );

                    std::cout << area << "\n";
                } catch (...) {
                    std::cerr << "<INVALID COMMAND>\n";
                    continue;
                }
            }
        } else if (main_cmd == "MAX") {
            // ...
        }
    }

    return 0;
}
