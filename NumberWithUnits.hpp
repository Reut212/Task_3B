
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <map>
#include <cstring>

using namespace std;

namespace ariel{

    static map<pair <string, string>, double>unitsMap;
    static const double epsilon = 0.001;

    class NumberWithUnits{
    private:

        double number;
        std::string unit;


    public:
        NumberWithUnits(double num, const string& newUnit);

        friend double ratioUnits(const string aUnit, const string bUnit, double number);

        static void read_units(ifstream& file);

        static bool checkUnit(const string &getunit);

        friend ostream& operator<<(ostream& out, const NumberWithUnits& num);
        friend istream& operator>>(istream& in, NumberWithUnits& num);

        NumberWithUnits operator-(const NumberWithUnits& b)const;
        NumberWithUnits operator-()const;

        NumberWithUnits operator+(const NumberWithUnits& a)const;
        NumberWithUnits operator+() const;

        friend NumberWithUnits operator* (const NumberWithUnits& a, double num);
        friend NumberWithUnits operator* (double num,const NumberWithUnits& a);
        NumberWithUnits& operator*=(double x);

        friend bool operator>(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator>=(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator<(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator<=(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator==(const NumberWithUnits& a, const NumberWithUnits& b);
        friend bool operator!=(const NumberWithUnits& a, const NumberWithUnits& b);

        NumberWithUnits& operator-=(const NumberWithUnits& b);
        NumberWithUnits& operator+=(const NumberWithUnits& b);
        NumberWithUnits& operator++();
        NumberWithUnits operator++(int);
        NumberWithUnits& operator--();
        NumberWithUnits operator--(int);
    };
}
