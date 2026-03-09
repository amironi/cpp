
void getMaxDepth(BSN* t)
{
	if (!t) return 0;

	return 1 + max(getMaxDepth(t->mRight), getMaxDepth(t->mLeft) );
}



/* Function to get diameter of a binary tree */
int diameter(node * tree, int& height)
{
   /* base case where tree is empty */
   if (tree == NULL) height = 0; return 0;
 
  /* get the height of left and right sub-trees */
  int lheight = 0; 
  int rheight = 0; 
 
  /* get the diameter of left and right sub-trees */
  int ldiameter = diameter(tree->left, lheight);
  int rdiameter = diameter(tree->right, rheight);
 
  height = 1 + max(rheight, lheight);
  /* Return max of following three
   1) Diameter of left subtree
   2) Diameter of right subtree
   3) Height of left subtree + height of right subtree + 1 */
  return max(lheight + rheight + 1, max(ldiameter, rdiameter));

} 
 

/* Function to get diameter of a binary tree */
int diameter(struct node * tree)
{
   /* base case where tree is empty */
   if (tree == NULL)
     return 0;
 
  /* get the height of left and right sub-trees */
  int lheight = height(tree->left);
  int rheight = height(tree->right);
 
  /* get the diameter of left and right sub-trees */
  int ldiameter = diameter(tree->left);
  int rdiameter = diameter(tree->right);
 
  /* Return max of following three
   1) Diameter of left subtree
   2) Diameter of right subtree
   3) Height of left subtree + height of right subtree + 1 */
  return max(lheight + rheight + 1, max(ldiameter, rdiameter));

} 
 
  
/* Function to find height of a tree */
int height(Node* root, int& ans)
{
    if (root == NULL)
        return 0;
 
    int left_height = height(root->left, ans);
 
    int right_height = height(root->right, ans);
 
    // update the answer, because diameter of a
    // tree is nothing but maximum value of
    // (left_height + right_height + 1) for each node
    ans = max(ans, 1 + left_height + right_height);
 
    return 1 + max(left_height, right_height);
}
 
/* Computes the diameter of binary tree with given root. */
int diameter(Node* root)
{
    if (root == NULL)
        return 0;
    int ans = INT_MIN; // This will store the final answer
    int height_of_tree = height(root, ans);
    return ans;
}
 
int maxPath(BSN* t)
{
	if (!t) return 0;

	return max(
		getMaxDepth(t->mRight) + getMaxDepth(t->mLeft) + 1, 
		maxPath(t->mRight), 
		maxPath(t->mLeft) );
}

main()
{
	maxPath(root);

	cout << "maxLen =" << ml << endl;
}


int countSubtreesWithSumX(Node* root, int x, int count = 0)
{
    if (!root) return 0;

    sum = 
      (countSubtreesWithSumX(root->left, count, x) + 
      countSubtreesWithSumX(root->right, count, x) + 
      root->data);

    if ( sum == x) count++;
 
    return sum;
}


bool subTree(T* r1, T* r2) 
{

 if (r1 == null ) return false;

 if (matchTree(r1,r2)) return true;

 return (subTree(r1.left, r2) || subTree(r1.right, r2));
}

bool matchTree(T* r1, T* r2) {
 
 if (r2 == null && r1 == null) return true;
 if (r1 == null || r2 == null) return false;
 if (r1.data != r2.data)   return false;  

 return matchTree(r1.left, r2.left) && matchTree(r1.right, r2.right);
}