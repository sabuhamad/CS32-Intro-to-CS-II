#ifndef WORDTREE_H
#define WORDTREE_H

#include <iostream>
#include <string>

typedef std::string WordType;

struct WordNode {
    WordType m_data;
    WordNode* m_left;
    WordNode* m_right;
    int num;
    WordNode(const WordType& myData) {
        m_data = myData;
        m_left = m_right = NULL;
        num = 1;
        
    }
    // You may add additional data members and a constructor
    // in WordNode
};
class WordTree {
private:
    WordNode* root;
    
    void destruct(WordNode *&node);
    void assignTree(WordNode *&node, WordNode *otherNode);
    void display(WordNode *node, std::ostream& output) const;
    int totalWords(WordNode *node) const;
    int distinctWords(WordNode *node) const;

public:
    // default constructor
    WordTree() : root(nullptr) { };
    // copy constructor
    WordTree(const WordTree& rhs);
    // assignment operator
    const WordTree& operator=(const WordTree& rhs);
    // Inserts v into the WordTree
    void add(WordType V);
    // Returns the number of distinct words / nodes

    int distinctWords() const;

    // Returns the total number of words inserted, including
    // duplicate values
    int totalWords() const;

    // Prints the WordTree
    friend std::ostream& operator<<(std::ostream& out, const
        WordTree& rhs);

    // Destroys all the dynamically allocated memory in the
    // tree
    ~WordTree();
};


#endif // WORDTREE_H

