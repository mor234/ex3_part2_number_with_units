/**
 * @file NumberWithUnits.hpp
 * @author mor234
 * @brief 
 * @version 0.1
 * @date 2021-04-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <iostream>
#include <map>

namespace ariel {

    class NumberWithUnits {
    private:
        //variables
        static std::map<std::string, std::map<std::string, double>> units_convertions;
        constexpr static const double EPS = 0.001;
        double num;
        std::string unit_type;

        //functions
        double convert_to_this_unit(const NumberWithUnits &otherNum) const;

        bool is_convertable(const NumberWithUnits &otherNum) const;

        static void update_units(std::string unit1, std::string unit2, double unit1_to_unit2);

    public:
        NumberWithUnits() {};

        NumberWithUnits(const NumberWithUnits &other);

        NumberWithUnits(double num, std::string unit);

        static void read_units(std::ifstream &units_file);
        //-------------------------------------
        // operators
        //-------------------------------------

        //----------------------------------
        // unary operators
        //----------------------------------

        //Addition
        NumberWithUnits operator+() const;

        // prefix increment:
        NumberWithUnits &operator++();

        // postfix increment:
        NumberWithUnits operator++(int dummy_flag_for_postfix);

        //Subtraction
        NumberWithUnits operator-() const;

        // prefix decrement:
        NumberWithUnits &operator--();

        // postfix decrement:
        NumberWithUnits operator--(int dummy_flag_for_postfix);

        //----------------------------------------
        // binary operators
        //----------------------------------------

        //Addition
        NumberWithUnits operator+(const NumberWithUnits &otherNum) const;

        NumberWithUnits &operator+=(const NumberWithUnits &otherNum);

        //Subtraction
        NumberWithUnits operator-(const NumberWithUnits &otherNum) const;

        NumberWithUnits &operator-=(const NumberWithUnits &otherNum);

        //multiplication
        NumberWithUnits operator*(const double &otherDoubleNum) const;

        friend NumberWithUnits operator*(const double &otherDoubleNum, const NumberWithUnits &otherNum);

        //Comparison operators
        bool operator<(const NumberWithUnits &otherNum) const;

        bool operator>(const NumberWithUnits &otherNum) const;

        bool operator<=(const NumberWithUnits &otherNum) const;

        bool operator>=(const NumberWithUnits &otherNum) const;

        bool operator==(const NumberWithUnits &otherNum) const;

        bool operator!=(const NumberWithUnits &otherNum) const;

        //----------------------------------
        // friend global IO operators
        //----------------------------------
        friend std::ostream &operator<<(std::ostream &output, const NumberWithUnits &num);

        friend std::istream &operator>>(std::istream &input, NumberWithUnits &num);
    };
}