#pragma once 

#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <assert.h>
#include <set>
#include <list>
#include "VariableTable.h"
#include "Tree.h"

using namespace std;

static AritOperators AritOperators_;

class ArithmeticExp {
  public:
    ArithmeticExp(const string& template_user, VariableTable& variable_table_) {
        const vector<string> v = split(template_user, " ");

        stack<iNode*> nodes;

        for(auto& str: v) {
            
            char c = str[0];
            
            if(isalpha(c)) {
                nodes.push(new Variable(&variable_table_, c));
            }
            else if(is_number(str)) {
                nodes.push(new Numeric(stoi(str)));
            }
            else if(k_all_operators_.find(c) != k_all_operators_.end()) {

                iNode* b = nodes.top(); nodes.pop();
                iNode* a = nodes.top(); nodes.pop();

                // TODO: optimization if a && b numeric can create new Numeric instead new Expression and delete a/b 
                nodes.push(new Expression(a, b, c, AritOperators_));
            }
        }

        assert(nodes.size() == 1);
        root_ = nodes.top();
    }

    double run() {
        return root_->val();
    }

    virtual ~ArithmeticExp() {
        delete root_;
    }

  private:
    iNode* root_;
    const set<char> k_all_operators_{'+','-','/','*'};
};