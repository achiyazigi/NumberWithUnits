#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

#include "NumberWithUnits.hpp"
using namespace ariel;

void get_file_name(string& str){
    cout << "insert input file name, enter nothing for units.txt (default): ";
    if(std::getline(std::cin, str))
    {
        if(str.empty())
        {
            str = "units.txt";
        }
    }
    else
    {
        std::cout << "Input failed - no more input?" << std::endl;
    }
}

int main() {
    string input_file_name;
    get_file_name(input_file_name);
    
    ifstream units_file{input_file_name};
    while(units_file.fail()){
        cout << "reading from " << input_file_name << " failed." << endl;
        get_file_name(input_file_name);
        units_file.open(input_file_name);
    }
    NumberWithUnits::read_units(units_file);
    /**
     * content of units file:
     * 1 km = 1000 m
     * 1 m = 100 cm
     * 1 kg = 1000 g
     * 1 ton = 1000 kg
     * 1 hour = 60 min
     * 1 min = 60 sec
     * 1 USD = 3.33 ILS
     */
    NumberWithUnits km1;
    NumberWithUnits m1{1, "m"};
    NumberWithUnits cm1{1, "cm"};
    NumberWithUnits km2 = 1000 * m1; // = km1
    NumberWithUnits m2 = 100 * cm1; // = m1
    NumberWithUnits cm2 = km1 * (1.0/100000); // = cm1
    cout << "the following input file has been processed successfuly: " << input_file_name << endl;
    cout << "insert 1st unit (format example: 1[km]): ";
    cin >> km1;
    cout << km1 << endl;
    cout << "insert 2nd unit (format example: 100[cm]): ";
    cin >> km1;
    cout << km1 << endl;
    return 0;
}