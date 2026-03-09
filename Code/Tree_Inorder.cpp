
/* Given a binary tree, print its nodes in inorder*/
void printInorder(struct node* node)
{
     if (node == NULL) return;
     printInorder(node->left);
     printf("%d ", node->data);  
     printInorder(node->right);
}
 

 /* Iterative function for inorder tree traversal */
void printInorder(struct tNode *t)
{
  while (1)
  {
    /* Reach the left most tNode of the current tNode */
    if(t !=  NULL)
    {
      /* place pointer to a tree node on the stack before traversing 
        the node's left subtree */
     	s.push( t);                                               
      t = t->left;  
    }
    else                                                             
    {
      if (!s.isEmpty())
      {
        t = s.pop();
        
        printf("%d ", t->data);
 
        t = t->right;
      }
      else
        return; 
    }
  } 
}     


node * mostLeft( node *n)
{
  if (!n->left) return n;

  return mostLeft( n->left );
}

node * getleftParent( node *n)
{
  if (!n) return null;

  if (n == n->parent->left ) return n->parent;

  return getleftParent( n->parent );
}

node * inOrderSuccessor(node *root, node *n)
{
  if( n->right != NULL )
    return mostLeft(n->right);
 
  return getleftParent(n);

  //while(n->parent && n != n->parent->left) n = n->parent;

  //return n->parent;
}
void write(T* t, file)
{
  if (!t) file << -1 ; return;

  file << t->data;

  write(t->left);
  write(t->right);
}

T* read( int[] file, i, j)
{
  if (j>i) return;

  int m = i+j/2;
  
  T* t = new T(file[n]);
  
  t->left = read( file, i, m-1);
  t->right =read( file, m+1, j);
  
  return t;
}

T* read( file)
{
  file >> t->X;

  if t->X == -1 return;

  T* t = new T();
 
  t->left = read( file );
  t->right = read( file );
  
  return t;
}

int insert( T* t, int data)
{
  if (t->data == data) return;

  if (t->data > data) {
    if (t->left) 
      return insert(t->left,data);
    
    t->left = new T(data);
    
    return;
  }
  
  if (t->right) 
    return insert(t->right,data);
  
  t->right = new T(data);
}

int find( int* s, int* e, int x)
{
  while (s+1<e) {

    int* m = (s+e)/2;
    
    if x == *m return m;
    
    if x < *m 
      e = m - 1;
    else      
      s = m + 1;
  }
 
  return *;
}

int find( int* s, int* e)
{
  if (s+1>=e) 
    return max(*s,*e);
  
  int* m = (s+e)/2;
    
  if isLocalMax(s,e) 
      return *m;
    
  if  (*m-1) < *m 
    e = m - 1;
  else      
    s = m + 1;
}

int find( T* t, int x, prev = -1)
{
  if (!t) return prev;

  if x == t->X return m;

  if x < t->X 
    return find(t->left, x, t->X);
  
  find(t->rigth, x, t->X);
}

int localMinUtil(int arr[], int low, int high, int n)
{
    int mid = (high + low)/2;  /* (low + high)/2 */
 
    if ((mid == 0 || arr[mid-1] > arr[mid]) &&
            (mid == n-1 || arr[mid+1] > arr[mid]))
        return mid;
 
    else if (mid > 0 && arr[mid-1] < arr[mid])
        return localMinUtil(arr, low, (mid -1), n);
 
    return localMinUtil(arr, (mid + 1), high, n);
}