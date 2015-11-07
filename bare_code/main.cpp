/**********************************\
| Game of life by Derk Vedelaar    |
| http://www.vedelaar.nl/          |
| vedelaar@gmail.com               |
\**********************************/

#define F_CPU 16000000  // set processor speed
#include <avr/io.h>     // for DDRB PORTB etc

#include "ht1632c.h"
#include "gameoflife.h"

//all the displays
HT1632C display1( // this is the display that is executing the code
  &DDRB, 
  &PORTB, 
  3, //cs
  &DDRB, 
  &PORTB, 
  4, //wr
  &DDRB, 
  &PORTB, 
  5); //data

HT1632C display2(
  &DDRD, 
  &PORTD, 
  1, //cs (txd)
  &DDRB, 
  &PORTB, 
  4, //wr
  &DDRB, 
  &PORTB, 
  5); //data

HT1632C display3(
  &DDRD, 
  &PORTD, 
  0, //cs (rxd)
  &DDRB, 
  &PORTB, 
  4, //wr
  &DDRB, 
  &PORTB, 
  5); //data

HT1632C display4(
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

//show on display 1
void print_display1_field()
{
  display1.begin_sent_data();
  for ( uint8_t w = 1 ; w < 5 ; ++w)
    for ( uint8_t h = 4 ; h < 12; ++h )
      display1.sent_data(gol.field[w][h]);
  display1.finish_sent_data();
}

//show on display 2
void print_display2_field()
{
  display2.begin_sent_data();
  for ( uint8_t w = 1 ; w < 5 ; ++w)
    for ( uint8_t h = 12 ; h < 20; ++h )
      display2.sent_data(gol.field[w][h]);
  display2.finish_sent_data();
}

//show on display 3
void print_display3_field()
{
  display3.begin_sent_data();
  for ( uint8_t w = 1 ; w < 5 ; ++w)
    for ( uint8_t h = 20 ; h < 28; ++h )
      display3.sent_data(gol.field[w][h]);
  display3.finish_sent_data();
}

//show on display 4
void print_display4_field()
{
  display4.begin_sent_data();
  for ( uint8_t w = 1 ; w < 5 ; ++w)
    for ( uint8_t h = 28 ; h < 36; ++h )
      display4.sent_data(gol.field[w][h]);
  display4.finish_sent_data();
}

int main(void) 
{
  //initialize the displays
  display1.start();
  display1.set_com_option(0);
  display1.set_brightness(15);
  display2.start();
  display2.set_com_option(0);
  display2.set_brightness(15);
  display3.start();
  display3.set_com_option(0);
  display3.set_brightness(15);
  display4.start();
  display4.set_com_option(0);
  display4.set_brightness(15);
  
  //initialize field
  gol.randomfield();
  
  while(1)
  {
    //show on 2 displays
    print_display1_field();
    print_display2_field();
    print_display3_field();
    print_display4_field();
    
    //play
    gol.step();
    
    //recreate field when it does not do any interesting antmore
    if (gol.is_dead(40))
      gol.randomfield();
  }
  return(0);
}