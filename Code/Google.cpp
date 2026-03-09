void DupA(char* r, char* w) {
    while(r != w) {
        *w = *r;
        
        if (*r == 'a') {
            w--;
            *w == 'a';
        }
        
        w--;
        r--;
    }
}

int RemoveB(char* r, char* w, char* end) {
    while(r != end) {
        if (*r == 'b') {
            r++;
            continue;
        }
        *w = *r;

        r++;
        w++;
    }

    return r - w;
}

void DupA_RemoveB(char* s, int len, int n) {

    len = len - RemoveB(s,s, s + len);
    int need_len = len + countA(str, len);
    
    if(need_len > n){
        return; //error
    }

    DupA(s + len, s + need_len);
}
/////
void deleteNode(T* t,  list<T*>& ret, predicate_t predicate) {
    
    if(predicate(t) ) {
        if (t->parent && t->parent->left == t )  t->parent->left = null;
        if (t->parent && t->parent->right == t )  t->parent->right = null;
        delete t;
    }
    else if (t->parent == null) {
        ret.push_back(t);
    }
}

void scan(T* t, predicate_t predicate) {
    if(t) {
        scan(t->left);
        scan(t->right);

        deleteNode(t,ret,predicate);
    }
} 

list<T*> RemoveNodes(T* t, predicate_t predicate) {
    list<T*> ret;

    queue<T*> q;
    q.push_back(t);

    while(!q.empty()) {
        T* t = q.front();
        q.pop_front();

        if(t) {
            q.push_back(t->right);
            q.push_back(t->left);

            deleteNode(t,ret,predicate);
        }
    }

    return ret;
} 


////
T* findNodeInFullTree(T* t, int i) {
    if(i == 1 ) 
        return t;
    
    if(t == null)
        return null;

    return findNodeInFullTree(i & 1 ? t->right : t->left, i >> 1);
} 
 
T* findMaxIndexNodeInFullTree(T* t) {
    int h = h();

    int a = h << 1;
    int b = (h+1) << 1;

    while (a < b) {
        int m = (a+b)/2;
        
        if(findNodeInFullTree(t,m)) 
            a = m + 1;
        else 
            b = m;
    }

    return a;
} 
/////
bigint findBigInt(f_t f){
    bigint a = 1;
    bigint b = 1;

    while(is_up(b,f)) {
        a = b;
        b *= 2;
    }

    while (a < b) {
        bigint m = (a+b) / 2;
        
        if(is_up(m,f)) 
            a = m + 1;
        else 
            b = m;
    }

    return a;
}

//

vector<vector<int>> three2four(vector<vector<int>>& v){
    for(auto)
}