

#include  "Delay.h"

void delays(uint32_t delay)
{
    volatile int counter, counter2;

    for(counter2 = 16; counter2 > 0; counter2--)
    {
        for(counter=delay; counter > 0; counter--);

    }
}
