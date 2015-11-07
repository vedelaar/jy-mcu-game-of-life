#include <gameoflife.h>

//4 bytes wide, by 32 pixels high.
gameoflife gol(4,32);

word seed = 0;

void setup() {
  Serial.begin(115200);

  //create a random field, it's just setting pixels at random, nothing smart
  gol.randomfield(seed);
}

//print binary string with leading zeros
void print_binary(uint8_t byte)
{
  //buffer, 8 bits + space for the null character
  char buffer[9];
  itoa (byte, buffer, 2);
  //figure out how many leading zeros we need and print them
  for (int i = 0; i < 8-strlen(buffer); i++) {
    Serial.print('0');
  }
  //print the string without leading zeros
  Serial.print(buffer);
}

void loop() {
  //loop over the rows (notice the 32 bits high we initialized the gameoflife with)
  for ( uint8_t h = 0 ; h < 32; ++h )
  {
    //loop over the columns (notice the 4 bytes we had initialized the gameoflife with)
    for ( uint8_t w = 0 ; w < 4 ; ++w)
    {
      //print this particilar byte of the game of life
      print_binary(gol.field[w][h]);
    }
    //new line for the next row
    Serial.print("\n");
  }

  //new line for the next execution
  Serial.print("\n");

  //calculate the next step
  gol.step();

  //create a new random field if the old field is dead after waiting 40 executions (don't make this too low!)
  if (gol.is_dead(40))
    gol.randomfield(++seed);

  //wait some time
  delay(200);
}
