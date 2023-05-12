#include "WordTree.h"

using namespace std;

WordTree::~WordTree() {
     destruct(root);
 }

void WordTree::add(WordType V)
{
    if (root == NULL)
    {
        root = new WordNode(V);   return;
    }
    WordNode* thisNode = root;
    for (;;)
    {
        if (V == thisNode->m_data)
        {
            thisNode->num += 1;
            return;
        }

        if (V < thisNode->m_data)
        {
            if (thisNode->m_left != NULL)
            {
                thisNode = thisNode->m_left;
            }
            else
            {
                thisNode->m_left = new WordNode(V);
                return;
            }
        }
        else if (V > thisNode->m_data)
        {
            if (thisNode->m_right != NULL)
            {
                thisNode = thisNode->m_right;
            }
            else
            {
                thisNode->m_right = new WordNode(V);
                return;
            }
        }
    }
}

int WordTree::distinctWords(WordNode *node) const
{
    if(node == nullptr) 
        return 0;
    else
        return 1 + distinctWords(node->m_left) + distinctWords(node->m_right);
}

// Returns the number of distinct words / nodes
int WordTree:: distinctWords() const
{
    return distinctWords(root);
}


int WordTree::totalWords(WordNode *node) const
{
    if(node == nullptr) // empty node, return 0
        return 0;
    else
        return node->num + totalWords(node->m_left) + totalWords(node->m_right);
}

int WordTree:: totalWords() const
{
    return totalWords(root);
}

void WordTree::display(WordNode *node, std::ostream& output) const
{
    if(node != nullptr)
    {
        display(node->m_left, output);
        output << node->m_data << " " << node->num << std::endl;
        display(node->m_right, output);
    }
}


std::ostream& operator<<(std::ostream& out, const WordTree& rhs)
{
     rhs.display(rhs.root, out);
     return out;
}

WordTree::WordTree(const WordTree& rhs)
{
    if(rhs.root == nullptr)
        root = nullptr;
    else
        assignTree(root, rhs.root);
}
 
const WordTree& WordTree::operator=(const WordTree& rhs)
{
    if(this != &rhs)
    {
        if(root != nullptr)
        {
            destruct(root);
            if(rhs.root == nullptr)
                root = nullptr;
            else
                assignTree(root, rhs.root);
        }
        else if(root == nullptr)
        {
            if(rhs.root == nullptr)
            {
                
            }
            else
            {
                assignTree(root, rhs.root);
            }
        }
    }
    return *this;
}

void WordTree:: destruct(WordNode *&node)
{
    if(node != nullptr)
        {
            destruct(node->m_left);
            destruct(node->m_right);
            delete node;
            node = nullptr;
        }
}

void WordTree:: assignTree(WordNode *&node, WordNode *otherNode)
{
    if(otherNode == nullptr)
    {
        node = nullptr;
    }
    else
    {
        node = new WordNode(otherNode->m_data);
        node->num = otherNode->num;
        assignTree(node->m_left, otherNode->m_left);
        assignTree(node->m_right, otherNode->m_right);
    }
}




