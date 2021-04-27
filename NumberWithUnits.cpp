
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <map>
#include <cstring>
#include <cmath>
#include "NumberWithUnits.hpp"

using namespace std;

namespace ariel{
    double ratioUnits(const string &aUnit, const string &bUnit, double number){
        double value =0;
        bool flag = false;
        if(aUnit == bUnit){
            return number;
        }
        map<pair<std::string,std::string>,double>::iterator itr;
        for(itr=ariel::unitsMap.begin(); itr!= ariel::unitsMap.end(); itr++){
            pair<const string,const string> key = itr->first;

            if(key.first==aUnit && key.second==bUnit){
                value = itr -> second;
                flag = true;
                break;
            }
        }
        if(!flag){
                throw invalid_argument {"According to this file, there is no way to convert those two units."};
        }
        return value*number;
    }

    NumberWithUnits::NumberWithUnits(double num, const string& newUnit){
        
        string unvalideUnit = " ";
        bool keyWasFound = false;
        map<pair<string,string>,double>::iterator itr;

        for(itr=unitsMap.begin(); itr!= unitsMap.end(); itr++){
            pair<string,string> key = itr->first;

            if(key.first==newUnit || key.second==newUnit){
                keyWasFound = true;
            }
        }
        if(!keyWasFound){
            throw invalid_argument("Invalid Unit: There is no such unit in this file");
        }
        
        this -> number = num;
        this -> unit = newUnit;
    }



    void NumberWithUnits::read_units(ifstream& file){
        if(file.is_open()){
            double firstNum;
            double lastNum;
            string firstUnit;
            string lastUnit;
            int unvalidNum = 0;

            string x;
            while (file >> firstNum >> firstUnit >> x >> lastNum >> lastUnit && x == "="){
                if( firstNum !=0 && lastNum != 0){
                    unitsMap[{firstUnit, lastUnit}] = lastNum;
                    unitsMap[{lastUnit, firstUnit}] = 1/lastNum;
                }else{
                    throw invalid_argument("Invalid Unit: Zero can not be used as a criterion!");
                }
            }
            // for (auto& t : unitsMap)
            //         cout << "(" << t.first.first << "," << t.first.second << ") = Ratio: " << t.second << "\n";
        }
        file.close();
        map < pair < string, string >, double > ::iterator it1;
        map < pair < string, string >, double > ::iterator it2;

        for (it1 = unitsMap.begin(); it1 != unitsMap.end(); it1++) {
            for (it2 = unitsMap.begin(); it2 != unitsMap.end(); it2++) {
                string from1 = it1->first.first;
                string to1 = it1->first.second;
                string from2 = it2->first.first;
                string to2 = it2->first.second;

                if (to1 == from2 && from1 != from2) {
                    unitsMap[pair<string, string>(from1, to2)] = unitsMap[it2->first] * unitsMap[it1->first];
                    unitsMap[pair<string, string>(to2, from1)] = 1 / (unitsMap[it2->first] * unitsMap[it1->first]);
                }
            }
        }
    }
 
    bool NumberWithUnits::checkUnit(const string &getunit) {
        return unitsMap.count(pair<string, string>(getunit, getunit)) != 0;
    }

    ostream& operator<<(ostream& out, const NumberWithUnits& NumberWithUnits){
        out << NumberWithUnits.number << "[" << NumberWithUnits.unit << "]" ;
        return out;
    }
    NumberWithUnits NumberWithUnits::operator-()const{
        return NumberWithUnits(- this -> number,this -> unit);
    }
    NumberWithUnits NumberWithUnits::operator+() const{
        return NumberWithUnits(this -> number,this -> unit);
    }
    NumberWithUnits operator* (double num,const NumberWithUnits& a){
        return NumberWithUnits(num*a.number, a.unit);
    }
    NumberWithUnits operator* (const NumberWithUnits& a, double num){
        return NumberWithUnits(num*a.number, a.unit);
    }
    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& a)const{
        double ratioAB= ratioUnits(a.unit, this->unit, a.number);
        return NumberWithUnits(this->number+ratioAB, this->unit);
    }
    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& b)const{
        double ratioAB= ratioUnits(b.unit, this->unit, b.number);
        return NumberWithUnits(this -> number-ratioAB, this -> unit);
    }
    bool operator>(const NumberWithUnits& a, const NumberWithUnits& b){
        double ratioAB= ratioUnits(b.unit, a.unit, b.number);
        return a.number-ratioAB >= epsilon;
    }
    bool operator>=(const NumberWithUnits& a, const NumberWithUnits& b){
        return ((a == b) || (a > b));
    }
    bool operator<(const NumberWithUnits& a, const NumberWithUnits& b){
        double ratioAB= ratioUnits(b.unit, a.unit, b.number);
        return ratioAB-a.number >= epsilon;
    }
    bool operator<=(const NumberWithUnits& a, const NumberWithUnits& b){
        return ((a == b) || (a < b));
    }
    bool operator==(const NumberWithUnits& a, const NumberWithUnits& b){
        double ratioAB= ratioUnits(b.unit,a.unit,b.number);
        return abs(ratioAB - a.number) < epsilon;
    }
    bool operator!=(const NumberWithUnits& a, const NumberWithUnits& b){
        return (!(a == b));
    }
    NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& b){
        double ratioAB= ratioUnits(b.unit, this -> unit, b.number);
        this -> number -= ratioAB;
        return * this;
    }
    NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& b){
        double ratioAB= ratioUnits(b.unit, this -> unit, b.number);
        this -> number += ratioAB;
        return *this;
    }
    NumberWithUnits& NumberWithUnits::operator++(){
        ++number;
        return *this;
    }
    NumberWithUnits& NumberWithUnits::operator--(){
        --number;
        return *this;
    }
    NumberWithUnits NumberWithUnits::operator--(int){
        NumberWithUnits temp{number, unit};
        number--;
        return temp;
    }
    NumberWithUnits NumberWithUnits::operator++(int){
        NumberWithUnits temp{number, unit};
        number++;
        return temp;
    }
    istream& operator>> (istream& in, NumberWithUnits& num){
        string str;
        getline(in, str, ']');

        string number;
        string unitIn;
        unsigned int i = 0;

        while (' ' == str[i]){
            i++;
        }

        while (('0' <= str[i] && str[i] <= '9') || '+' == str[i] || '-' == str[i] || '.' == str[i]){
            number += str[i++];
        }

        while (' ' == str[i] || str[i] == '['){
            i++;
        }

        while ('A' <= str[i] && str[i] <= 'z'){
            unitIn += str[i++];
        }

        if (!NumberWithUnits::checkUnit(unitIn)) {
            throw std::invalid_argument(unitIn + " is invalid unit");
        }
        num.number = stod(number);
        num.unit = unitIn;
        num = NumberWithUnits(num.number, num.unit);
        return in;
    }



}