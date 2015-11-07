#include <gameoflife.h>
#include <ht1632c.h>

ht1632c display(
  &DDRD,
  &PORTD,
  2, //cs
  &DDRD,
  &PORTD,
  3, //wr
  &DDRD,
  &PORTD,
  4); //data

//6 bites wide, by 12 pixels high
gameoflife gol(6,12);

word seed = 0;

void setup() {
  //turn on the display, there is also a stop() to turn it off 
  display.start();
  //change the display layout. Possibilities are 0, 1, 2 and 3.
  display.set_com_option(0);
  //The birghtness. 0 is lowest (not off) and 15 is highest.
  display.set_brightness(15);

  //create a random field, it's just setting pixels at random, nothing smart
  gol.randomfield(seed);
}

void loop() {
  display.begin_sent_data();
  for ( uint8_t w = 1 ; w < 5 ; ++w)
    for ( uint8_t h = 2 ; h < 10; ++h )
      display.sent_data(gol.field[w][h]);
  display.finish_sent_data();

  gol.step();

  if (gol.is_dead(40))
  {
    gol.randomfield(++seed);
  }
}
