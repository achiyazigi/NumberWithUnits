#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <readline/readline.h>
using namespace std;

#include "NumberWithUnits.hpp"
using namespace ariel;


ifstream units_file;
void get_file_name(string& str){
    cout << "insert input file name, enter nothing for units.txt (default): ";
    char* line = readline("");
    str.assign(line);

    // getting rid of the spaces at the end
    if(str.ends_with(' ')){
        str.resize(str.find_last_not_of(' ')+1);
    }

    if(str.empty())
    {
        str = "units.txt";
    }
   
}

void init_units_file(string input_file_name){
    units_file.open(input_file_name);
    while(units_file.fail()){
        cout << "reading from " << input_file_name << " failed." << endl;
        get_file_name(input_file_name);
        units_file.open(input_file_name);
    }
    cout << "the following input file has been processed successfuly: " << input_file_name << endl;
    NumberWithUnits::read_units(units_file);
}

void change_var(NumberWithUnits& var){
    try{
        cin >> var;
    }
    catch(invalid_argument e){
        cout << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n'); // clear the buffer

    }
}

void show_menu(){
    cout << "===========menu===========" << endl;
    cout << "|e:     end session      |" << endl;
    cout << "|c1:    change 1st unit  |" << endl;
    cout << "|c2:    change 2nd unit  |" << endl;
    cout << "|m:     show menu        |" << endl;
    cout << "|f:     show input file  |" << endl;
    cout << "|cf:    change input file|" << endl;
    cout << "==========================" << endl;
}



int main() {
    std::streamsize ss = std::cout.precision();
    string input_file_name;
    get_file_name(input_file_name);
    
    init_units_file(input_file_name);
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
    NumberWithUnits a;
    NumberWithUnits b;
    string oper;
    while(a.units() == ""){
        cout << "insert 1st unit (format example: 1[km]): ";
        change_var(a);
    }
    while(b.units() == ""){
        cout << "insert 2nd unit (format example: 100[cm]): ";
        change_var(b);
    }
    show_menu();
    cout.precision(ss);
    cout << boolalpha;
    do{
        cout << "choose an operator: (+,-,==,!=,<=,>=,<,>,+=,-=,++,--,<<)" << endl;
        cin >> oper;
        try{

            if(oper == "+"){
                cout << a << oper << b << '=' << a+b << endl;
            }
            else if(oper == "-"){
                cout << a << oper << b << '=' << a-b << endl;
            }
            else if(oper == "=="){
                cout << a << oper << b << '=' << (a==b) << endl;
            }
            else if(oper == "!="){
                cout << a << oper << b << '=' << (a!=b) << endl;
            }
            else if(oper == "<="){
                cout << a << oper << b << '=' << (a<=b) << endl;
            }
            else if(oper == ">="){
                cout << a << oper << b << '=' << (a>=b) << endl;
            }
            else if(oper == "<"){
                cout << a << oper << b << '=' << (a<b) << endl;
            }
            else if(oper == ">"){
                cout << a << oper << b << '=' << (a>b) << endl;
            }
            else if(oper == "+="){
                cout << a << oper << b << '=' << (a+=b) << endl;
            }
            else if(oper == "-="){
                cout << a << oper << b << '=' << (a-=b) << endl;
            }
            else if(oper == "++"){
                cout << a << oper << '=' << ++a << ' ' << b << oper << '=' << ++b << endl;
            }
            else if(oper == "--"){
                cout << a << oper << '=' << --a << ' ' << b << oper << '=' << --b << endl;
            }
            else if(oper == "<<"){
                cout << "first: " << a << " second: " << b << endl;
            }
            else if(oper == "m"){
                show_menu();
            }
            else if(oper == "c1"){
                cout << "insert 1st unit (format example: 1[km]): ";
                cin >> a;
            }
            else if(oper == "c2"){
                cout << "insert 2nd unit (format example: 100[cm]): ";
                cin >> b;
            }
            else if(oper == "f"){
                if(units_file.is_open()){
                    units_file.clear();
                    units_file.seekg(0);
                    cout << input_file_name << endl << units_file.rdbuf() << endl;
                }
            }
            else if(oper == "cf"){
                units_file.close();
                input_file_name = "new";
                cin.ignore();
                get_file_name(input_file_name);
                init_units_file(input_file_name);
            }
            
        }
        catch(invalid_argument e){
            cout << e.what() << endl;
            cin.clear();
            cin.ignore((streamsize)SIZE_MAX);
        }
    }
    while (oper != "e" && oper != "end");
    
    return 0;
}