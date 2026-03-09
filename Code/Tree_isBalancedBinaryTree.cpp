#include <iostream>
using namespace std;

int _min = MAX_INT;
int _max = 0;

int getMinDepth()
{
   if(!t) return 0;
  
   return 1 + min(getMinDepth(t->right), getMinDepth(t->left));
}

int getMaxDepth()
{
   if(!t) return 0;
  
   return 1 + max(getMaxDepth(t->right), getMaxDepth(t->left));
}
bool isBalancedBinaryTree(Tree* t)
{
  return ( getMaxDepth(t) - getMinDepth(t) ) <= 1;
}