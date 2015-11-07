/**********************************\
| Game of life by Derk Vedelaar    |
| http://www.vedelaar.nl/          |
| vedelaar@gmail.com               |
\**********************************/

#ifndef gameoflife_h
#define gameoflife_h

//for utint8_t
#include <stdint.h>

class gameoflife
{
  private:
    //width and height of the field
    uint8_t width;
    uint8_t height;
    
    //field that is used for the calculations
    uint8_t** field2;
    
    //array for the is_dead function (remembers last 4 countcells() values)
    uint8_t lastcounts[4];
    
    //amount of turns that the field did not change (for the is_dead function)
    uint8_t turns;
    
    //set a bit in the field
    void setbit(uint8_t x, uint8_t y, uint8_t zeroone);
    
    //get the neighbours of a bit (works on field2)
    uint8_t getneighbours(uint8_t x,uint8_t y);
    
    //get a bit in the field
    uint8_t getbit_field(uint8_t** thefield, uint8_t x, uint8_t y);
    
  public:
    // Constructor, width in bytes (6 = 48 pixels) height in pixels
    gameoflife(uint8_t w, uint8_t h);

    // This is the field that contains the current display data
    // works similar to field[width][height]
    uint8_t** field;

    // Fill the field with random data
    void randomfield(uint16_t seed);
    
    // Calculate next generation
    void step();
    
    // Count all the cells in the current field
    uint8_t countcells();
    
    // Returns true if nothing interesting is happening anymore in the field
    // Generations is the amount of generations that are watched for changes
    bool is_dead(uint8_t generations);
};


#endif
