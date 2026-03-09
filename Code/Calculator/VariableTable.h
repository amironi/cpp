#pragma once 

#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Utils.h"

using namespace std;

class VariableTable {
  public:
    void set(const string& userInput) {
        map_.clear();
        
        vector<string> v = split(userInput, " ");
        for(int i=0 ; i < v.size(); i +=2){
            add(v[i][0], stoi(v[i+1]));
        }
    }

    void add(char var, int val) {
        map_[var] = val;
    }

    int get(char var) {
        return map_[var];
    }
    
  private:
    unordered_map<char, int> map_;
};

