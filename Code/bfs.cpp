
void bfs(BSN* t)
{
	if (!t) return 0;

    print t->data;

	bfs(t->left);
    bfs(t->right);
}

void dfs(BSN* t)
{
	list<T*> l;

    l.push_back(t);

    while(l.empty){
        t = l.pop_first();
        
        print t->data;
        
        push_back(t->left)
        push_back(t->right)
    }
}
