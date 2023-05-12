//
//  list.cpp
//  hw1
//
//  Created by Sari Abu-Hamad on 7/8/22.
//
#include "LinkedList.h"
#include <iostream>

// default constructor
LinkedList::LinkedList()
{
    head = nullptr;
}
       
// copy constructor
LinkedList::LinkedList(const LinkedList& rhs)
{
    if(rhs.head == nullptr)
    {
        head = nullptr;
        return;
    }
    Node*p;
    p = rhs.head;
    Node*q = new Node;
    head = q;
    q->value = p->value;
    while (p->next != nullptr)
    {
        p = p->next;
        Node* n = new Node;
        q->next = n;
        q = q->next;
        n->value = p->value;
        if(p->next == nullptr)
        {
            n->next = nullptr;
            break;
        }
        
    }
}
       
// Destroys all the dynamically allocated memory
// in the list.
LinkedList::~LinkedList()
{
    Node* p;
    p = head;
    while (p != nullptr)
    {
        Node *n;
        n = p->next;
        delete p;
        p = n;
    }
}
       
// assignment operator
const LinkedList& LinkedList::operator=(const LinkedList& rhs)
{
    if(head == rhs.head)
        return *this;
    delete this->head;
    head = new Node;
    Node*p;
    p = head;
    Node*q;
    q = rhs.head;
    while(q != nullptr)
    {
        p->value = q->value;
        Node*n = new Node;
        p->next = n;
        p = p->next;
        q = q->next;

    }
    return *this;
}

// Inserts val at the front of the list
void LinkedList::insertToFront(const ItemType &val)
{
    Node *p;
    p = new Node;
    p->value = val;
    p->next = head;
    head = p;
}
       
// Prints the LinkedList
void LinkedList::printList() const
{
    if(head == nullptr)
    {
        return;
    }
    Node *p;
    p = head;
    while(p != nullptr)
    {
        std::cout << p->value << " ";
        p = p->next;
    }
    std::cout << std::endl;
}
       
// Sets item to the value at position i in this
// LinkedList and return true, returns false if
// there is no element i
bool LinkedList::get(int i, ItemType& item) const
{
    if(head == nullptr)
        return false;
    Node *p;
    p = head;
    int j = 0;
    while(j != i)
    {
        if(p->next == nullptr)
            return false;
        p = p->next;
        j++;
    }
    item = p->value;
    return true;
    
}
// Reverses the LinkedList
void LinkedList::reverseList()
{
    if(head == nullptr || head->next == nullptr)
    {
        return;
    }
    Node* p;
    Node* after;
    Node* temp;
    p = head;
    after = head->next;
    p->next = nullptr;
    temp = after->next;
    while(after != nullptr)
    {
        if(temp == nullptr)
        {
            head = after;
            head->next = p;
            break;
        }
        temp = after->next;
        after->next = p;
        if(temp != nullptr)
        {
            p = after;
            after = temp;
        }
    }
}
          
   // Prints the LinkedList in reverse order
void LinkedList::printReverse() const
{
    if (head == nullptr)
    {
        return;
    }
    Node* q = head;
    int count = size() - 1;
    bool swapElements = true;
    while (swapElements)
    {
        swapElements = false;
        for (int i = 0; i < count; i++)
        {
            q = q->next;
            swapElements = true;
        }
        std::cout << q->value << " ";
        count--;
        q = head;
    }
    std::cout << std::endl;
}
          
   // Appends the values of other onto the end of this
   // LinkedList.
void LinkedList::append(const LinkedList &other)
{
    if(other.head == nullptr)
    {
        return;
    }
    Node* p;
    p = head;
    if(p != nullptr)
    {
        while(p->next != nullptr)
        {
            p = p->next;
        }
    }
    
    Node* q;
    q = other.head;
    while(q != nullptr)
    {
        Node* a = new Node();
        a->value = q->value;
        p->next = a;
        p = p->next;
        q = q->next;
        if(q == nullptr)
        {
            a->next = nullptr;
            return;
        }
    }
}
   // Exchange the contents of this LinkedList with the other
   // one.
void LinkedList::swap(LinkedList &other)
{
    int thisSize = this->size(), otherSize = other.size();
    if(thisSize > otherSize)
    {
        if(otherSize == 0 )
        {
            Node*e;
            e = new Node;
            other.head = e;
            e->value = head->value;
            Node*f;
            f = head->next;
            int y = 0;
            while(y < thisSize)
            {
                Node*n;
                n = new Node;
                e->next = n;
                n->value = f->value;
                f = f->next;
                if(f == nullptr)
                {
                    n->next = nullptr;
                    break;
                }
                e = e->next;
                y++;
            }
            Node*temp;
            temp = head;
            head = nullptr;
            int p = 0;
            while (p < thisSize)
            {
                if(!(temp->next == nullptr))
                {
                    Node*temp2;
                    temp2 = temp->next;
                    delete temp;
                    temp = temp2;
                }
                else
                {
                    delete temp;
                }
                p++;
            }
            return;
        }
        LinkedList* tempList;
        tempList = new LinkedList(*this);
        int i = 0;
        Node*p;
        p = head;
        Node*q;
        q = other.head;
        while (i < otherSize)
        {
            p->value = q->value;
            q = q->next;
            if(i + 1 != otherSize)
                p = p->next;
            i++;
        }
        Node*temp;
        temp = p->next;
        p->next = nullptr;
        while (i < thisSize)
        {
            if(!(temp->next == nullptr))
            {
                Node*temp2;
                temp2 = temp->next;
                delete temp;
                temp = temp2;
            }
            else
            {
                delete temp;
            }
            i++;
        }
        int j = 0;
        Node*x;
        x = other.head;
        Node*z;
        z = tempList->head;
        while (j < otherSize)
        {
            x->value = z->value;
            z = z->next;
            if(x->next != nullptr)
                x = x->next;
            j++;
        }
        while (otherSize < thisSize)
        {
            Node*n;
            n = new Node;
            x->next = n;
            n->value = z->value;
            z = z->next;
            x = x->next;
            otherSize++;
        }
        x->next = nullptr;
        delete tempList;
    }
    else
    {
        if(thisSize == 0 )
        {
            if(otherSize == 0)
            {
                return;
            }
            Node*e;
            e = new Node;
            head = e;
            e->value = other.head->value;
            Node*f;
            f = other.head->next;
            int y = 0;
            while(y < otherSize)
            {
                Node*n;
                n = new Node;
                e->next = n;
                n->value = f->value;
                f = f->next;
                if(f == nullptr)
                {
                    n->next = nullptr;
                    break;
                }
                e = e->next;
                y++;
            }
            Node*temp;
            temp = other.head;
            other.head = nullptr;
            int p = 0;
            while (p < otherSize)
            {
                if(!(temp->next == nullptr))
                {
                    Node*temp2;
                    temp2 = temp->next;
                    delete temp;
                    temp = temp2;
                }
                else
                {
                    delete temp;
                }
                p++;
            }
            return;
        }
        LinkedList* tempList2;
        tempList2 = new LinkedList(other);
        int i = 0;
        Node*p;
        p = other.head;
        Node*q;
        q = head;
        while (i < thisSize)
        {
            p->value = q->value;
            q = q->next;
            if(i + 1 != thisSize)
                p = p->next;
            i++;
        }
        Node*temp;
        temp = p->next;
        p->next = nullptr;
        while (i < otherSize)
        {
            if(!(temp->next == nullptr))
            {
                Node*temp2;
                temp2 = temp->next;
                delete temp;
                temp = temp2;
            }
            else
            {
                delete temp;
            }
            i++;
        }
        int j = 0;
        Node*x;
        x = head;
        Node*z;
        z = tempList2->head;
        while (j < thisSize)
        {
            x->value = z->value;
            z = z->next;
            if(x->next != nullptr)
                x = x->next;
            j++;
        }
        while (thisSize < otherSize)
        {
            Node*n;
            n = new Node;
            x->next = n;
            n->value = z->value;
            z = z->next;
            x = x->next;
            thisSize++;
        }
        x->next = nullptr;
        delete tempList2;
    }
}
          
   // Returns the number of items in the Linked List.
int LinkedList::size() const
{
    if(head == nullptr)
    {
        return 0;
    }
    Node* p;
    p = head;
    int i = 0;
    while (p != nullptr)
    {
        p = p->next;
        i++;
    }
    return i;
}


