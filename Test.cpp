// made by achiyazigi
#include "doctest.h"

using namespace std;
#include <iostream>

#include "NumberWithUnits.hpp"

using namespace ariel;

static char throw_message_expected[55];

/*
 * updates throw_message_expected with new units:
 * "Units do not match - [unit1] cannot be converted to [unit2]"
 */
static void generate_expected_throw(const string& unit1, const string& unit2) {
    string res = "Units do not match - [" + unit1 + "] cannot be converted to [" + unit2 + ']';
    size_t length = res.copy(throw_message_expected,55);
    throw_message_expected[length]='\0';
}

TEST_CASE("Operators"){
    /*
     * min1 = sec1 < min2 = 2*min1 = 2*60*sec1 < hour1
     * cm1 < km2 < 2*km2 = km1 = 100000*cm1 = cm2 = m1
     * g1 < 1000*g1 = kg1 < 1000*kg1 = ton1
     * NIS2 < 8*NIS2 = NIS1 = USD1
     */
    ifstream units_file{"units.txt"};
    NumberWithUnits::read_units(units_file);

    // min1 = sec1 < min2 = 2*min1 = 2*60*sec1 < hour1
    NumberWithUnits min1{3.5, "min"}; // = sec1
    NumberWithUnits min2{7, "min"}; // = 2*min1
    NumberWithUnits sec1{3.5 * 60 , "sec"}; // = min1
    NumberWithUnits hour1{1, "hour"};

    // cm1 < km2 < 2*km2 = km1 = 100000*cm1 = cm2 = m1
    NumberWithUnits km1{3.5, "km"}; // = 2*km2 = cm2 = m1
    NumberWithUnits km2{1.75, "km"};
    NumberWithUnits m1{3500, "m"}; // = km1
    NumberWithUnits cm1{3.5, "cm"}; // = 100000*cm2
    NumberWithUnits cm2{350000, "cm"}; // = km1

    // NIS2 < 8*NIS2 = NIS1 = USD1
    NumberWithUnits USD1{8, "USD"}; // = NIS1
    NumberWithUnits USD2{8 /3.33, "USD"}; // = NIS2
    NumberWithUnits NIS1{8 * 3.33, "ILS"}; // USD1
    NumberWithUnits NIS2{8, "ILS"}; // = USD2

    // g1 < 1000*g1 = kg1 < 1000*kg1 = ton1
    NumberWithUnits kg1{2.5, "kg"};
    NumberWithUnits ton1{2.5, "ton"};
    NumberWithUnits g1{2.5, "g"};

    SUBCASE("Operator=="){
    /*  nothing to see here... ->  */                                                                                                                                                                            cout << "(\\__/)    \033[1;31mAchiya\033[0m\n(>'.'<)   \033[1;31mZigler's\033[0m\n(\")_(\")   \033[1;31mTest\033[0m" << endl;
        CHECK_EQ(km1, cm2);
        CHECK_EQ(cm2, km1);
        CHECK_EQ(km1, m1);
        CHECK_EQ(cm2, m1);
        CHECK_EQ(USD1, NIS1);
        CHECK_EQ(NIS1, USD1);
        
        generate_expected_throw(cm1.units(), min1.units());
        CHECK_THROWS_WITH(bool b = (min1 == cm1);, throw_message_expected);
        generate_expected_throw(min1.units(), cm1.units());
        CHECK_THROWS_WITH(bool b = (cm1 == min1);, throw_message_expected);
    }

    SUBCASE("Operator!="){
        CHECK_NE(km1, cm1);
        CHECK_NE(km1, km2);
        CHECK_NE(NIS2, USD1);
        CHECK_NE(NIS2, NIS1);

        generate_expected_throw(cm1.units(), min1.units());
        CHECK_THROWS_WITH(bool b = (min1 != cm1);, throw_message_expected);
        generate_expected_throw(USD1.units(), min1.units());
        CHECK_THROWS_WITH(bool b = (min1 != USD1);, throw_message_expected);
    }

    SUBCASE("Operator<="){
        CHECK_LE(min1, min2);
        CHECK_LE(g1, kg1);
        CHECK_LE(kg1, ton1);
        CHECK_LE(g1 * 999, kg1);
        CHECK_LE(kg1 * 999, ton1);

        generate_expected_throw(cm1.units(), kg1.units());
        CHECK_THROWS_WITH(bool b = (kg1 <= cm1);, throw_message_expected);
        generate_expected_throw(USD1.units(), min1.units());
        CHECK_THROWS_WITH(bool b = (min1 <= USD1);, throw_message_expected);
    }

    SUBCASE("Operator>="){

        CHECK_GE(hour1, min1);
        CHECK_GE(hour1, min2);
        CHECK_GE(NIS1, USD1);
        CHECK_GE(USD1, NIS1);
        CHECK_GE(ton1, kg1);
        CHECK_GE(m1, cm2);
        CHECK_GE(km1, cm2);
        CHECK_GE(km2, cm1);

        generate_expected_throw(USD1.units(), g1.units());
        CHECK_THROWS_WITH(bool b = (g1 >= USD1), throw_message_expected);
        generate_expected_throw(m1.units(), sec1.units());
        CHECK_THROWS_WITH(bool b = (sec1 >= m1), throw_message_expected);
    }

    SUBCASE("Operator<"){
        CHECK_LT(min1, hour1);
        CHECK_LT(sec1, hour1);
        CHECK_LT(NIS2, NIS1);
        CHECK_LT(kg1, ton1);
        CHECK_LT(g1, kg1);
        CHECK_LT(km2, m1);
        CHECK_LT(km2, cm2);
        CHECK_LT(cm1, km2);

        generate_expected_throw(km1.units(), g1.units());
        CHECK_THROWS_WITH(bool b = (g1 < km1), throw_message_expected);
        generate_expected_throw(m1.units(), NIS1.units());
        CHECK_THROWS_WITH(bool b = (NIS1 < m1), throw_message_expected);
    }

    SUBCASE("Operator>"){
        CHECK_GT(hour1, min1);
        CHECK_GT(hour1, sec1);
        CHECK_GT(NIS1, NIS2);
        CHECK_GT(ton1, kg1);
        CHECK_GT(kg1, g1);
        CHECK_GT(m1, km2);
        CHECK_GT(cm2, km2);
        CHECK_GT(km2, cm1);

        generate_expected_throw(km1.units(), g1.units());
        CHECK_THROWS_WITH(bool b = (g1 > km1), throw_message_expected);
        generate_expected_throw(m1.units(), NIS1.units());
        CHECK_THROWS_WITH(bool b = (NIS1 > m1), throw_message_expected);
    }

    SUBCASE("Operator*"){
        NumberWithUnits min = min1 * 2;
        NumberWithUnits km = km1 * 2;
        CHECK_EQ(min.number(), 7);
        CHECK_EQ(min.units(), "min");
        CHECK_EQ(min, min2);
        CHECK_NE(km.number(), (2 * km2).number());
        CHECK_NE(km2.number(), km1.number());
        CHECK_EQ(km.units(), "km");
        CHECK_NE(km, km2);
        CHECK_EQ(km, 4 * km2);
        CHECK_EQ(g1 * 1000, kg1);
        CHECK_EQ(1000 * g1, kg1);
        CHECK_EQ(kg1 * 1000, ton1);
        CHECK_EQ(1000 * kg1, ton1);
        CHECK_GT(sec1 * 60, hour1);
        CHECK_EQ(60 * sec1 * (1/3.5), hour1);
    }

    SUBCASE("Operator+"){
        CHECK_EQ(sec1 + min1, min2);
        CHECK_EQ((cm1 + km1).number(), 350003.5);
        CHECK_EQ((km1 + cm1).number(), 3.500035);
        CHECK_EQ((hour1 + min1).number(), 1 + 3.5 / 60);
        CHECK_EQ((min1 + hour1).number(), 3.5 + 60);
        CHECK_EQ((NIS1 + USD1).number(), (2 * USD1).number() * 3.33);
        CHECK_EQ(USD1 + NIS1, 2 * USD1);
        CHECK_EQ(g1 + ton1, NumberWithUnits{2.5 * 1000 * 1000 + 2.5, "g"});
        CHECK_EQ(ton1 + kg1, NumberWithUnits{2.5 + 2.5 / 1000, "ton"});

        generate_expected_throw(g1.units(), km1.units());
        CHECK_THROWS_WITH((km1 + g1) + sec1, throw_message_expected);
        generate_expected_throw(sec1.units(), g1.units());
        CHECK_THROWS_WITH(km1 + (g1 + sec1), throw_message_expected);
        generate_expected_throw(USD1.units(), ton1.units());
        CHECK_THROWS_WITH(ton1 + USD1, throw_message_expected);
    }

    SUBCASE("Operator-"){
        CHECK_EQ((min1 - hour1).number(), 3.5 - 60);
        CHECK_EQ((km1 - cm1).number(), 3.5 - 0.000035);
        CHECK_EQ((m1 - cm2).number(), 0);
        CHECK_EQ((USD1 - NIS1 - USD1).number(), -8);
        CHECK_EQ(+(USD1 - NIS1 - USD1).number(), -8); // yes, its not a mistake (read about unary plus)
        CHECK_EQ((USD1 - NIS2).number(), 8 - 8 / 3.33);
        CHECK_EQ((NIS2 - USD2).number(), 0);
        CHECK_EQ((min1 - sec1 - min2), -min2);
        CHECK_EQ(-(min1 - sec1 - min2), min2);

        generate_expected_throw(g1.units(), km1.units());
        CHECK_THROWS_WITH((km1 - g1) + sec1, throw_message_expected);
        generate_expected_throw(sec1.units(), g1.units());
        CHECK_THROWS_WITH(km1 + (g1 - sec1), throw_message_expected);
        generate_expected_throw(USD1.units(), ton1.units());
        CHECK_THROWS_WITH(ton1 - USD1, throw_message_expected);
    }

    SUBCASE("Operator+="){
        NumberWithUnits km = km1; // = 3.5[km]
        NumberWithUnits m = m1; // = 3500[m]
        km += m; // = 7[km]
        CHECK_EQ(km.number(), 7);
        CHECK_EQ(km.units(), "km");
        CHECK_EQ(km1.number(), 3.5);
        m += km; // = 10500[m]
        CHECK_EQ(m, km * 1.5);
        CHECK_EQ(m.units(), "m");
        CHECK_EQ(m1.number(), 3500);

        generate_expected_throw(cm1.units(), kg1.units());
        CHECK_THROWS_WITH(kg1 += cm1;, throw_message_expected);
        generate_expected_throw(USD1.units(), min1.units());
        CHECK_THROWS_WITH(min1 += USD1;, throw_message_expected);
        km.~NumberWithUnits();
    }

    SUBCASE("Operator-="){
        NumberWithUnits USD_first = USD1;
        NumberWithUnits USD_second = USD2;
        NumberWithUnits NIS = NIS1;
        USD_first -= USD_second;
        CHECK_EQ((USD_first).number(), 8 - 8 / 3.33);
        CHECK_EQ(USD_first.units(), "USD");
        CHECK_EQ(USD1.number(), 8);
        NIS -= USD_second;
        CHECK_EQ(NIS.number(), 8 * 3.33 - 8);
        CHECK_EQ(NIS.units(), "ILS");
        CHECK_EQ(NIS1.number(), 8 * 3.33);

        generate_expected_throw(kg1.units(), sec1.units());
        CHECK_THROWS_WITH(sec1 -= kg1;, throw_message_expected);
        generate_expected_throw(kg1.units(), min1.units());
        CHECK_THROWS_WITH(min1 -= kg1;, throw_message_expected);
    }

    SUBCASE("Operator++"){
        
        NumberWithUnits km_toinc = km1;
        NumberWithUnits one_km{1, "km"};
        CHECK_EQ(km_toinc++, km1);
        CHECK_EQ(km_toinc, km1 + one_km);
        CHECK_EQ(km_toinc++++, km1 + one_km++);
        CHECK_EQ(km_toinc, km1 + one_km);
    }

    SUBCASE("Operator--"){
        NumberWithUnits km_toinc = km1;
        NumberWithUnits one_km{1, "km"};
        CHECK_EQ(km_toinc--, km1);
        CHECK_EQ(km_toinc, km1 - one_km);
        CHECK_EQ(km_toinc----, km1 - one_km++);
        CHECK_EQ(km_toinc, km1 - one_km);
    }

    SUBCASE("++Operator"){
        NumberWithUnits km_toinc = km1;
        NumberWithUnits one_km{1, "km"};
        CHECK_EQ(++km_toinc, km1 + one_km);
        CHECK_EQ(km_toinc, km1 + one_km++);
        CHECK_EQ(++++km_toinc, km1 + ++one_km);
        CHECK_EQ(++km_toinc, km1 + ++one_km);
    }

    SUBCASE("--Operator"){
        NumberWithUnits km_toinc = km1;
        NumberWithUnits one_km{1, "km"};
        CHECK_EQ(--km_toinc, km1 - one_km);
        CHECK_EQ(km_toinc, km1 - one_km++);
        CHECK_EQ(----km_toinc, km1 - ++one_km);
        CHECK_EQ(--km_toinc, km1 - ++one_km);
    }

    SUBCASE("Operator<<"){
        ostringstream os;
        os << ton1;  
        CHECK_EQ(os.str(), "2.5[ton]");
        os.str(string());
        os << kg1 << m1 << sec1 + min1;
        CHECK_EQ(os.str(), "2.5[kg]3500[m]420[sec]");
    }

    SUBCASE("Operator>>"){
        NumberWithUnits new_NumberWithUnits;
        istringstream sinput("2.0000008 [  zigi   ]");
        sinput >> new_NumberWithUnits;
        CHECK(sinput);
        CHECK_EQ(new_NumberWithUnits.number(), 2.0000008);
        CHECK_EQ(new_NumberWithUnits.units(), "zigi");
    }

}

TEST_CASE("achiyazigi_units"){ // the fun part
    // creating new txt file with some new units
    ofstream outfile ("achiyazigi_units.txt"); //    that's meee

    outfile << "1 mile = 1609.34 m" << endl; // whats is it gonna be with those americans...
    outfile << "1 km = 1000 m" << endl;
    outfile << "1 m = 100 cm" << endl;
    outfile << "1 l = 1000 ml" << endl;
    outfile << "1 l = 0.264172 gal" << endl; // yep
    outfile << "1 lb = 0.453592 kg" << endl;
    outfile << "1 day = 24 hour" << endl;
    outfile << "1 year = 365 day" << endl;
    outfile << "1 hour = 60 min" << endl;
    outfile << "1 min = 60 sec" << endl;
    outfile << "1 EUR = 1.19 USD" << endl;
    outfile << "1 USD = 3.33 ILS" << endl;
    outfile.close();
    ifstream myunits{"achiyazigi_units.txt"}; 
    NumberWithUnits::read_units(myunits);
    remove("achiyazigi_units.txt"); // cleaning after myself...

    NumberWithUnits days;
    NumberWithUnits lb;
    NumberWithUnits year{1, "year"};
    NumberWithUnits lbtokg{3.78 * 0.453592, "kg"}; // 3.78 pound converted to kg
    NumberWithUnits secinyear{60*60*24*365, "sec"}; // seconds in year = 1[year]
    NumberWithUnits EUR{2.6, "EUR"};
    NumberWithUnits ILS{2.6, "ILS"};
    istringstream in{"365[day]"}; // = 1[year]
    in >> days;
    in.str("3.78[ lb]");
    in >> lb;

    CHECK_EQ(lb.number(), 3.78);
    CHECK_EQ(lb.units(), "lb");
    CHECK_EQ(lb, lbtokg);
    CHECK(abs((lb + (lb * 0.5) - lbtokg * 1.5).number()) <= __DBL_EPSILON__ * 10); // division is not precise
    CHECK_EQ(days, year);
    CHECK_EQ(days * (1.0/(365 * 24)), NumberWithUnits{1, "hour"});
    CHECK_EQ(secinyear, year);
    CHECK_EQ(secinyear, days);
    CHECK_EQ(secinyear + secinyear, 2 * year);

    CHECK_NE(EUR, ILS);
    CHECK_GT(EUR, ILS);
    CHECK(abs((EUR - ILS * 3.33 * 1.19).number()) <= __DBL_EPSILON__ * 10); // division is not precise
}