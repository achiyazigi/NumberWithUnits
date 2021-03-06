#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>

namespace ariel{
    
    class NumberWithUnits{
    private:
        double _number = 0;
        std::string _units = "";
    public:
        // NumberWithUnits()=default;
        NumberWithUnits(const double& number, const std::string& units);
        NumberWithUnits(const NumberWithUnits& other):_number{other.number()}, _units{other.units()} {}
        ~NumberWithUnits() = default;
        static void read_units(std::ifstream& stream);

        /*
         * getters
         */
        double number() const {
            return _number;
        }
        std::string units() const {
            return _units;
        }

        /*
         * operators
         */
        static double get_converted(const std::string& convert_to, const std::string& to_convert);
        
        /*
         * unary operators
         */
        bool operator!() const {
            return _number == 0;
        }

        const NumberWithUnits operator-() const {
            return NumberWithUnits(-_number, _units);
        }
        const NumberWithUnits operator+() const {
            return NumberWithUnits(+_number, _units);
        }

        /*
         * binary operators
         */
        NumberWithUnits& operator+=(const NumberWithUnits& other);
        NumberWithUnits& operator-=(const NumberWithUnits& other);
        NumberWithUnits& operator*=(const double& d);

        NumberWithUnits operator+(const NumberWithUnits& other) const;
        NumberWithUnits operator-(const NumberWithUnits& other) const;
        bool operator==(const NumberWithUnits& other) const;
        bool operator!=(const NumberWithUnits& other) const;
        bool operator<=(const NumberWithUnits& other) const;
        bool operator>=(const NumberWithUnits& other) const;
        bool operator<(const NumberWithUnits& other) const;
        bool operator>(const NumberWithUnits& other) const;
        
        /*
         * friend global binary operators
         */
        friend const NumberWithUnits operator*(const NumberWithUnits& nu, const double& scalar) {
            return NumberWithUnits(nu._number * scalar, nu._units);
        }
        friend const NumberWithUnits operator*(const double& scalar, const NumberWithUnits& nu) {
            return NumberWithUnits(nu._number * scalar, nu._units);
        }

        /*
         * friend global IO operators
         */
        friend std::ostream& operator<< (std::ostream& os, const NumberWithUnits& nu);
        friend std::istream& operator>> (std::istream& is, NumberWithUnits& nu);

        /*
         * prefix inc operator
         */
        NumberWithUnits& operator++() {
            _number++;
            return *this;
        }

        /*
         * prefix dcr operator
         */
        NumberWithUnits& operator--() {
            _number--;
            return *this;
        }

        /*
         * postfix inc operator
         */
        NumberWithUnits operator++(int dummy_flag_for_postfix_increment) {
            NumberWithUnits copy = *this;
            _number++;
            return copy;
        }

        /*
         * postfix dcr operator
         */
        NumberWithUnits operator--(int dummy_flag_for_postfix_increment) {
            NumberWithUnits copy = *this;
            _number--;
            return copy;
        }



    };
}