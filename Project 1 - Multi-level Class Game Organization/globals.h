//
//  globals.h
//  cs32project1
//
//  Created by Sari Abu-Hamad on 6/22/22.
//

#ifndef globals_h
#define globals_h

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////
const int MAXROWS = 20;             // max number of rows in the arena
const int MAXCOLS = 40;             // max number of columns in the arena
const int MAXROBOTS = 130;          // max number of robots allowed
const int MAXSHOTLEN = 5;           // max number of steps you can shoot
const int INITIAL_ROBOT_HEALTH = 2;
const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;
///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////
int decodeDirection(char dir);
void clearScreen();

#endif /* globals_h */
