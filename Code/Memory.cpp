#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

int pool[2*1024];// = new int(N);

void init() {
    foreach(pool, pool + 2024, free);
}

struct Block {   
    Block* next;
};

int* mem = nullptr;

int* malloc() {
    int* ret = mem;
    mem = mem ? *mem : null;
    return ret;
}

void free(int* x) {
    *x = mem;
    mem = x;
}



