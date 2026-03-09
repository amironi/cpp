public class Queue<E>
{

    private Stack<E> inbox = new Stack<E>();
    private Stack<E> outbox = new Stack<E>();

    public void queue(E item) {
        inbox.push(item);
    }

    public E dequeue() {
        if (outbox.isEmpty()) {
            while (!inbox.isEmpty()) {
               outbox.push(inbox.pop());
            }
        }
        return outbox.pop();
    }

}

struct Node
 {
    Node* next;
    Node* data;     
 }; 


Node* c(Node* n)
{
    Node* r = new Node;

    while(n->next)
    {
        r->next = new Node;
        
        n = n->next;
        r = r->next;
    }

    return r;
}