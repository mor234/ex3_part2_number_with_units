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
#include <bits/stdc++.h>
#include <stdexcept>





using namespace std;

namespace ariel {
    std::map<std::string, std::map<std::string, double>>  NumberWithUnits::units_convertions;

    NumberWithUnits::NumberWithUnits(const double & num, const string & unit) {
        if (!units_convertions.contains(unit))//hasn't found
        {
            throw std::"Error. The unit type doesn't exist in the class.";
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
     void NumberWithUnits::update_units(const string & unit1, const string & unit2, double unit1_to_unit2)
     {
        for (auto const & pair:units_convertions[unit1]) {
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
    //got help from a friend
    void  NumberWithUnits::print_units() const
    {
        for (auto it = units_convertions.begin(); it != units_convertions.end();++it) 
        {
            cout << it->first<<":"<<endl;
            auto inside_map=it->second;
            for (auto it2 =  inside_map.begin(); it2 != inside_map.end(); ++it2) {
                cout << " <" << it2->first << " , " << it2->second << "> " ;
            }
            cout<<endl;
        }
    }

    void NumberWithUnits::read_units(ifstream &units_file) {

        if (!units_file) {
            throw "error opening input file\n";
        }
        int one=0;
        string unit1;
        string equal_sign;
        double number=0;
        string unit2;
        while (!units_file.eof()) {
            units_file >> one >> unit1 >> equal_sign >> number >> unit2;
            //number not suppose to be zero
            if(number==0)
            {
                throw "Error. The number is not suppose to be zero";
            }
            update_units(unit1, unit2, number);
            update_units(unit2, unit1, 1/number);

            units_convertions[unit1][unit2] = number;
            units_convertions[unit2][unit1] = 1 / number;


        }
        units_file.close();
 
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
        double converted_number=1;
        if (!(this->unit_type==otherNum.unit_type))
        {
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
    bool NumberWithUnits::operator==(const NumberWithUnits &otherNum) const {
        if (!is_convertable(otherNum)) {
            throw "Error. The units types are from different dimensions and can't be compared.";
        }

        return abs(this->num-convert_to_this_unit(otherNum))<=EPS;
    }

    bool NumberWithUnits::operator!=(const NumberWithUnits &otherNum) const {
        return !(*this == otherNum);
    }
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
        return ((*this) < otherNum)  || ((*this)==otherNum);
    }

    bool NumberWithUnits::operator>=(const NumberWithUnits &otherNum) const {
        return ((*this) > otherNum)  || ((*this)==otherNum);
    }


    //----------------------------------
    // friend global IO operators
    //----------------------------------
    std::ostream &operator<<(std::ostream &output, const NumberWithUnits &num) {
        output << num.num << '[' << num.unit_type << ']';
        return output;
    }
    /**
     * @brief 
     * 
     * @param str 
     * @return true 
     * @return false 
     */
    bool NumberWithUnits::remove_spaces_and_bracket(string & str)
    {
        string::size_type i=0;
        bool first_bracket_left=true;
        //remove spaces from the start and '[' once
        while(i<str.size()&&(str[i]==' '||(first_bracket_left&&(str[i]=='['))))  
        {
            if(first_bracket_left&&(str[i]=='['))
            {
                first_bracket_left=false;
            }
           
            i++;
        }
        str.erase(0,i);
        i=str.size()-1;
        bool first_bracket_right=true;
        // remove spaces from the end and ']' once
        while( i>=0 && (str[i]==' '|| ( first_bracket_right && (str[i]==']') ) ) )  
        {
            if(first_bracket_right&&(str[i]==']'))
            {
                first_bracket_right=false;
            }
            str.pop_back();
            i--;
        }
        return (!first_bracket_left&&!first_bracket_right);//both appeared already  
    }
 
    std::istream &operator>>(std::istream &input, NumberWithUnits &num)//
    {
        string input_unit_1,input_unit_2,input_unit_3;
        string input_unit;
        double input_num=0;

        input>>input_num>>input_unit_1;
        if(input_unit_1[input_unit_1.size()-1]!=']')
        {
            input>>input_unit_2;
            if(input_unit_2[input_unit_2.size()-1]!=']')
            {
                input>>input_unit_3;
            }
        }

        input_unit=input_unit_1+" "+input_unit_2+" "+input_unit_3;

        bool good_bracket_format=NumberWithUnits::remove_spaces_and_bracket(input_unit);
        if(!good_bracket_format) //input one is empty
        {
            throw "Error. Wrong input format.";
        }
        if (!(NumberWithUnits::units_convertions.contains(input_unit))) {
            throw "Error. The given unit type does not exist. ";
        }

        num.num=input_num;
        num.unit_type=input_unit;

        return input;
    }
}