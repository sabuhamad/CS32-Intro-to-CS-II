//
//  main.cpp
//  homework2
//
//  Created by Sari Abu-Hamad on 7/23/22.
//

#include <iostream>
#include <string>
using namespace std;

// Returns the product of two non-negative integers, m and n,
// using only repeated addition.
int multiplyTwoNumbers(unsigned int m, unsigned int n)
{
    int total;
    if(n == 1)
        total = m;
    else
        total = m + multiplyTwoNumbers(m, n-1);
    return total;
}
// Returns the number of occurrences of digit in the decimal
// representation of num. digit is an int between 0 and 9
// inclusive.
//
// Pseudocode Example:
// tabulationStation(18838, 8) => 3
// tabulationStation(55555, 3) => 0
// tabulationStation(0, 0) => 0 or 1 (either if fine)
//
int tabulationStation(int num, int digit)
{
    int count = 0;
    if (digit < 0 || digit > 9)
        return 0;
    if(num - digit == 0)
    {
        count = 1;
    }
    else if(num < 10 && num != digit)
    {
        count = 0;
    }
    else
    {
        if(!((num - digit) % 10 == 0))
        {
            count = 0;
        }
        else
        {
            count = 1;
        }
        count = count + tabulationStation(num /10, digit);
    }
    return count;
}
// Returns a string where the same characters next each other in
// string n are separated by "55" (you can assume the input
// string will not have 5’s)
//
// Pseudocode Example:
// highFives("goodbye") => "go55odbye"
// highFives("yyuu") => "y55yu55u"
// highFives("aaaa") => "a55a55a55a"
//
string highFives(string n)
{
    if(n.size() == 0)
    {
        return "";
    }
    string f;
    if(n.size() == 1)
    {
        f = n[0];
    }
    else
    {
        f = f + n[0];
        if(n[0] == n[1])
        {
            f = f + "55";
        }
        f = f + highFives(n.substr(1));
    }
 return f;
}
// str contains a single pair of parentheses, return a new
// string made of only the parentheses and whatever those
// parentheses contain
//
// Pseudocode Example:
// jheriCurls("abc{ghj}789") => "{ghj}"
// jheriCurls("{x}7") => "{x}"
// jheriCurls("4agh{y}") => "{y}"
// jheriCurls("4agh{}") => "{}"
//
string jheriCurls(string str)
{
    string f;
    if(str.size() == 0)
    {
        return "";
    }
    if(str[0] == '{')
    {
        if(str[str.size()-1] == '}')
            return str.substr(0, str.size());
        f = f + jheriCurls(str.substr(0, str.size()-1));
    }
    else
    {
        f = jheriCurls(str.substr(1));
    }
    return f;
}
// Return true if the total of any combination of elements in
// the array a equals the value of the target.
//
// Pseudocode Example:
// aggregationNation({2, 4, 8}, 3, 10) => true
// aggregationNation({2, 4, 8}, 3, 6) => true
// aggregationNation({2, 4, 8}, 3, 11) => false
// aggregationNation({2, 4, 8}, 3, 0) => true
// aggregationNation({}, 0, 0) => true
//
bool aggregationNation(const int a[], int size, int target)
{
    bool f = false, s = false;
    if(target == 0)
        return true;
    if(size == 0)
        return false;
    if(a[size - 1] == target)
        return true;
    else
        f = aggregationNation(a, size -1, target);
    s = aggregationNation(a, size -1, target - a[size -1]);
    if(s || f)
        return true;
    return false;
}
// Write a C++ function named onMyWayHome that determines whether or not there's a path from start to finish in a rectangular maze. Here is the prototype:
bool onMyWayHome(string maze[], int nRows, int nCols,
 int sr, int sc, int er, int ec)
{
    bool f = false;
    if(sr == er && sc == ec)
        f = true;
    if(sr + 1 < nRows && maze[sr + 1][sc] != 'X' && maze[sr + 1][sc] != '@' && maze[sr + 1][sc] != 'v' && f != true)
    {
        maze[sr][sc] = 'v';
        f = onMyWayHome(maze, nRows, nCols, sr + 1, sc, er, ec);
    }
    if(sc + 1 < nCols && maze[sr][sc+1] != 'X' && maze[sr][sc +1] != '@' && maze[sr][sc +1] != 'v' && f != true)
    {
        maze[sr][sc] = 'v';
        f = onMyWayHome(maze, nRows, nCols, sr, sc + 1, er, ec);
    }
    if(sr - 1 > 0 && maze[sr - 1][sc] != 'X' && maze[sr - 1][sc] != '@' && maze[sr - 1][sc] != 'v' && f != true)
    {
        maze[sr][sc] = 'v';
        f = onMyWayHome(maze, nRows, nCols, sr - 1, sc, er, ec);
    }
    if(sc - 1 > 0 && maze[sr][sc - 1] != 'X' && maze[sr][sc - 1] != '@' && maze[sr][sc - 1] != 'v' && f != true)
    {
        maze[sr][sc] = 'v';
        f = onMyWayHome(maze, nRows, nCols, sr, sc - 1, er, ec);
    }
    maze[sr][sc] = 'v';
    return f;
}
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

int main() {
}
