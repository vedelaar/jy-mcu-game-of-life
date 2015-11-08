/**********************************\
| Game of life by Derk Vedelaar    |
| http://www.vedelaar.nl/          |
| vedelaar@gmail.com               |
\**********************************/

#define F_CPU 16000000  // set processor speed

#include "ht1632c.h"
#include "gameoflife.h"

#include <avr/eeprom.h>

//all the displays
ht1632c display1( // this is the display that is executing the code
  &DDRB, 
  &PORTB, 
  3, //cs
  &DDRB, 
  &PORTB, 
  4, //wr
  &DDRB, 
  &PORTB, 
  5); //data

ht1632c display2(
  &DDRD, 
  &PORTD, 
  1, //cs (txd)
  &DDRB, 
  &PORTB, 
  4, //wr
  &DDRB, 
  &PORTB, 
  5); //data

ht1632c display3(
  &DDRD, 
  &PORTD, 
  0, //cs (rxd)
  &DDRB, 
  &PORTB, 
  4, //wr
  &DDRB, 
  &PORTB, 
  5); //data

ht1632c display4(
  &DDRD, 
  &PORTD, 
  1, //cs (txd)
  &DDRD, 
  &PORTD, 
  0, //wr (rxd)
  &DDRB, 
  &PORTB, 
  5); //data (data)

//the game of life, width (6 * 8 = 48 pixels), height is 40 pixels
gameoflife gol(6,40);

//initialize the displays
void initialize_display(ht1632c &display)
{
  display.start();
  display.set_com_option(0);
  display.set_brightness(15);
}

//show on display
void print_display_field(ht1632c &display, uint8_t from, uint8_t to)
{
  display.begin_sent_data();
  for ( uint8_t w = 1 ; w < 5 ; ++w)
    for ( uint8_t h = from ; h < to; ++h )
      display.sent_data(gol.field[w][h]);
  display.finish_sent_data();
}

int main(void)
{
  //initialize 4 displays
  initialize_display(display1);
  initialize_display(display2);
  initialize_display(display3);
  initialize_display(display4);
 
  uint16_t seed = eeprom_read_word((uint16_t*)0x02);
 
  //initialize field
  gol.randomfield(seed);
  
  while(1)
  {
    //show on 4 displays
    print_display_field(display1,  4, 12);
    print_display_field(display2, 12, 20);
    print_display_field(display3, 20, 28);
    print_display_field(display4, 28, 36);
    
    //play
    gol.step();
    
    //recreate field when it does not do any interesting anymore
    if (gol.is_dead(40))
    {
      gol.randomfield(++seed);
      eeprom_write_word((uint16_t*)0x02,seed);
    }
  }
  return(0);
}
