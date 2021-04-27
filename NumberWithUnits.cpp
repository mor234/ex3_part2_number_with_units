/**
 * @file NumberWithUnits.cpp
 * @author mor234
 * @brief implementation of the class NumberWithUnits
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
    //hold the units and the conventions between them
    std::map <std::string, std::map<std::string, double>>  NumberWithUnits::units_convertions;

    /**
     * @brief Construct a new Number With Units object
     * 
     * @param num - The value of the number
     * @param unit 
     */
    NumberWithUnits::NumberWithUnits(const double &num, const string &unit) {
        if (!units_convertions.contains(unit)) {
            throw std::invalid_argument{"Error. The unit type doesn't exist in the class."};

        }
        this->num = num;
        this->unit_type = unit;
    }

    /**
     * @brief Copy constructor:
     * construct a new Number With Units object
     * 
     * @param other Number With Units
     */
    NumberWithUnits::NumberWithUnits(const NumberWithUnits &other) {
        this->num = other.num;
        this->unit_type = other.unit_type;
    }

    /**
     * @brief 
     * Auxiliary function for the read_units function:
     * A function that receives two units and the conversion between them
     * and updates the units_convertions map so:
     * -each unit that can be converted for the second unit will receive the conversion to the second unit,
     * -each such unit will be added to the units that can be converted for the second unit. 
     * 
     * @param unit1 
     * @param unit2 
     * @param unit1_to_unit2 -conversion between unit1 and unit2
     */
    void NumberWithUnits::update_units(const string &unit1, const string &unit2, double unit1_to_unit2) {
        for (auto const &pair:units_convertions[unit1]) {
            /**
             * @brief 
             * m= 100 cm, m=0.001 km  ->   km= 100/0.001 cm 
             * unit 1= unit1_to_unit2 unit2, unit1=pair.second pair.first ->
             * -> units_conventions[pair.first][unit2]=unit1_to_unit2/pair.second
             * 
             */
            units_convertions[pair.first][unit2] = unit1_to_unit2 / pair.second;
            units_convertions[unit2][pair.first] = pair.second / unit1_to_unit2;
        }
    }

    /**
     * @brief 
     * Prints the data structure of the unit conversion- units_convertions 
     */
    void NumberWithUnits::print_units() {
        for (auto it = units_convertions.begin(); it != units_convertions.end(); ++it) {
            cout <<"    "<< it->first << ":" << endl<<"     ";
            auto inside_map = it->second;
            for (auto it2 = inside_map.begin(); it2 != inside_map.end(); ++it2) {
                cout << " (" << it2->first << " , " << it2->second << ") ";
            }
            cout << endl;
        }
    }

    /**
     * @brief Gets ifstream of a unit conversion file
     *  and inserts the unit conversion data into the data structure- units_convertions
     * @param units_file: ifstream of a unit conversion file 
     */
    void NumberWithUnits::read_units(ifstream &units_file) {

        if (!units_file) {
            throw std::invalid_argument{"error opening input file\n"};
        }
        int one = 0;
        string unit1;
        string equal_sign;
        double number = 0;
        string unit2;
        while (!units_file.eof()) {
            units_file >> one >> unit1 >> equal_sign >> number >> unit2;//read a line
            //number not suppose to be zero- can't divide by 0!
            if (number == 0) {
                throw std::invalid_argument{"Error. The number is not suppose to be zero"};
            }
            /*
             * Adds the convertible units associated with one of the units to the second unit,
             *  and adds the second unit to the convertible units  
             */
            update_units(unit1, unit2, number);
            update_units(unit2, unit1, 1 / number);

            //add to each other
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

    /**
     * @brief 
     * Unary plus
     * @return NumberWithUnits 
     */
    NumberWithUnits NumberWithUnits::operator+() const {
        return *this;
    }

    /**
     * @brief 
     * Prefix increment
     * @return NumberWithUnits& -reference to the object after the change
     */
    NumberWithUnits &NumberWithUnits::operator++() {
        (this->num) += 1;
        return *this;
    }

    /**
     * @brief 
     * Postfix increment:
     * @param dummy_flag_for_postfix
     * @return NumberWithUnits 
     */
    NumberWithUnits NumberWithUnits::operator++(int dummy_flag_for_postfix) {
        (this->num)++;
        return NumberWithUnits((this->num) - 1, this->unit_type);
    }

    /**
     * @brief 
     * Unary minus
     * @return NumberWithUnits with negative value
     */
    NumberWithUnits NumberWithUnits::operator-() const {
        return NumberWithUnits(-(this->num), this->unit_type);
    }

    /**
     * @brief 
     * Prefix decrement 
     * @return NumberWithUnits& -reference to the object after the change
     */
    NumberWithUnits &NumberWithUnits::operator--() {
        this->num -= 1;
        return *this;
    }

    /**
     * @brief 
     *  Postfix decrement
     */
    NumberWithUnits NumberWithUnits::operator--(int dummy_flag_for_postfix) {
        this->num -= 1;
        return NumberWithUnits((this->num) + 1, this->unit_type);
    }

    /**
     * @brief
     * Receives NumberWithUnits and calculates and return its number value
     *  after converting to the unit of the object that called the function (* this)  
     * 
     * @param otherNum: NumberWithUnits
     * @return double  
     */
    double NumberWithUnits::convert_to_this_unit(const NumberWithUnits &otherNum) const {
        double converted_number = 1;
        if (!(this->unit_type == otherNum.unit_type)) {
            converted_number = units_convertions[otherNum.unit_type][this->unit_type];
        }
        return otherNum.num * converted_number;
    }

    /**
     * @brief 
     * Check if the given NumberWithUnits can be converted to the uit type of the calling object
     * @param otherNum: NumberWithUnits
     * @return true if otherNum can be converted to the unit type belong to the calling object (*this)
     * @return false if not
     */
    bool NumberWithUnits::is_convertable(const NumberWithUnits &otherNum) const {
        if (this->unit_type == otherNum.unit_type) {
            return true;
        }
        return (units_convertions[this->unit_type]).contains(otherNum.unit_type);
    }

    //----------------------------------------
    // binary operators
    //----------------------------------------

    /**
     * @brief 
     * Adding two NumberWithUnits. if they can't be converted to each other- throws an excaption.
     * @param otherNum 
     * @return NumberWithUnits 
     */
    NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &otherNum) const {
        if (!(is_convertable(otherNum))) {
            throw std::logic_error{"Error. The units types are from different dimensions and can't be added."};
        }
        double ans_num = convert_to_this_unit(otherNum) + this->num;
        return NumberWithUnits(ans_num, this->unit_type);
    }

    /**
     * @brief 
     * += operator. 
     * Adding two NumberWithUnits. if they can't be converted to each other- throws an excaption.
     * @param otherNum 
     * @return NumberWithUnits& - reference to the calling object
     */
    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &otherNum) {
        NumberWithUnits tmp_ans = *this + otherNum;
        //continue to this line only if hasn't throwen error
        this->num = tmp_ans.num;
        return *this;
    }

    /**
     * @brief Subtraction
     * Substructing two NumberWithUnits. if they can't be converted to each other- throws an excaption.
     *  
     * @param otherNum 
     * @return NumberWithUnits 
     */
    NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &otherNum) const {
        if (!(is_convertable(otherNum))) {
            throw std::logic_error{"Error. The units types are from different dimensions and can't be substruced."};
        }
        double ans_num = this->num - convert_to_this_unit(otherNum);
        return NumberWithUnits(ans_num, this->unit_type);
    }

    /**
     * @brief -= operator
     *  Substructing two NumberWithUnits. if they can't be converted to each other- throws an excaption.
     *  
     * 
     * @param otherNum 
     * @return NumberWithUnits& - reference to the calling object
     */
    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &otherNum) {
        NumberWithUnits tmp_ans = *this - otherNum;
        //continue to this line only if has throwen error
        this->num = tmp_ans.num;
        return *this;
    }

    /**
     * @brief Multiplication of NumberWithUnits with double, 
     * double is from the right
     * 
     * @param otherDoubleNum 
     * @return NumberWithUnits 
     */
    NumberWithUnits NumberWithUnits::operator*(const double &otherDoubleNum) const {
        return NumberWithUnits(otherDoubleNum * this->num, this->unit_type);

    }

    /**
     * @brief  Multiplication of NumberWithUnits with double, 
     * double is from the left
     * 
     * @param otherDoubleNum 
     * @param otherNum 
     * @return NumberWithUnits 
     */
    NumberWithUnits operator*(const double &otherDoubleNum, const NumberWithUnits &otherNum) {
        return otherNum * otherDoubleNum;
    }

    //-------------------------------------
    // Comparison operators:
    //-------------------------------------

    /**
     * @brief == operator
     * 
     * @param otherNum 
     * @return true if equal
     * @return false if not equal
     */
    bool NumberWithUnits::operator==(const NumberWithUnits &otherNum) const {
        if (!is_convertable(otherNum)) {
            throw std::logic_error{"Error. The units types are from different dimensions and can't be compared."};
        }

        return abs(this->num - convert_to_this_unit(otherNum)) <= EPS;
    }

    /**
     * @brief !=operator
     * 
     * @param otherNum 
     * @return true if not equal
     * @return false if equal
     */
    bool NumberWithUnits::operator!=(const NumberWithUnits &otherNum) const {
        return !(*this == otherNum);
    }

    /**
     * @brief < operator
     * 
     * @param otherNum 
     * @return true 
     * @return false 
     */
    bool NumberWithUnits::operator<(const NumberWithUnits &otherNum) const {
        if (!is_convertable(otherNum)) {
            throw std::logic_error{"Error. The units types are from different dimensions and can't be compared."};
        }
        return (this->num) < convert_to_this_unit(otherNum);
    }

    /**
     * @brief > operator
     * 
     * @param otherNum 
     * @return true 
     * @return false 
     */
    bool NumberWithUnits::operator>(const NumberWithUnits &otherNum) const {
        return otherNum < (*this);
    }

    /**
     * @brief <= operator
     * 
     * @param otherNum 
     * @return true 
     * @return false 
     */
    bool NumberWithUnits::operator<=(const NumberWithUnits &otherNum) const {
        return ((*this) < otherNum) || ((*this) == otherNum);
    }

    /**
     * @brief >= operator
     * 
     * @param otherNum 
     * @return true 
     * @return false 
     */
    bool NumberWithUnits::operator>=(const NumberWithUnits &otherNum) const {
        return ((*this) > otherNum) || ((*this) == otherNum);
    }


    //----------------------------------
    // friend global IO operators
    //----------------------------------
    /**
     * @brief Output operator
     * 
     * @param output 
     * @param num 
     * @return std::ostream& 
     */
    std::ostream &operator<<(std::ostream &output, const NumberWithUnits &num) {
        output << num.num << '[' << num.unit_type << ']';
        return output;
    }

    /**
     * @brief 
     * Auxiliary function for the input operator.
     * leave inside the string only the unit
     * Receives a string and deletes spaces from the end and the beginning, 
     * as well as the character ']' from the beginning once and the character '[' from the end once.
     * @param str  
     * @return true -If the characters [] are deleted from the end and the beginning - that is, the input structure is valid 
     * @return false if the input structure is valid.
     */
    bool NumberWithUnits::remove_spaces_and_bracket(string &str) {
        string::size_type i = 0;
        bool first_bracket_left = true;
        //remove spaces from the start and '[' once
        while (i < str.size() && (str[i] == ' ' || (first_bracket_left && (str[i] == '[')))) {
            if (first_bracket_left && (str[i] == '[')) {
                first_bracket_left = false;
            }

            i++;
        }
        str.erase(0, i);
        i = str.size() - 1;
        bool first_bracket_right = true;
        // remove spaces from the end and ']' once
        while (i >= 0 && (str[i] == ' ' || (first_bracket_right && (str[i] == ']')))) {
            if (first_bracket_right && (str[i] == ']')) {
                first_bracket_right = false;
            }
            str.pop_back();
            i--;
        }
        return (!first_bracket_left && !first_bracket_right);//both appeared already  
    }

    /**
     * @brief input operator
     * 
     * @param input 
     * @param num 
     * @return std::istream& 
     */
    std::istream &operator>>(std::istream &input, NumberWithUnits &num)//
    {
        string input_unit_1, input_unit_2, input_unit_3;
        string input_unit;
        double input_num = 0;

        input >> input_num >> input_unit_1;
        //receive more input only if hasn't finised.
        if (input_unit_1[input_unit_1.size() - 1] != ']') {
            input >> input_unit_2;
            if (input_unit_2[input_unit_2.size() - 1] != ']') {
                input >> input_unit_3;
            }
        }

        input_unit = input_unit_1 + " " + input_unit_2 + " " + input_unit_3;
        //remove from input_unit anything that isn't the unit itself
        bool good_bracket_format = NumberWithUnits::remove_spaces_and_bracket(input_unit);
        if (!good_bracket_format) {
            throw "Error. Wrong input format.";
        }
        if (!(NumberWithUnits::units_convertions.contains(input_unit))) {
            throw "Error. The given unit type does not exist. ";
        }

        num.num = input_num;
        num.unit_type = input_unit;

        return input;
    }
}