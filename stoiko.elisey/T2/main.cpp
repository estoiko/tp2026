// var 5
// ULL OCT CMP LSP

#include <cctype>
#include <sstream>
#include <string>
#include <complex>

// (:key1 #c(1.0 -1.0):key2 (:keyX 076:):key3 "data":)
// (:key2 (:keyX 01001:):key3 "with : inside":key1 #c(2.0 -3.0):)
struct DataStruct {
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
};

struct DelimiterIO {
    char exp;
};

struct OctLiteralIO {
    unsigned long long& ref;
};

struct ComplexIO {
    std::complex<double>& ref;
};

struct StringIO {
    std::string& ref;
};

struct LabelIO {
    std::string exp;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, OctLiteralIO&& dest);
std::istream& operator>>(std::istream& in, ComplexIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, LabelIO&& dest);

std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    char c = '0';
    in >> c;

    if (in && (c != dest.exp)) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& operator>>(std::istream& in, OctLiteralIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    unsigned long long num;

    in >> DelimiterIO{'0'} >> num;
    if (!in) {
        return in;
    }

    std::istringstream str_view(std::to_string(num));

    char digit;
    while (str_view >> digit) {
        if (((digit - '0') < 0) || ((digit - '0') > 9)) {
            in.setstate(std::ios::failbit);
            return in;
        }
    }

    dest.ref = num;

    return in;
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    double re;
    double im;

    in >> DelimiterIO{'#'} >> DelimiterIO{'c'} >> DelimiterIO{'('} >> re;
    if (!in) {
        return in;
    }

    if (!std::isspace(in.peek())) {
        in.setstate(std::ios::failbit);
        return in;
    }
    in.get();

    in >> im >> DelimiterIO{')'};
    if (in) {
        dest.ref = std::complex<double>(re, im);
    }

    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    std::string data = "";
    if ((in >> StringIO{data}) && (data != dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}
