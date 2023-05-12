//
//  Previous.hpp
//  cs32project1
//
//  Created by Sari Abu-Hamad on 6/29/22.
//

#ifndef Previous_h
#define Previous_h

#include "globals.h"

class Previous
 {
   public:
     Previous(int nRows, int nCols);
     bool dropACrumb(int r, int c);
     void showPreviousMoves() const;
    private:
     //2d grid
     int m_grid[MAXROWS][MAXCOLS];
     int m_rows;
     int m_cols;
 };

#endif /* Previous_h */
