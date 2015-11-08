/**********************************\
| Game of life by Derk Vedelaar    |
| http://www.vedelaar.nl/          |
| vedelaar@gmail.com               |
\**********************************/

#include <stdlib.h>     // for rand()

#include "gameoflife.h"

gameoflife::gameoflife(uint8_t w, uint8_t h)
  :
  width  (w), 
  height (h),
  turns (0)
{
  field = (uint8_t**)malloc(sizeof(uint8_t*) * w);
  for(uint8_t i = 0 ; i < w ; i++)
    field[i] = (uint8_t*)malloc(sizeof(uint8_t) * h);

  field2 = (uint8_t**)malloc(sizeof(uint8_t*) * w);
  for(uint8_t i = 0 ; i < w ; i++)
    field2[i] = (uint8_t*)malloc(sizeof(uint8_t) * h);
}

void gameoflife::setbit(uint8_t x, uint8_t y, uint8_t zeroone)
{
  uint8_t bit = x % 8;
  uint8_t display = x / 8;
  uint8_t wbyte = y;
  
  uint8_t filter = 0x80 >> bit;
  
  if (zeroone == 1)
    field[display][wbyte] |= filter;
  else
    field[display][wbyte] &= (~filter);
}

uint8_t gameoflife::getbit_field(uint8_t** thefield, uint8_t x, uint8_t y)
{
  uint8_t bit = x % 8;
  uint8_t display = x / 8;
  uint8_t byte = y;
  
  return (((thefield[display][byte]) << (bit)) & 0x80) >> 7;
}

uint8_t gameoflife::getneighbours(uint8_t x,uint8_t y)
{
  uint8_t neighbours = 0;
  //above
  if (y > 0 && getbit_field(field2, x, y-1) == 1)
    ++neighbours;
  //below
  if (y < height-1 && getbit_field(field2, x, y+1) == 1)
    ++neighbours;
  //left
  if (x > 0 && getbit_field(field2, x-1, y) == 1)
    ++neighbours;
  //right
  if (x < (width*8)-1 && getbit_field(field2, x+1, y) == 1)
    ++neighbours;
  
  //left above
  if (y > 0 && x > 0 && getbit_field(field2, x-1, y-1) == 1)
    ++neighbours;
  //right above
  if (y > 0 && x < (width*8) - 1 && getbit_field(field2, x+1, y-1) == 1)
    ++neighbours;
  //left below
  if (y < height-1 && x > 0 && getbit_field(field2, x-1, y+1) == 1)
    ++neighbours;
  //right below
  if (y < height-1 && x < (width*8)-1 && getbit_field(field2, x+1, y+1) == 1)
    ++neighbours;
      
  return neighbours;
}

void gameoflife::step()
{
  for ( uint8_t h = 0 ; h  < height; ++h )
    for ( uint8_t w = 0 ; w < width ; ++w)
      field2[w][h] = field[w][h];
  
  for (uint8_t y=0 ; y<height ; ++y)
  {
    for (uint8_t x=0 ; x<(width*8) ; ++x)
    {
      uint8_t neighbours = getneighbours(x, y);
      
      //Any live cell with fewer than two live neighbours dies, as if caused by under-population.
      //Any live cell with two or three live neighbours lives on to the next generation.
      //Any live cell with more than three live neighbours dies, as if by overcrowding.
      //Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
      if (getbit_field(field, x, y) == 1)
        if (neighbours == 2 || neighbours ==3)
          setbit(x, y, 1);
        else 
          setbit(x, y, 0);
      else
        if (neighbours == 3)
          setbit(x, y, 1);
        else
          setbit(x, y, 0);
    }
  }
}

void gameoflife::randomfield(uint16_t seed)
{
  srand(seed);
  for ( uint8_t h = 0  ; h  < height; ++h )
    for ( uint8_t w = 0 ; w < width*8 ; ++w)
      if (rand() %3)
        setbit(w, h, 0);
      else
        setbit(w, h, 1);
}

uint8_t gameoflife::countcells()
{
  uint8_t count = 0;
  for (uint8_t y = 0; y<height; ++y)
    for (uint8_t x=0; x<(width*8); ++x)
      if (getbit_field(field, x, y) == 1)
        ++count;
  return count;
}

bool gameoflife::is_dead(uint8_t generations)
{
  //shift 0 out of the array and shift countcells into the array
  for(uint8_t i = 0; i < 4; i++)
    if (i != 3)
      lastcounts[i] = lastcounts[i+1];
    else
      lastcounts[i] = countcells();
  
  //check if any of these have the same amount of counts
  bool thesame = false;
  for(uint8_t i = 0; i < 3; i++)
    if (lastcounts[i] == lastcounts[3])
      thesame = true;
  
  if (thesame)
    ++turns;
  else
    turns = 0;
    
  if (turns == generations)
    return true;
  return false;
}
