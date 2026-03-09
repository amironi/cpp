#pragma once 

#include <iostream>
#include <string>
#include <map>

using namespace std;

double plus_func(double a , double b) {return a + b;}
double minus_func(double a , double b) {return a - b;}
double mul_func(double a , double b) {return a * b;}
double div_func(double a , double b) {return a / b;}

typedef double(*op_func)(double,double);

class AritOperators {
  public:
  AritOperators() {
    operator_map_['+'] = &plus_func;
    operator_map_['-'] = &minus_func;
    operator_map_['*'] = &mul_func;
    operator_map_['/'] = &div_func;
  }

  map<char, op_func> operator_map_;  
};
