#include "PSM.h"

#define DEBUG 1

#ifdef DEBUG
#include "stdio.h"
#endif

int main(void)
{

	PSM_INIT();

	while (1) {
		if (PSM_GET_CHANGE(TERMINAL1)) {
			PSM_STM(TERMINAL1);
		}
		if (PSM_GET_CHANGE(TERMINAL2)) {
			PSM_STM(TERMINAL2);
			PSM_CLEAN_CHANGE(TERMINAL2);
		}

	}
	return 0;
}
