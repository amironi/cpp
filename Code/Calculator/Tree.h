#pragma once 

#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <map>
#include "VariableTable.h"
#include "AritOperators.h"

using namespace std;

class iNode {
  public:
    virtual double val() = 0;
};

class Expression : public iNode {
  public:
    Expression(  
      iNode* left,
      iNode* right,
      char oper,
      AritOperators& AritOperators) :
      left_(left),
      right_(right),
      operator_(oper),
      AritOperators_(AritOperators)
    {
    }

    virtual ~Expression(){
        delete left_;
        delete right_;
    }

    double val() { return AritOperators_.operator_map_[operator_](left_->val(), right_->val());}

  private:
    iNode* left_;
    iNode* right_;
    char operator_;
    AritOperators& AritOperators_;
};

class Numeric : public iNode {
  public:
    Numeric(int v) {val_ = v;}
    
    double val() { return val_;}

  private:
    int val_;
};

class Variable : public iNode {
  public:
    Variable(VariableTable* v_table, char name) {v_table_ = v_table; name_ = name;}

    double val() { return v_table_->get(name_);}

  private:
    VariableTable* v_table_;
    char name_;
};
