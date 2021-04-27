/**
 * @file NumberWithUnits.cpp
 * @author mor234
 * @brief 
 * @version 0.1
 * @date 2021-04-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "NumberWithUnits.hpp"
#include <fstream>
#include<sstream>
#include <string>



using namespace std;

namespace ariel {
    std::map<std::string, std::map<std::string, double>>  NumberWithUnits::units_convertions;

    NumberWithUnits::NumberWithUnits(double num, string unit) {
        if (!units_convertions.contains(unit))//hasn't found
        {
            throw "Error. The unit type doesn't exist in the class.";
        }
        this->num = num;
        this->unit_type = unit;
    }

    NumberWithUnits::NumberWithUnits(const NumberWithUnits &other) {
        this->num = other.num;
        this->unit_type = other.unit_type;
    }

    /**
     * @brief 
     * 
     * @param unit1 
     * @param unit2 
     * @param unit1_to_unit2 
     */
     void NumberWithUnits::update_units(string unit1, string unit2, double unit1_to_unit2)
     {
        for (auto pair:units_convertions[unit1]) {
            /**
             * @brief 
             * m= 100 cm, m=0.001 km  ->   km= 100/0.001 cm 
             * unit 1= unit1_to_unit2 unit2, unit1=pair.second pair.first ->
             * -> units_convertions[pair.first][unit2]=unit1_to_unit2/pair.second
             * 
             */
            units_convertions[pair.first][unit2] = unit1_to_unit2 / pair.second;
            units_convertions[unit2][pair.first] = pair.second/unit1_to_unit2;
        }
    }

    void NumberWithUnits::read_units(ifstream &units_file) {

        if (!units_file) {
            throw "error opening input file\n";
        }
        int one;
        string unit1;
        string equal_sign;
        double number;
        string unit2;
        while (!units_file.eof()) {
            units_file >> one >> unit1 >> equal_sign >> number >> unit2;
            //number not suppose to be zero

            update_units(unit1, unit2, number);
            update_units(unit2, unit1, 1/number);

            units_convertions[unit1][unit2] = number;
            units_convertions[unit2][unit1] = 1 / number;


        }
        units_file.close();//need to?
        for (map < string, map < string, double >> ::iterator it = units_convertions.begin(); it !=
                                                                                              units_convertions.end();
        ++it) {
            cout << it->first<<":"<<endl;
            map<string, double> internal_map = it->second;
            for (map<string, double>::iterator it2 = internal_map.begin(); it2 != internal_map.end(); ++it2) {
                cout << " <" << it2->first << " , " << it2->second << " > " ;
            }
            cout<<endl;
        }


    }
    //-------------------------------------
    // operators
    //-------------------------------------

    //----------------------------------
    // unary operators
    //----------------------------------

    //Addition
    NumberWithUnits NumberWithUnits::operator+() const {
        return *this;
    }

    // prefix increment:
    NumberWithUnits &NumberWithUnits::operator++() {
        (this->num) += 1;
        //return NumberWithUnits(*this-1)
        return *this;

    }

    // postfix increment:
    NumberWithUnits NumberWithUnits::operator++(int dummy_flag_for_postfix) {
        (this->num)++;
        //return NumberWithUnits(*this-1)
        return NumberWithUnits((this->num) - 1, this->unit_type);
    }


    //Minus
    NumberWithUnits NumberWithUnits::operator-() const {
        return NumberWithUnits(-(this->num), this->unit_type);
    }

    // prefix decrement:
    NumberWithUnits &NumberWithUnits::operator--() {
        this->num -= 1;
        return *this;
    }

    // postfix decrement:
    NumberWithUnits NumberWithUnits::operator--(int dummy_flag_for_postfix) {
        this->num -= 1;
        return NumberWithUnits((this->num) + 1, this->unit_type);
    }
    //help functions
    /**
     * @brief 
     * 
     * @param otherNum 
     * @return double 
     */
    double NumberWithUnits::convert_to_this_unit(const NumberWithUnits &otherNum) const {
        double converted_number;
        if (this->unit_type==otherNum.unit_type)
        {
            converted_number=1;
        }
        else{
            converted_number=units_convertions[otherNum.unit_type][this->unit_type];
        }
        return otherNum.num*converted_number;
    }

    /**
     * @brief 
     * 
     * @param otherNum 
     * @return true 
     * @return false 
     */
    bool NumberWithUnits::is_convertable(const NumberWithUnits &otherNum) const {
        if(this->unit_type==otherNum.unit_type)
        {
            return true;
        }
        return (units_convertions[this->unit_type]).contains(otherNum.unit_type);
    }

    //----------------------------------------
    // binary operators
    //----------------------------------------

    //Addition
    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &otherNum) const {
        if (!(is_convertable(otherNum))) {
            throw "Error. The units types are from different dimensions and can't be added.";
        }
        double ans_num = convert_to_this_unit(otherNum) + this->num;
        return NumberWithUnits(ans_num, this->unit_type);
    }

    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &otherNum) {
        NumberWithUnits tmp_ans = *this + otherNum;
        //continue to this line only if has throwen error
        this->num = tmp_ans.num;
        return *this;
    }

    //Subtraction
    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &otherNum) const {
        if (!(is_convertable(otherNum))) {
            throw "Error. The units types are from different dimensions and can't be substruced.";
        }
        double ans_num =this->num  - convert_to_this_unit(otherNum);
        return NumberWithUnits(ans_num, this->unit_type);
    }

    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &otherNum) {
        NumberWithUnits tmp_ans = *this - otherNum;
        //continue to this line only if has throwen error
        this->num = tmp_ans.num;
        return *this;
    }

    //multiplication
    NumberWithUnits NumberWithUnits::operator*(const double &otherDoubleNum) const {
        return NumberWithUnits(otherDoubleNum * this->num, this->unit_type);

    }

    NumberWithUnits operator*(const double &otherDoubleNum, const NumberWithUnits &otherNum) {
        return otherNum * otherDoubleNum;
    }


    //Comparison operators
    bool NumberWithUnits::operator<(const NumberWithUnits &otherNum) const {
        if (!is_convertable(otherNum)) {
            throw "Error. The units types are from different dimensions and can't be compared.";
        }
        return (this->num) < convert_to_this_unit(otherNum);
    }

    bool NumberWithUnits::operator>(const NumberWithUnits &otherNum) const {
        return  otherNum<(*this);
    }

    bool NumberWithUnits::operator<=(const NumberWithUnits &otherNum) const {
        return !((*this) > otherNum);
    }

    bool NumberWithUnits::operator>=(const NumberWithUnits &otherNum) const {
        return !(*this < otherNum);
    }

    bool NumberWithUnits::operator==(const NumberWithUnits &otherNum) const {
        if (!is_convertable(otherNum)) {
            return false;
        }

        return abs(this->num-convert_to_this_unit(otherNum))<=EPS;
    }

    bool NumberWithUnits::operator!=(const NumberWithUnits &otherNum) const {
        return !(*this == otherNum);
    }


    //----------------------------------
    // friend global IO operators
    //----------------------------------
    std::ostream &operator<<(std::ostream &output, const NumberWithUnits &num) {
        output << num.num << '[' << num.unit_type << ']';
        return output;
    }

    std::istream &operator>>(std::istream &input, NumberWithUnits &num)//
    {
        const char LEFT_BRACKET = '[';
        const char RIGHT_BRACKET = ']';
        char left_bracket_input, right_bracket_input;
        double input_num;
        string input_unit;

        input >> input_num >>input_unit;

        istringstream iss(string);     //object 'iss' created
        string str;
        iss >> str;       
        
        // if (iss)
        // {
        //     throw "Error. Wrong input format.";
        // }
        if (left_bracket_input != LEFT_BRACKET || right_bracket_input != RIGHT_BRACKET) {
            throw "Error. Wrong input format.";
        }
        if (!(NumberWithUnits::units_convertions.contains(input_unit))) {
            throw "Error. The given unit type does not exist. ";
        }
        return input;
    }
}