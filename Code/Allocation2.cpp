/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

template<class T>
class Item {
    Item* next;
    Item* prev
    T data;
};

template<class T>
class Allocator {
    Item<T>* alloc() {

        if(p == nullptr) {
            p = new T[100];
        }   
        
        Item<T>* ret = p;
        p = p->next;        
        return ret;
    }

    free(Item<T>* item) {

    }
    Item<T>* p;
};

template<class T>
class MyVec {
    Allocator<T> allocator{100};
    Item<T>* head_{nullptr};
    Item<T>* tail_{nullptr};
    
    T pop_back() {
        Item<T>* to_del = tail_;
        tail_ = tail_->prev;
        tail_->next = nullptr;
        Allocator.free(to_del);
            
        return to_del.data;
    }
    
    T& push_back() {
        Item<T>* new_item = Allocator.alloc();
        tail_->next = new_item;
        new_item->prev = tail_;
        tail_ = new_item;
        return new_item.data;
    }
};

int
main ()
{
  //cout<<"Hello World";


  cout << Check ("amir", "??ir") << endl;

  return 0;
}