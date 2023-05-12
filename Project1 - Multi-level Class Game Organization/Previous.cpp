//
//  Previous.cpp
//  cs32project1
//
//  Created by Sari Abu-Hamad on 6/29/22.
//
#include <iostream>
#include "globals.h"
#include "Previous.h"
using namespace std;
Previous::Previous(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for(int r = 0; r < m_rows; r++)
        for(int c = 0; c < m_cols; c++)
            m_grid[r][c] = 0;
}
bool Previous::dropACrumb(int r, int c)
{
    if(r < 1 || c < 1 || r > m_rows || c > m_cols)
        return false;
    m_grid[r-1][c-1]++;
    return true;
}
void Previous::showPreviousMoves() const
{
    // Position (row,col) in the arena coordinate system is represented in
    // the array element grid[row-1][col-1]
  char grid[MAXROWS][MAXCOLS];
  int r, c;
      // Fill the grid with dots
  for (r = 0; r < m_rows; r++)
  {
      for (c = 0; c < m_cols; c++)
      {
          if(m_grid[r][c] == 0)
          {
             grid[r][c] = '.';
          }
          else if (m_grid[r][c] > 0 && m_grid[r][c] < 26)
          {
              grid[r][c] = (char)(m_grid[r][c] + 64);
          }
          else
          {
              grid[r][c] = 'Z';
          }
      }
  }
  
    clearScreen();
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
        {
            cout << grid[r][c];
        }
        cout << endl;
    }
    cout << endl;
        // Write message, robot, and player info
}
