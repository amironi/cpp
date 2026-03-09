// This is the text editor interface. 
// Anything you type or change here will be seen by the other person in real time.

#include <stdio.h>


void removeSubStr(char *a, const char *b)
{
    //find length of b

    if (!a || !b)
        return;
 
    const char* pb = b;
    
    while (*pb++);

    int length_b = pb-b-1;
    
    printf("%d\n",length_b);
    
   
    char* r = a;
    char* w = a;
    pb = b;
  
    while(*r){
        
        *w = *r;

        if(*pb == *r)
            pb++;
        else 
            pb = b;
        
        if(pb == b + length_b){
            w -= length_b-1;
            pb = b;
        }
        else
            w++;
            
        r++;
        
    }
    
    *w = 0;
    
    return;
}

int main() {
    char a1[100] = "this that there";
    char b1[100] = "th";

    removeSubStr(a1, b1);

    printf("%s\n",a1); // should print "is at ere"

    return 0;
}
