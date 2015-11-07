#include <ht1632c.h>

/*
You have to find the avr port and pin numer to be able to use this library. Here is a table:
Digital pin 0:  DDRD PORTD 0
Digital pin 1:  DDRD PORTD 1
Digital pin 2:  DDRD PORTD 2
Digital pin 3:  DDRD PORTD 3
Digital pin 4:  DDRD PORTD 4
Digital pin 5:  DDRD PORTD 5
Digital pin 6:  DDRD PORTD 6
Digital pin 7:  DDRD PORTD 7
Digital pin 8:  DDRB PORTB 0
Digital pin 9:  DDRB PORTB 1
Digital pin 10: DDRB PORTB 2
Digital pin 11: DDRB PORTB 3
Digital pin 12: DDRB PORTB 4
Digital pin 13: DDRB PORTB 5
Analog pin 0:   DDRC PORTC 0
Analog pin 1:   DDRC PORTC 1
Analog pin 2:   DDRC PORTC 2
Analog pin 3:   DDRC PORTC 3
Analog pin 4:   DDRC PORTC 4
Analog pin 5:   DDRC PORTC 5
*/

//First 3 parameters are CS, second three are WR and third three are DATA pins
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

void setup() {
  //turn on the display, there is also a stop() to turn it off
  display.start();
  //change the display layout. Possibilities are 0, 1, 2 and 3.
  display.set_com_option(0);
  //The birghtness. 0 is lowest (not off) and 15 is highest.
  display.set_brightness(15);
}

void loop() {
  //begin sending data
  display.begin_sent_data();
  for (uint8_t i = 0; i < 32; i++)
    if (i%2)
      //For a 32*8 display you need to sent 32 bytes to fill the screen
      display.sent_data(0b01010101);
    else
      display.sent_data(0b10101010);
  //stop sending data and show on display what we had just sent
  display.finish_sent_data();

  //wait some time
  delay(200);

  //do the same thing again but inverse it.
  display.begin_sent_data();
  for (uint8_t i = 0; i < 32; i++)
    if (! (i%2))
      display.sent_data(0b01010101);
    else
      display.sent_data(0b10101010);
  display.finish_sent_data();

  delay(200);
}
