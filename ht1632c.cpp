/**********************************\
| HT1632c library by Derk Vedelaar |
| http://www.vedelaar.nl/          |
| vedelaar@gmail.com               |
\**********************************/

#include <avr/io.h>
#include "ht1632c.h"

//Modi:
#define HT1632C_OPERATION_READ      0b110 //Need RD pin, not available on my display
#define HT1632C_OPERATION_WRITE     0b101 //also read-modify-write, address first (7 bit) then data (4 bit). When stacked, address is sent once and automatically incremented
#define HT1632C_OPERATION_COMMAND   0b100 //Can be stacked

//Commands:                    
#define HT1632C_COMMAND_SYSDIS      0b000000000 //SYS DIS             1 0 0 0000-0000-X C   Turn off both system oscillator and LED duty    cycle generator   Yes   
#define HT1632C_COMMAND_SYSEN       0b000000010 //SYS EN              1 0 0 0000-0001-X C   Turn on system oscillator
#define HT1632C_COMMAND_LEDOFF      0b000000100 //LED Off             1 0 0 0000-0010-X C   Turn off LED duty cycle generator Yes
#define HT1632C_COMMAND_LEDON       0b000000110 //LED On              1 0 0 0000-0011-X C   Turn on LED duty cycle generator
#define HT1632C_COMMAND_BLINKOFF    0b000010000 //BLINK Off           1 0 0 0000-1000-X C   Turn off blinking function Yes
#define HT1632C_COMMAND_BLINKON     0b000010010 //BLINK On            1 0 0 0000-1001-X C   Turn on blinking function
#define HT1632C_COMMAND_SLAVEMODE   0b000100000 //SLAVE Mode          1 0 0 0001-0XXX-X C   Set slave mode and clock source from external clock, the system clock input from OSC pin and synchronous signal input from SYN pin 
#define HT1632C_COMMAND_MASTERMODE  0b000110000 //RC Master Mode      1 0 0 0001-10XX-X C   Set master mode and clock source from on-chip RC oscillator, the system clock output to OSC pin and synchronous signal output to SYN pin     Yes
#define HT1632C_COMMAND_EXTCLK      0b000111000 //EXT CLK Master Mode 1 0 0 0001-11XX-X C   Set master mode and clock source from external clock, the system clock input from OSC pin and synchronous signal output to SYN pin
#define HT1632C_COMMAND_PWMDUTY     0b101000000 //PWM Duty            1 0 0 101X-abcd-X C   PWM duty, abcd = binary low to high Yes
#define HT1632C_COMMAND_COMOPTION   0b001000000 //COM Option          1 0 0 0010-abXX-X C   ab=00: N-MOS open drain output and 8 COM option
                                                //                                          ab=01: N-MOS open drain output and 16 COM option
                                                //                                          ab=10: P-MOS open drain output and 8 COM option
                                                //                                          ab=11: P-MOS open drain output and 16 COM option

#define setbit(value, bit)   (value|=(1<<bit))
#define clearbit(value, bit) (value&=(~(1<<bit)))

HT1632C::HT1632C(
  volatile uint8_t* const cs_ddr, 
  volatile uint8_t* const cs_port, 
  const    uint8_t         cs_pin, 
  volatile uint8_t* const wr_ddr, 
  volatile uint8_t* const wr_port, 
  const    uint8_t         wr_pin, 
  volatile uint8_t* const data_ddr, 
  volatile uint8_t* const data_port, 
  const    uint8_t         data_pin)
  :
  cs_ddr    (cs_ddr), 
  cs_port   (cs_port),
  cs_pin    (cs_pin),
  wr_ddr    (wr_ddr), 
  wr_port   (wr_port),
  wr_pin    (wr_pin),
  data_ddr  (data_ddr), 
  data_port (data_port),
  data_pin  (data_pin)
{
}

void HT1632C::send(const uint16_t data, const uint8_t bits)
{
  for (uint16_t i = bits; i>0; i--) //MSB first...
  {
    clearbit(*wr_port, wr_pin);
    if (data & ((uint16_t)1 << i-1)) 
      setbit(*data_port, data_pin);
    else
      clearbit(*data_port, data_pin);
    setbit(*wr_port, wr_pin);
  }
}

void HT1632C::command(const uint16_t data)
{
  setbit(*cs_port, cs_pin);
  clearbit(*cs_port, cs_pin);
  send(HT1632C_OPERATION_COMMAND,3);
  send(data,9);
  setbit(*cs_port, cs_pin);
}

void HT1632C::start(void)
{
  setbit(*cs_ddr,cs_pin);
  setbit(*wr_ddr,wr_pin);
  setbit(*data_ddr,data_pin);
  setbit(*cs_port,cs_pin);
  setbit(*wr_port,wr_pin);
  setbit(*data_port,data_pin);

  command(HT1632C_COMMAND_SYSEN);
  command(HT1632C_COMMAND_MASTERMODE);
  command(HT1632C_COMMAND_BLINKOFF);
  command(HT1632C_COMMAND_LEDON);
  command(HT1632C_COMMAND_PWMDUTY);
}

void HT1632C::stop(void)
{
  command(HT1632C_COMMAND_LEDOFF);
  command(HT1632C_COMMAND_SYSDIS);
}

void HT1632C::start_blink(void)
{
  command(HT1632C_COMMAND_BLINKON);
}

void HT1632C::stop_blink(void)
{
  command(HT1632C_COMMAND_BLINKOFF);
}

void HT1632C::set_brightness(const uint8_t brightness)
{
  uint8_t filtered_brightness = brightness & 0b00001111;
  command(HT1632C_COMMAND_PWMDUTY | (filtered_brightness << 1));
}

void HT1632C::set_com_option(const uint8_t option)
{
  uint8_t filtered_option = option & 0b00000011;
  command(HT1632C_COMMAND_COMOPTION | (filtered_option << 3));
}

void HT1632C::begin_sent_data(const uint8_t address)
{
  clearbit(*cs_port, cs_pin);
  send(HT1632C_OPERATION_WRITE,3);
  send(address,7);
}

void HT1632C::sent_data(const uint8_t data)
{
  send(data,8);
}

void HT1632C::finish_sent_data()
{
  setbit(*cs_port, cs_pin);
}