/*
 *	HaptykConnect.c v2
 *	Team Haptyk
 *	4/11/2019
 *
 *	This source is used to interface with a Haptyk
 *	device. Connections will be made, and UUIDs
 *	of services will be read from and updated on	
 *	a global button data array for the developer
 *	to use.
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "gattlib.h"
#include "HaptykConnect.h"

int main(int argc, char * argv[]) {
	struct haptyk_buttons_t b1;
	b1.b1 = 1;

	const char* haptyk_addr = "CD:FB:24:E5:2F:14";
	//const char* haptyk_addr = "E7:2F:7B:39:84:52";

	haptyk_init(haptyk_addr);
	int index;
	fprintf(stdout,"%i\n",b1.b1);

	for (;;) {
		haptyk_get_data(&b1);
		haptyk_print(&b1);
	}
	
	haptyk_disconnect(haptyk_addr);
	return 0;
}
