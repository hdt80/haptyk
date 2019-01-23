#ifndef HAPTYK_H
#define HAPTYK_H

// Kameron Ramey
// 12 Jan 2019

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

typedef struct{
	short device_id;
	char buttons[12];
} haptyk_t;


int pair_haptyk(int scan_duration){
	
	inquiry_info * devices = NULL;
	int max_rsp, num_rsp;
	int device_id, sock, flags;
	int index;
	char addr[19] = { 0 };
	char name[248] = { 0 };


	device_id = hci_get_route(NULL);	// retrieve the resource number of the first available bluetooth adapter
	sock = hci_open_dev(device_id);		// enable the bluetooth device
	if ( defice_id < 0 || sock < 0){	// make sure bluetooth is enabled on target device
		perror("failed to open bluetooth device\n");
		exit(-1);
	}

	max_rsp = 64;				// limit the number of bluetooth responses
	flags = IREQ_CACHE_FLUSH;		// flush device cache from previous search

	responses = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info)); // allocate memory for device search 

	num_rsp = hci_inquiry(device_id, scan_duration, max_rsp, NULL, &responses, flags) // scan for bluetooth devices for scan_duration
	

	
	return 0;
}

#endif HAPTYK_H
