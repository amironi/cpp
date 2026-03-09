#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#define N 100

int *p;
int *f;

char Mem[2*1024];// = new int(N);

void init()
{
    p = Mem;
    f = NULL;
    
}

struct Node
{   
    short _size = 2*1024;
    char* data=mem;
    Node* _next;
};

char* malloc(int size)
{
    
}

int* alloc()
{
    if (p < Mem + N ) 
        return p++;
    
    if (f) 
    {
            int* ret=f;
            f = (int*)*f;
            return ret;
        
    }
    
    return NULL;
    
}

void relese(int* i)
{
    *i = (long)f;
    f = i;
}

int
main ()
{
    
    init();
    int* last;
  //cout<<"Hello World";
    for( int i = 0 ; i <  N; i++)
        cout << i <<": "<< ( last = alloc() )<< endl;
    
    assert( alloc() == 0);
    
    relese(last);
    
    cout << "after free: "<< (last = alloc()) << endl;
    
    assert( alloc() == 0);
 
  return 0;
}



Allocator::Allocator(size_t size, UINT objects, CHAR* memory, const CHAR* name) :
    m_blockSize(size < sizeof(long*) ? sizeof(long*):size),
    m_objectSize(size),
    m_maxObjects(objects),
    m_pHead(NULL),
    m_poolIndex(0),
    m_blockCnt(0),
    m_blocksInUse(0),
    m_allocations(0),
    m_deallocations(0),
    m_name(name)
{
    // If using a fixed memory pool 
	if (m_maxObjects)
	{
		// If caller provided an external memory pool
		if (memory)
		{
			m_pPool = memory;
			m_allocatorMode = STATIC_POOL;
		}
		else 
		{
			m_pPool = (CHAR*)new CHAR[m_blockSize * m_maxObjects];
			m_allocatorMode = HEAP_POOL;
		}
	}
	else
		m_allocatorMode = HEAP_BLOCKS;
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
Allocator::~Allocator()
{
	// If using pool then destroy it, otherwise traverse free-list and 
	// destroy each individual block
	if (m_allocatorMode == HEAP_POOL)
		delete [] m_pPool;
	else if (m_allocatorMode == HEAP_BLOCKS)
	{
		while(m_pHead)
			delete [] (CHAR*)Pop();
	}
}


char m_pPool[2*1024];// = new int(N);


struct Block {
    Block* pNext;
};

Block *m_pHead = NULL;


void* Allocate(size_t size)
{
    assert(size <= m_objectSize);
	
    void* pBlock = NULL;

    if (m_pHead) {
        pBlock = m_pHead;
        m_pHead = m_pHead->pNext;
    }

    // If can't obtain existing block then get a new one
    if (!pBlock)
    {
        // If using a pool method then get block from pool,
        // otherwise using dynamic so get block from heap
        if (m_maxObjects)
        {
            // If we have not exceeded the pool maximum
            if(m_poolIndex < m_maxObjects)
            {
                pBlock = (void*)(m_pPool + (m_poolIndex++ * m_blockSize));
            }
        }
        else
        {
        	m_blockCnt++;
            pBlock = (void*)new CHAR[m_blockSize];
        }
    }
	
    return pBlock;
}

void Free(void* pMemory) {
    Block* pBlock = (Block*)pMemory;
    pBlock->pNext = m_pHead;
    m_pHead = pBlock;
}



