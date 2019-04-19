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
	haptyk_init("E7:2F:7B:39:84:52");
	int index;
	for(index = 0; index < 10; index++) {
		haptyk_get_data();
	}
	
	haptyk_disconnect("E7:2F:7B:39:84:52");
	return 0;
}
