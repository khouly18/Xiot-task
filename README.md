"# Xiot-task" 
//For the temperature sensor I am setting a timer interrupt for every 1 sec and after three interrupts I will read the temperature value

//For the push button I connected it to an external interrupt tigger and I accomodated the code in an interrupt service routine

//declaring the suitable pins on the arduino board
//the switch will be connected to pin 2, as only pins 2 and 3 on the arduino board work for external interrupts
//LED is connected to pin 12, it could be connected to any pin, but I chose 12
