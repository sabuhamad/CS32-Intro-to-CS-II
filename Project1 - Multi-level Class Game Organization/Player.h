//
//  Player.h
//  cs32project1
//
//  Created by Sari Abu-Hamad on 6/22/22.
//

#ifndef Player_h
#define Player_h
#include <iostream>

class Arena;
class Player
{
  public:
        // Constructor
    Player(Arena *ap, int r, int c);
        // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;
        // Mutators
    std::string takeComputerChosenTurn();
    void   stand();
    void   move(int dir);
    bool   shoot(int dir);
    void   setDead();
  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_age;
    bool   m_dead;
    int    computeDanger(int r, int c) const;
};


#endif /* Player_h */
