/**********************************\
| HT1632c library by Derk Vedelaar |
| http://www.vedelaar.nl/          |
| vedelaar@gmail.com               |
\**********************************/

#ifndef ht1632c_h
#define ht1632c_h

class HT1632C
{
  private:
    //send bits to the display
    void send(const uint16_t data, const uint8_t bits);
    
    //execute a command
    void command(const uint16_t data);
    
    //the ports, pins and direction registers
    volatile uint8_t* const cs_ddr; 
    volatile uint8_t* const cs_port;
    const    uint8_t         cs_pin; 
    volatile uint8_t* const wr_ddr;
    volatile uint8_t* const wr_port; 
    const    uint8_t         wr_pin;
    volatile uint8_t* const data_ddr;
    volatile uint8_t* const data_port;
    const    uint8_t         data_pin;
    
  public:
    //Constructor, enter the ports and pins of your display configuration
    HT1632C(
    volatile uint8_t* const cs_ddr, 
    volatile uint8_t* const cs_port, 
    const    uint8_t         cs_pin, 
    volatile uint8_t* const wr_ddr, 
    volatile uint8_t* const wr_port, 
    const    uint8_t         wr_pin, 
    volatile uint8_t* const data_ddr, 
    volatile uint8_t* const data_port, 
    const    uint8_t         data_pin);
    
    //Pins to output and initialize HT1632C
    void start(void);
    
    //Turn off HT1632C
    void stop(void);
    
    //Blink all the leds every quarter a second
    void start_blink(void);
    
    //Stop blinking
    void stop_blink(void);
    
    //Set the brightness of the display. Possible values: 0..15
    void set_brightness(const uint8_t brightness);
    
    //Setting for how your display's are connected. Possible values: 0..3
    void set_com_option(const uint8_t option);
    
    //Start sending data to the display. Give the address where you want to start sending
    void begin_sent_data(const uint8_t address = 0);
    
    //Sent data to the display. Every time called the next row will be sent to the display
    void sent_data(const uint8_t byte);
    
    //Finish sending data
    void finish_sent_data();

};

#endif