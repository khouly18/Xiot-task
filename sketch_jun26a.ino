//libraries that includes necessary macros for activating interrupts. We can work without these libraries, but this will be a hedache because we would need to make all the macros required

#include <avr/io.h>
#include <avr/interrupt.h>
      //Simple explanation for the code

//For the temperature sensor I am setting a timer interrupt for every 1 sec and after three interrupts I will read the temperature value

//declaring the suitable pins on the arduino board
//the switch will be connected to pin 2, as only pins 2 and 3 on the arduino board work for external interrupts
//LED is connected to pin 12, it could be connected to any pin, but I chose 12
const unsigned char ucswtch = 2;
const unsigned char ucled = 12;

//needed variables
unsigned char ucTempCount=0;
unsigned char ucflag = 0;
unsigned char ucflag1 = 1;
unsigned char ucswitchstate = 0;
//interrupt service routine for timer
ISR(TIMER1_COMPA_vect)
{
  //timer is set to interrupt every 1 sec. I made a ucTempCount to count to 3 to wait for the 3 seconds
  ucTempCount++;
  if(ucTempCount==3)
  {
    ucTempCount=0;
    Serial.print("temperature= ");
    Serial.print(analogRead(0));
  }
}
//interrupt service routine functionality (must be as small as possible to not delay the main code, but the code in itself is about the switch, led and temperature 
//sensor then no problem if is a little long)
ISR(INT0_vect)
{
  //reading the value of the input
  delay(100);
  ucswitchstate=digitalRead(ucswtch);
  if(ucswitchstate==LOW)
  {
    ucflag1=1;
    if(ucflag)
    {
      Serial.print("pressed");
      ucflag=0;
      Serial.print("ON");
    }
    digitalWrite(ucled,1);
  }
  else
  {
    ucflag=1;
    if(ucflag1)
    {
      Serial.print("OFF");
      ucflag1=0;
    } 
    digitalWrite(ucled,0); 
  }
}
void setup() 
{
  //clearing all interrupts
  cli();
  //setting the pin states
  //switch is set to PULLUP to give the pin a default '1' state, so that the hardware can be simpler.
  //I made it pullup and not pulldown because there is an internal pullup network in most of AVR boards.
  pinMode(ucswtch,INPUT_PULLUP);
  pinMode(ucled,OUTPUT);
  //serial communication with serial port
  Serial.begin(9600);
  //activating the interrupt environment
  //setting the global interrupt
  sei();
  //setting the state of the interrupt where it will be activated. 
  //I chose it to be activated if there is a change in the logic in general, so that the interrupt is activated when switching ON or OFF the light
  MCUCR&=~(1<<0);
  MCUCR&=~(1<<1);


  
  //Now for the temperature sensor setup
  //Using timer1 interrupt as it is a 16 bit timer and more suitable for large time values
  //set timer1 interrupt at 1Hz
  //set entire TCCR1A register to 0
  TCCR1A = 0;
  //set entire TCCR1B register to 0
  TCCR1B = 0;
  //initialize ucTempCount value to 0
  TCNT1  = 0;
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
}

void loop() 
{

}
