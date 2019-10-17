#include "msp.h"
#include <math.h>

double TestFunction(double num);

int main(void) {
    double mainVar;
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    P4->SEL1 &= ~BIT1; //set P1.0 as simple I/O
    P4->SEL0 &= ~BIT1;
    P4->DIR |= BIT1; //set P1.0 as output
    P4->SEL1 &= ~BIT5; //set P2.0 as simple I/O
    P4->SEL0 &= ~BIT5;
    P4->DIR |= BIT5; //set P2.0 as output pins
    while(1) { // infinite loop to do nothing
        P4->OUT |= BIT5; // turn on Blue LED
        mainVar = TestFunction(15); // test function for timing
        P4->OUT &= ~BIT5; // turn off Blue LED
    }

}
double TestFunction(double num) {
    double testVar;
    P4->OUT |= BIT1; // turn RED LED on
    // test function
    testVar = abs(num);
    P4->OUT &= ~BIT1; // turn RED LED off
    return testVar;
}
