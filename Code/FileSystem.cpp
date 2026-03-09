// SimpleFS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "map.h"
#include <conio.h>

using namespace std;

class Tree {
    void add(string name) {
        pair = split(name);
        subs_[p.first].add(p.second);
    }
  private:
	map<name, Tree> subs_;
};

void print(string pref, Node n) {
    if(subs_.size() == 0){
        cout << pref;
        return
    }

    for(auto i: n.subs_) {
        print(pref + i.first + '/', i.second);
    }
}



int main() {

    Node root;
    root.add("abc");
    root.add("abc/def")

    return 0;
}

