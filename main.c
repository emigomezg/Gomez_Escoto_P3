#include "PSM.h"

#define DEBUG 1

#ifdef DEBUG
	#include "stdio.h"
#endif



int main(void)
{

	PSM_INIT();
	/*
	screen_clear_data();
	screen_add_Item_end('6');
	screen_add_Item_end('7');
	screen_add_Item_end('8');
	screen_add_Item_end('9');
	screen_add_Item_end('A');

	HT16k33_init();
//HT16k33_sendMat(&data[0]);
	for(;;) {
		screen_send_array_2mat();
		screen_slideR();
		for(uint32_t a=0;a<400000;a++);
	}
	*/
	while (1) {
	}
	return 0;
}
