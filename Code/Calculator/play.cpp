#include <iostream>
#include <string>
#include <map>
#include "ArithmeticExp.h"

using namespace std;

int main(int argc, char *argv[]){

    VariableTable variable_table;
    ArithmeticExp arithmetic_exp(argv[1], variable_table);

    for (std::string line; std::getline(std::cin, line);) {
        variable_table.set(line);

        cout << "=" << arithmetic_exp.run() << endl;
    }
}