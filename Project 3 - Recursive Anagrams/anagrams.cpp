#include <iostream>
#include <fstream>
#include <istream>
#include <cstring>
using namespace std;

const int MAXRESULTS   = 20;    // Max matches that can be found
const int MAXDICTWORDS = 30000; // Max words that can be read in

int lexiconBuilder(istream &dictfile, string dict[])
//Puts each string in dictfile into the array dict. Returns the number of words
//read into dict. This number should not be larger than MAXDICTWORDS since that is
//the size of the array.
{
    string line;
    int count = 0;
    if(getline (dictfile,line))
    {
        count = count + lexiconBuilder(dictfile, dict); //to keep track of which element in dict should be referred to
        dict[count] = line;
        count++;
    }
    else
    {
        count = 0;
    }
    return count;
}

string checkNextLetter(string word) //rotates passed word over by one letter
{
    if (word.size() > 0)
        return word.substr(1, word.size()) + word[0];
    else return word;
}

bool checkAnagram(string word, string dictionaryWord, bool &found, int counter) //checks if the words passed are anagrams
{
    if (counter > word.size()) //the word is not an anagram and the function has shifted the dictionary word back to the original position
    {
        counter = 0;
        found = false;
        return false;
    }
    
    if (word.size() == 0) //this means the dictionary word is an anagram as there are no letters left
    {
        counter = 0;
        found = true;
        return true;
    }
    
    if (word[0] == dictionaryWord[0]) //if the first letter is the same then delete it from each and compare the remaining letters
    {
        word = word.substr(1, word.size() - 1);
        dictionaryWord = dictionaryWord.substr(1, dictionaryWord.size() - 1);
        checkAnagram(word, dictionaryWord, found, 0);
    }
    
    else //if not then shift the dictionary word and repeat
    {
        dictionaryWord = checkNextLetter(dictionaryWord);
        found = checkAnagram(word, dictionaryWord, found, counter + 1);
    }
    if(found)
        return true;
    return false;
}

int theJumbler(string word, const string dict[], int size, string results[])
{
    if(size <= 0)
        return 0;
    int count = 0;
    bool match = false;
    string dictionaryWord = dict[size - 1]; //take the last word from the dictionary
    if (word.length() == dictionaryWord.length() && checkAnagram(word, dictionaryWord, match, 0)) //if the words are the same length and match then increment count and add to results
    {
        count = count + theJumbler(word, dict, size - 1, results);
        results[count] = dictionaryWord;
        count++;
    }
    else //if not keep recursively going through the function
        count = count + theJumbler(word, dict, size - 1, results);
    return count;
}

void divulgeSolutions(const string results[], int size)
{
    if(size == 0)
        return;
    else
    {
        cout << "Matching word " << results[size - 1] << endl; //displays results while results still has elements
        divulgeSolutions(results, size -1);
    }
}

int main()
{
    return 0;
}
