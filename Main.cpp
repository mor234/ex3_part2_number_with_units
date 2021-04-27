#include "NumberWithUnits.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>


using namespace ::std;
using namespace ::ariel;

void create_my_units_file() {
    ofstream units_file;
    units_file.open("my_units.txt");
    units_file << "1 BTC = 54926 USD" << endl;
    units_file << "1 ETH = 2629 USD" << endl;
    units_file << "1 USD = 3.33 ILS" << endl;
    units_file << "1 hour = 60 min" << endl;
    units_file << "1 min = 60 sec" << endl;
    units_file << "1 day = 24 hour" << endl;
    units_file.close();
}

int main() {
    create_my_units_file();
    ifstream units_file_stream{"my_units.txt"};
    NumberWithUnits::read_units(units_file_stream);
    cout << "[̲̅$̲̅(̲̅5̲̅)̲̅$̲̅] [̲̅$̲̅(̲̅5̲̅)̲̅$̲̅] [̲̅$̲̅(̲̅5̲̅)̲̅$̲̅] [̲̅$̲̅(̲̅5̲̅)̲̅$̲̅]";
    cout << endl << endl;
    cout << "       --------Welcome to the Moriya Crypto Trading Unit!-------";
    cout << endl << endl;
    cout << "[̲̅$̲̅(̲̅5̲̅)̲̅$̲̅] [̲̅$̲̅(̲̅5̲̅)̲̅$̲̅] [̲̅$̲̅(̲̅5̲̅)̲̅$̲̅] [̲̅$̲̅(̲̅5̲̅)̲̅$̲̅]";
    cout << endl << endl;
    cout << "===================================================" << endl;

    NumberWithUnits
            hour(6, "hour"),
            day2(0.125, "day"),
            min(3.25, "min"),
            day(0.5, "day"),
            sec(195, "sec"),
            usd(32, "USD"),
            eth(20, "ETH"),
            ils(3.2, "ILS"),
            btc(15, "BTC");

    cout << endl << "Calculations examples:" << endl << endl;
    cout << "    " << ils << " + " << ils << " = " << ils + ils << endl << endl;
    cout << "    " << eth << " += " << btc << " = " << (eth += btc) << endl << endl;
    cout << "    " << eth << " - " << usd << " = " << eth - usd << endl << endl;
    cout << "    " << hour << " - " << day2 << " = " << hour - day2 << endl << endl;
    cout << "    " << 109 << " * " << btc << " = " << (109 * btc) << endl << endl;
    cout << "    " << day << " ++ " << "= " << day++ << endl << endl;
    try {
        cout << "    " << day << "+" << ils << " = " << day + ils << endl << endl;
    }
    catch (exception &ex) {
        cout << ex.what() << endl << endl;
    }

    cout << "===================================================" << endl;
    cout << endl << "Comperation examples:" << endl << endl;
    cout << boolalpha;

    cout << "    (" << min << " == " << sec << ") = " << (min == sec) << endl << endl;
    cout << "    (" << day << " != " << hour << ") = " << (day != hour) << endl << endl;
    cout << "    (" << ils << " > " << btc << ") = " << (ils > btc) << endl << endl;


    cout << "===================================================" << endl << endl;
    cout << "The conversion data base:" << endl;
    NumberWithUnits::print_units();
    cout << "===================================================" << endl;


}