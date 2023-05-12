#include "WeddingGuest.h"
#include <iostream>
#include <cassert>
using namespace std;
 
 
int main() {
    // default constructor
       WeddingGuest lal;
       lal.inviteGuest("Billy", "Bob", 32); //add one guest
       WeddingGuest lal2(lal);
       string first,last;
    int value;
       assert(lal2.guestCount() == 1 && lal2.verifyGuestOnTheList(0, first, last, value) && first == "Billy" && last == "Bob" && value == 32); //tests if values were passed correctly to the new list
       assert(lal.guestCount() == 1 && lal.verifyGuestOnTheList(0, first, last, value) && first == "Billy" && last == "Bob" && value == 32); //tests if values are still in original list

} 
