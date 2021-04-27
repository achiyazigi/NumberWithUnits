#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

#include "NumberWithUnits.hpp"
using namespace ariel;

/*
 * holds the normalized convertion values
 */
struct myGraph{
    unordered_map<string, unordered_map<string, double>> edges;
    unordered_set<string> _nodes;

    const unordered_set<string>& nodes() const{
        return _nodes;
    }

    int size() const{
        return _nodes.size();
    }

    void addNode(const string& v){
        _nodes.insert(v);
    }

    // A utility function to add an edge in directed graph.
    void addEdge(const string& u, const string& v, double w)
    {
        addNode(u);
        addNode(v);
        edges[u][v] = w;
        edges[v][u] = 1/w;
        // for example: if u="km", v="m", w=0.001, then:
        // km->m = 0.001, m->km = 1000
    }

    // returns edge (u,v) value.
    double getEdge(const string& u, const string& v){
        if(_nodes.contains(u) && _nodes.contains(v)){
            return edges[u][v];
        }
        
        return -1;
    }

    // A utility function to print the adjacency list
    // representation of graph (in use when debugging)
    void printGraph()
    {
        for (const auto& v : _nodes)
        {
            cout << "\n Adjacency list of vertex "
                << v << "\n head ";
            for (auto& x : edges[v]){
                cout << "-> " << x.first << '(' << x.second << ')';
            }
            cout << endl;
        }
    }
};

static myGraph g; // will contain the normalized convertion values, as explained in NumberWithUnits::read_units

/* 
 * generates the expected exception message:
 * "Units do not match - [unit2] cannot be converted to [unit1]"
 */
void throw_invalid_arguments(const string& unit1, const string& unit2){
    throw invalid_argument("Units do not match - ["+unit2+"] cannot be converted to ["+unit1+"]");
}

/*
 * returns the normalized convertion value bitween two units, -1 if no such value.
 * implementation uses DFS starting from node "convert_to" ending at "to_convert":
 * 1.   map con[convert_to] = 1.
 * 2.   cur = convert_to
 * 3.   on each visit from node cur to n through edge e, con[n] = con[cur] * w(e)
 *      if to_convert reached: add edges:
 *      (convert_to, to_convert, con[to_convert])
 *      (to_convert, convert_to, 1/con[to_convert])
 *      for future use.
 * 4.   return con[to_convert]
 */
double NumberWithUnits::get_converted(const string& convert_to, const string& to_convert){
    stack<string> s;
    unordered_set<string> visited;
    unordered_map<string, double> con;
    con.clear();
    s.push(convert_to);
    con[convert_to] = 1;
    while (!s.empty())
    {
        string cur = s.top();
        s.pop();
        if(!visited.contains(cur)){
            visited.insert(cur); // update visited node
            if(cur == to_convert){ // goal reached
                if(g.getEdge(convert_to, to_convert) <= 0 && convert_to != to_convert){ // new shorting edge found:
                    g.addEdge(convert_to, to_convert, con[cur]);
                    // cout << "augmented with " << convert_to << " -> " << to_convert << endl;
                    // cout << "augmented with " << to_convert << " -> " << convert_to << endl;
                }
                return con[cur];
            }
            for(auto& n : g.edges[cur]){ // add unvisited neighbors to s
                string v = n.first;
                if(!visited.contains(v)){
                    double w = n.second;
                    con[v] = (w * con[cur]); // update con[v]
                    s.push(v);
                }
            }
        }
    }
    return -1; // the convertion faild (convert_to & to_convert are in different CC's)
}

/* 
 * proccess the input text and builds directional weighted graph.
 * the expected input is a text file where each line like so:
 * 1 {unit1} = {convertion value} {unit2}
 * for example: 
 * 1 km = 1000 m
 * each line creates 2 edges in myGraph g. (for 2 directions convertion)
 */
void NumberWithUnits::read_units(std::ifstream& stream){
    string line;
    double leftDouble = 0;
    double rightDouble = 0;
    string leftString;
    string rightString;
    string equation_sign;
    while(getline(stream, line)){
        std::stringstream linestream(line);

        // left side of equation:
        linestream >> skipws >> leftDouble;
        linestream >> skipws >> leftString;

        // check for '=':
        linestream >> skipws >> equation_sign;
        if(equation_sign != "="){
            throw invalid_argument("bad format in line: " + line);
        }

        // right side of equation:
        linestream >> skipws >> rightDouble; 
        linestream >> skipws >> rightString;
        
        // update myGrapg g:
        g.addEdge(leftString, rightString, leftDouble / rightDouble);
        g.addEdge(rightString, leftString, rightDouble / leftDouble);
 
    }
}

//========implemantaion of unimplemented methods of NumberWithUnits========

//(trivial methods already implemented in NumberWithUnits.hpp)
/**
 * constructor
 */
NumberWithUnits::NumberWithUnits(const double& number, const std::string& units): _number(number), _units(units){
    if(!(g._nodes.contains(units))){
        throw invalid_argument("unit named: ["+units+"] have not been initialized");
    }
}

/*
 * binary operators
 */
NumberWithUnits& NumberWithUnits::operator+=(const NumberWithUnits& other) {
    double converted = NumberWithUnits::get_converted(this->_units, other._units);
    if(converted < 0){
        throw_invalid_arguments(this->_units, other._units);
    }
    this->_number += converted * other._number;
    return *this;

}
NumberWithUnits& NumberWithUnits::operator-=(const NumberWithUnits& other) {
    double converted = NumberWithUnits::get_converted(this->_units, other._units);
    if(converted < 0){
        throw_invalid_arguments(this->_units, other._units);
    }
    this->_number -= converted * other._number;
    return *this;

}
NumberWithUnits& NumberWithUnits::operator*=(const double& d) {
    this->_number *= d;
    return *this;

}


/*
 * friend global binary operators
 */
NumberWithUnits ariel::operator+(const NumberWithUnits& nu1, const NumberWithUnits& nu2) {
    double converted = NumberWithUnits::get_converted(nu1.units(), nu2.units());
    if(converted < 0){
        throw_invalid_arguments(nu1.units(), nu2.units());
    }
    return NumberWithUnits(nu1.number() + converted * nu2.number(), nu1.units());

}
NumberWithUnits ariel::operator-(const NumberWithUnits& nu1, const NumberWithUnits& nu2) {
    return nu1 + -nu2;

}
bool ariel::operator==(const NumberWithUnits& nu1, const NumberWithUnits& nu2) {
    double converted = NumberWithUnits::get_converted(nu1.units(), nu2.units());
    if(converted < 0){
        throw_invalid_arguments(nu1._units, nu2._units);
    }
    const double TOLARENCE = 0.00001;
    return abs(nu1.number() - converted * nu2.number()) <= TOLARENCE;
}

bool ariel::operator!=(const NumberWithUnits& nu1, const NumberWithUnits& nu2) {
    return !(nu1 == nu2);
}

bool ariel::operator<=(const NumberWithUnits& nu1, const NumberWithUnits& nu2) {
    double converted = NumberWithUnits::get_converted(nu1.units(), nu2.units());
    if(converted < 0){
        throw_invalid_arguments(nu1._units, nu2._units);
    }
    
    return nu1.number() < converted * nu2.number() || nu1 == nu2;
}

bool ariel::operator>=(const NumberWithUnits& nu1, const NumberWithUnits& nu2) {
    double converted = NumberWithUnits::get_converted(nu1.units(), nu2.units());
    if(converted < 0){
        throw_invalid_arguments(nu1._units, nu2._units);
    }
   
    return nu1.number() > converted * nu2.number() || nu1 == nu2;
}

bool ariel::operator<(const NumberWithUnits& nu1, const NumberWithUnits& nu2) {
    return !(nu1 >= nu2);
    
}
bool ariel::operator>(const NumberWithUnits& nu1, const NumberWithUnits& nu2) {
    return !(nu1 <= nu2);

}

/*
* friend global IO operators
*/
ostream& ariel::operator<< (ostream& os, const NumberWithUnits& nu) {
    return (os <<  nu.number() << '[' << nu.units() << ']');
}

istream& ariel::operator>> (istream& is, NumberWithUnits& nu) {
    double n = 0;
    string str;
    // read the number:
    is >> skipws >> n;

    //skip spaces
    while(is.peek() == ' ' || is.peek() == '\t'){
        is.ignore();
    }

    // check if no more input
    if(is.peek() == '\n'){
        is.setstate(ios::failbit);
    }

    // get the rest input for further process


    else{
        getline(is, str, ']');
    }
    // '[' has to be the next char:
    if(!str.starts_with('[')){
        is.setstate(ios::failbit);
    }

    else{
        // get the string bitween [] brackets:
        str = str.c_str()+1;
    }

    // here the program will throw the exception if the input was wrong:
    // auto errorState = is.rdstate();
    if(is.fail()){
        throw invalid_argument{"wrong input format"};
    }

    // since the units name should be spaceless (by inputfile definition),
    // the input will be the first word in the brackets:
    string str2 = str.c_str() + str.find_first_not_of(' '); 
    size_t first_space = str2.find_first_of(' ');
    if(str2.size() > first_space && first_space > 0){
        str2.resize(first_space);
    }

    // check if units from input exsits in myGraph g:
    if(!g.nodes().contains(str2)){
        throw invalid_argument{"unit named: ["+str2+"] have not been initialized"};
    }

    // input completed seccessfully.
    nu._number = n;
    nu._units = str2;
    return is;
}

    
