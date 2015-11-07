/**********************************\
| Game of life by Derk Vedelaar    |
| http://www.vedelaar.nl/          |
| vedelaar@gmail.com               |
\**********************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gameoflife.h"

//the game of life, width (6 * 8 = 48 pixels), height is 40 pixels
gameoflife gol(6,40);

int main(void)
{
  uint16_t seed = 0;

  //initialize field
  gol.randomfield(seed);
  
  while(1)
  {
    for ( uint8_t h = 0 ; h < 40; ++h )
    {
      for ( uint8_t w = 0 ; w < 6 ; ++w)
      {
        for (uint8_t i = 0; i<8; i++)
        {
          if (gol.field[w][h] & ((uint8_t)1 << i-1))
            printf("1");
          else
            printf(" ");
        }
      }   
      printf("\n");
    }

    for (uint8_t i = 0; i<48; i++)
      printf("-");
    printf("\n");

    //play
    gol.step();
    
    //recreate field when it does not do any interesting anymore
    if (gol.is_dead(40))
    {
      exit(1);
      gol.randomfield(++seed);
    }
  }
  return(0);
}
