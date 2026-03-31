#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <limits>

namespace nspace {
    struct DataStruct {
        double key1 = 0;
        unsigned long long key2 = 0;
        std::string key3;
    };

    struct DelimiterIO { char exp; };
    struct KeyIO { std::string& ref; };
    struct DoubleIO { double& ref; };
    struct BinUllIO { unsigned long long& ref; };
    struct StringIO { std::string& ref; };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, KeyIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleIO&& dest);
    std::istream& operator>>(std::istream& in, BinUllIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

    void printBinary(std::ostream& out, unsigned long long n);
    bool compare_data(const DataStruct& a, const DataStruct& b);

    class iofmtguard {
    public:
        iofmtguard(std::basic_ios<char>& s) : s_(s), fmt_(s.flags()), prec_(s.precision()) {}
        ~iofmtguard() { s_.flags(fmt_); s_.precision(prec_); }
    private:
        std::basic_ios<char>& s_;
        std::ios_base::fmtflags fmt_;
        std::streamsize prec_;
    };
}

int main() {
    using nspace::DataStruct;

    std::vector<DataStruct> vec;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
    std::istream_iterator<DataStruct>(),
    std::back_inserter(vec)
    );

    std::sort(vec.begin(), vec.end(), nspace::compare_data);

    std::cout << "Found " << vec.size() << " elements:\n";
    for (const auto& x : vec) {
        std::cout << x << "\n";
    }

    return 0;
}

namespace nspace {
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        char c;
        if ((in >> c) && (c != dest.exp)) in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, KeyIO&& dest) {
        dest.ref.clear();
        char c;
        for (int i = 0; i < 4; ++i) {
            if (in >> c) dest.ref += c;
        }

        if (in.get() != ' ') in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& dest) {
        in >> dest.ref;
        return in;
    }

    std::istream& operator>>(std::istream& in, BinUllIO&& dest) {
        char c1, c2;
        if (!(in >> c1 >> c2) || c1 != '0' || c2 != 'b') {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::string bits;
        while (std::isdigit(in.peek())) {
            bits += static_cast<char>(in.get());
        }
        if (bits.empty()) in.setstate(std::ios::failbit);

        else dest.ref = std::stoull(bits, nullptr, 2);
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        char c;
        if (!(in >> c) || c != '"') {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::getline(in, dest.ref, '"');
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct temp;
        in >> DelimiterIO{'('} >> DelimiterIO{':'};

        for (int i = 0; i < 3; ++i) {
            std::string key;
            in >> KeyIO{key};

            if (key == "key1") in >> DoubleIO{temp.key1};
            else if (key == "key2") in >> BinUllIO{temp.key2};
            else if (key == "key3") in >> StringIO{temp.key3};
            else in.setstate(std::ios::failbit);

            in >> DelimiterIO{':'};
        }

        in >> DelimiterIO{')'};

        if (in) dest = temp;
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        iofmtguard guard(out);
        out << "(:key1 "
        << std::scientific
        << std::uppercase
        << std::setprecision(1)
        << src.key1;
        out << ":key2 ";
        printBinary(out, src.key2);
        out << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    void printBinary(std::ostream& out, unsigned long long n) {
        if (n == 0) { out << "0b0"; return; }
        std::string s = "";
        unsigned long long temp = n;
        while (temp > 0) {
            s = (temp % 2 == 0 ? "0" : "1") + s;
            temp /= 2;
        }
        out << "0b" << s;
    }

    bool compare_data(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
    }
}
