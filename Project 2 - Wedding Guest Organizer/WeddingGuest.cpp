//
//  WeddingGuest.cpp
//  weddingProject
//
//  Created by Sari Abu-Hamad on 7/14/22.
//

#include "WeddingGuest.h"

WeddingGuest::WeddingGuest()       // Create an empty WeddingGuest list
{
    head = nullptr;
    tail = nullptr;
    m_size = 0;
}
WeddingGuest::~WeddingGuest()
{
    if (m_size == 0)
            return; //return if list contains no elements
    if (m_size == 1) {
        delete head; //if list is 1 element big, destroy head and return
        return;
    }
    Node* p;
    p = head;
    while (p->next != nullptr) //loop through list while deleting previous
    {
        p = p->next;
        delete p->prev;
    }
    delete tail; //delete tail to account for last Node
}
WeddingGuest::WeddingGuest(const WeddingGuest& other)
{
    int i = 0; //initialize counter
    Node* q;
    q = head;
    Node* p;
    for (p = other.head; p != nullptr; p = p->next) //loop through the other list until reach nullptr
    {

        if (i == 0) //if counter is zero, initialize the list with a head and then increment counter
        {
            head = new Node;
            head->type = other.head->type;
            head->first = other.head->first;
            head->last = other.head->last;
            head->prev = nullptr;
            head->next = nullptr;
            i++;
            q = head;
            if (other.m_size == 1)
                tail = q;
            m_size++;
        }
        else //if counter is not zero, add a new node and increment size count variable
        {
            Node* n = new Node;
            n->type = p->type;
            q->next = n;
            n->prev = q;
            q = q->next;
            i++;
            m_size++;
        }
    }
    if (i == other.m_size) //if i is the size of the other list, it has fullly looped through so the Node used to loop through this list is the tail
    {
        q->next = nullptr;
        tail = q;
    }
    if (other.m_size == 0) //if the other list is size 0, initialize this list like in the default constructor
    {
        head = nullptr;
        tail = nullptr;
        m_size = 0;
    }
}

const WeddingGuest& WeddingGuest::operator=(const WeddingGuest& rhs)
{
    if (this != &rhs) //prevent aliasing
        {
            if (m_size == 0) //if the size of the current list is 0, do nothing
            {
                
            }
            else if (m_size == 1) //if the size of the current list is 1, delete the head
            {
                delete head;
            }
            else //if the size of the current list is > 1, loop through it deleting each previous node and then the tail
            {
                Node* p;
                p = head;
                while (p->next != nullptr)
                {
                    p = p->next;
                    delete p->prev;
                }
                delete tail;
            }
            if (rhs.head == nullptr) //if the other list's head is null, initialize this list like the default constructor then return it
            {
                head = nullptr;
                tail = nullptr;
                m_size = 0;
                return *this;
            }
            else //if the other list's head is not null, copy the values of the other list's head into the current head and increment size counter
            {
                head = new Node;
                tail = head;
                head->first = rhs.head->first;
                head->last = rhs.head->last;
                head->type = rhs.head->type;
                head->prev = nullptr;
                m_size = 1;
            }
            Node*p;
            p = rhs.head->next; //go to the next value in the other list
            for (int i = 1; i < rhs.m_size; i++) //loop through the other list until all values are copied into the new list
            {
                Node*n = new Node;
                n->first = p->first;
                n->last = p->last;
                n->type = p->type;
                tail->next = n;
                n->prev = tail;
                tail = n;
                tail->next = nullptr;
                m_size++;
                p = p->next;
            }
        }
        return *this; //return this list
}

bool WeddingGuest::noGuests() const  // Return true if the WeddingGuest list
                           // is empty, otherwise false.
{
    if (m_size == 0)
        return true;
    return false;
}

int WeddingGuest::guestCount() const  // Return the number of matches
                             // on the WeddingGuest list.
{
    return m_size;
}

bool WeddingGuest::inviteGuest(const std::string& firstName, const std::string&
           lastName, const GuestType& value)
  // If the full name (both the first and last name) is not equal
  // to any full name currently in the list then add it and return
  // true. Elements should be added according to their last name.
  // Elements with the same last name should be added according to
  // their first names. Otherwise, make no change to the list and
  // return false (indicating that the name is already in the
  // list).
{
    if(head == nullptr) //if this list is null, add a new guest, increment size, and then return true
    {
        Node*n = new Node;
        head = n;
        n->first = firstName;
        n->last = lastName;
        n->type = value;
        n->next = nullptr;
        n->prev = nullptr;
        head = n;
        tail = n;
        m_size++;
        return true;
    }
    if(head->next == nullptr) //if the next value is null
    {
        if(head->first == firstName && head->last == lastName) //return false if the values of head are = to the passed first and last names
        {
            return false;
        }
       else if((lastName < head->last) || (head->last == lastName && firstName < head->first)) //if they are not the same, add a new value in the correct alphabetical order and increment m_size
       {
           Node*n = new Node;
           n->first = firstName;
           n->last = lastName;
           n->type = value;
           n->next = head;
           n->prev = nullptr;
           head = n;
           n->next->prev = n;
           m_size++;
           return true;
       }
        else
        {
            Node*n = new Node;
            head->next = n;
            n->prev = head;
            n->first = firstName;
            n->last = lastName;
            n->type = value;
            n->next = nullptr;
            tail = n;
            m_size++;
            return true;
        }
    }
    if(lastName < head->last) //if the last name should be earlier alphabetically than head's last name, add it before
    {
        Node*n = new Node;
        n->first = firstName;
        n->last = lastName;
        n->type = value;
        n->next = head;
        n->prev = nullptr;
        head = n;
        n->next->prev = n;
        m_size++;
        return true;
    }
    Node*p;
    p = head;
    while (p != nullptr) //loop through the existing list, and if the current first and last name are found return false
    {
        if(p->last == lastName && p->first == firstName)
        {
            return false;
        }
        p = p->next;
    }
    p = head;
    while(lastName > p->last) //loop through the list again until the value for last name should come after the value in the list
    {
        p = p->next;
        if(p->next == nullptr && (lastName > p->last || (p->last == lastName && firstName > p->first))) //if the next value is going to be nullptr, it will be reaching the end and thus need to check if the lastName should go before or after the end
        {
            Node*n = new Node;
            p->next = n;
            n->next = nullptr;
            n->prev = p;
            n->first = firstName;
            n->last = lastName;
            n->type = value;
            tail = n;
            m_size++;
            return true;
        }
        else if(p->next == nullptr && (lastName < p->last || (lastName == p->last && firstName < p->first)))
        {
            Node*n = new Node;
            n->prev = p->prev;
            n->prev->next = n;
            n->next = p;
            n->first = firstName;
            n->last = lastName;
            n->type = value;
            p->prev = n;
            m_size++;
            return true;
        }
    }
    if(p->last == lastName) //if the node stopped at has the same last name, check first name
    {
        while(firstName > p->first && p->last == lastName) //loop through the elements while the last name is the same and the firstName is earlier in the alphabetical order
        {
            p = p->next;
            if(p->next == nullptr && (lastName > p->last || (p->last == lastName && firstName > p->first))) //if the next value is going to be nullptr, it will be reaching the end and thus need to check if the firstName should go before or after the end
            {
                Node*n = new Node;
                p->next = n;
                n->next = nullptr;
                n->prev = p;
                n->first = firstName;
                n->last = lastName;
                n->type = value;
                tail = n;
                m_size++;
                return true;
            }
            else if(p->next == nullptr && (lastName < p->last || (lastName == p->last && firstName < p->first)))
            {
                Node*n = new Node;
                n->prev = p->prev;
                n->prev->next = n;
                n->next = p;
                n->first = firstName;
                n->last = lastName;
                n->type = value;
                p->prev = n;
                m_size++;
                return true;
            }
        }
    }
    if(p->prev == nullptr) //check if the element is currently at the head and the new guest has to be added before the head
    {
        Node*n = new Node;
        n->first = firstName;
        n->last = lastName;
        n->type = value;
        n->next = head;
        n->prev = nullptr;
        head = n;
        n->next->prev = n;
        m_size++;
        return true;
    }
    //add a node in the middle of the list
    Node* n = new Node;
    n->first = firstName;
    n->last = lastName;
    n->type = value;
    n->prev = p->prev;
    n->next = p;
    p->prev->next = n;
    p->prev = n;
    m_size++;
    return true;
}

bool WeddingGuest::alterGuest(const std::string& firstName, const std::string&
lastName, const GuestType & value)
  // If the full name is equal to a full name currently in the
  // list, then make that full name no longer map to the value it
  // currently maps to, but instead map to the value of the third
  // parameter; return true in this case. Otherwise, make no
  // change to the list and return false.
{
    Node*p;
    p = head;
    while(p != nullptr) //loop through list
    {
        if(p->first == firstName && p->last == lastName) //if full name is found set the new value to the one passed and return true
        {
            p->type = value;
            return true;
        }
        p = p->next;
    }
    return false; //if not found return false
}

bool WeddingGuest::inviteOrAlter(const std::string& firstName, const
std::string& lastName, const GuestType& value)
  // If full name is equal to a name currently in the list, then
  // make that full name no longer map to the value it currently
  // maps to, but instead map to the value of the third parameter;
  // return true in this case. If the full name is not equal to
  // any full name currently in the list then add it and return
  // true. In fact, this function always returns true.
{
    Node*p;
    p = head;
    while(p != nullptr) //loop through list
    {
        if(p->first == firstName && p->last == lastName) //if full name is found set the new value to the one passed and return true
        {
            p->type = value;
            return true;
        }
        p = p->next;
    }
    inviteGuest(firstName, lastName, value); //if not found, add it and return true
        return true;
}

bool WeddingGuest::crossGuestOff(const std::string& firstName, const
std::string& lastName)
  // If the full name is equal to a full name currently in the
  // list, remove the full name and value from the list and return
  // true.  Otherwise, make no change to the list and return
  // false.
{
    if(m_size == 0)
    {
        return false;
    }
    Node*p;
    p = head;
    if(m_size == 1)
    {
        if(head->first == firstName && head->last == lastName)
        {
            delete head;
            m_size = 0;
            return true;
        }
    }
    while(p != nullptr) //loop through list
    {
        if(p->first == firstName && p->last == lastName) //if guest is found
        {
            if(p->prev == nullptr) //check if guest is at head to account for out of bounds errors
            {
                head = p->next;
                head->prev = nullptr;
                m_size--;
                delete p;
                return true;
            }
            if(p->next == nullptr) //check if guest is at tail to account for out of bounds errors
            {
                tail = p->prev;
                tail->next = nullptr;
                m_size--;
                delete p;
                return true;
            }
            p->prev->next = p->next;  //if in the middle, account for getting rid of p
            p->next->prev = p->prev;
            m_size--;
            delete p;
            return true;
        }
        p = p->next;
    }
    return false; //if not found, return false
}

bool WeddingGuest::invitedToTheWedding(const std::string& firstName, const
std::string& lastName) const
  // Return true if the full name is equal to a full name
  // currently in the list, otherwise false.
{
    Node*p;
    p = head;
    while(p != nullptr) //loop through list
    {
        if(p->first == firstName && p->last == lastName) //if found return true else false
        {
            return true;
        }
        p = p->next;
    }
    return false;
}

bool WeddingGuest::matchInvitedGuest(const std::string& firstName, const
std::string& lastName, GuestType& value) const
  // If the full name is equal to a full name currently in the
  // list, set value to the value in the list that that full name
  // maps to, and return true.  Otherwise, make no change to the
  // value parameter of this function and return false.
{
    Node*p;
    p = head;
    while(p != nullptr) //loop through list
    {
        if(p->first == firstName && p->last == lastName) //if found, set value of GuestType passed to the value assigned to the guest in the list and return true
        {
            value = p->type;
            return true;
        }
        p = p->next;
    }
    return false;
}

bool WeddingGuest::verifyGuestOnTheList(int i, std::string& firstName,
std::string& lastName, GuestType & value) const
  // If 0 <= i < size(), copy into firstName, lastName and value
  // parameters the corresponding information of the element at
  // position i in the list and return true.  Otherwise, leave the
  // parameters unchanged and return false. (See below for details
  // about this function.)
{
    if(!(i >= 0 && i < m_size)) //if i is out of bounds, return false
    {
        return false;
    }
    Node* p;
    p = head;
    int count = 0;
    while(count < i) //loop until reach the element of interest
    {
        p = p->next;
        count++;
    }
    firstName = p->first; //collect the values at that element
    lastName = p->last;
    value = p->type;
    return true;
}
       
void WeddingGuest::swapWeddingGuests(WeddingGuest& other)
  // Exchange the contents of this list with the other one.
{
    WeddingGuest temp;
    temp = *this; //assign the values of the temporary list to those of this list
    *this = other; //assign the values of this list to those of the other list
    other = temp;//assign the values of the other list to those of the temporary list
}

bool joinGuests(const WeddingGuest & odOne, const WeddingGuest & odTwo, WeddingGuest & odJoined)
{
    bool noDifferentValues = true; //use bool to track that there are no different values
    if(odOne.noGuests()) //if there are no guests in either list, set the joined list equal to the non empty list
    {
        odJoined = odTwo;
        if(noDifferentValues)
            return true;
        return false;
    }
    else if(odTwo.noGuests())
    {
        odJoined = odOne;
        if(noDifferentValues)
            return true;
        return false;
    }
    odJoined = odOne; //set the joined list equal to the first list
    for(int i = 0; i < odTwo.guestCount(); i++) //loop through the second list capturing each value of the guests
    {
        std::string first;
        std::string last;
        GuestType value;
        odTwo.verifyGuestOnTheList(i, first, last, value);
        GuestType value2;
        if(odOne.matchInvitedGuest(first, last, value2)) //if the guest is already on list 1 capture the GuestType value given on list 1
        {
            if (value2 != value) //if the values are not equal, set the bool to false and cross the guest off the joined list
            {
                noDifferentValues = false;
                odJoined.crossGuestOff(first, last);
            }
        }
        else //if the guest is not on list 1 add it to the joined list using inviteGuest
        {
            odJoined.inviteGuest(first, last, value);
        }
    }
    if(noDifferentValues) //if there are no different values return true else return false
        return true;
    return false;
}

void attestGuests (const std::string& fsearch, const std::string& lsearch, const WeddingGuest& odOne, WeddingGuest& odResult)
{
    if(fsearch == "*" && lsearch == "*") //if both first and last are wildcards, assign values of list one to result list and return
    {
        odResult = odOne;
        return;
    }
    WeddingGuest tempGuest; //create a tempList to store list one values to account for aliasing before setting odResult to an empty list
    tempGuest = odOne;
    WeddingGuest g;
    odResult = g;

    if(fsearch == "*") //if the first search is the wildcard
    {
        for(int i = 0; i < tempGuest.guestCount(); i++) //loop through the list of values from list one
        {
            std::string first;
            std::string last;
            GuestType value;
            tempGuest.verifyGuestOnTheList(i, first, last, value);
            if(lsearch == last) //if the last name is equal to the one being searched for, add it to the result list
            {
                odResult.inviteGuest(first, last, value);
            }
        }
    }
    else if(lsearch == "*") //if the last search is the wildcard
    {
        for(int i = 0; i < tempGuest.guestCount(); i++) //loop through the list of values from list one
        {
            std::string first;
            std::string last;
            GuestType value;
            tempGuest.verifyGuestOnTheList(i, first, last, value);
            if(fsearch == first) //if the first name is equal to the one being searched for, add it to the result list
            {
                odResult.inviteGuest(first, last, value);
            }
        }
    }
    else //if neither first or last are wildcards, check if list 1 contains both fsearch and lsearch, and then add it to the resulting list and return if so
    {
        GuestType p;
        if(tempGuest.matchInvitedGuest(fsearch, lsearch, p))
        {
            odResult.inviteGuest(fsearch, lsearch, p);
        }
        return;
    }
}
