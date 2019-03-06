#ifndef HAPTYK_H
#define HAPTYK_H 
// Kameron Ramey
// 12 Jan 2019



#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "bluez-master/lib/uuid.h"
#include "bluez-master/attrib/att.h"
#include "bluez-master/btio/btio.h"
#include "bluez-master/attrib/gattrib.h"
#include "bluez-master/attrib/gatt.h"
#include "bluez-master/attrib/gatttool.h"




static char * source_dev = NULL; 		// Specify client bluetooth adapter
static char * target_dev = "E7:2F:7B:39:84:52"; // Haptyk BLE addr
static char * addr_type = "random";       	// Haptyk BLE addr type
static char * security_level = "low";		// Default Security


static void connect_cb(GIOChannel *io, GError * error, gpointer data){
	GAttrib *attribute;
	
	if (error){
		g_printerr("%s\n", error->message);
		//g_main_loop_quit(main_loop);
	}
	attribute = g_attrib_new(io);

	operation(attribute);
}

void connect_haptyk(char * UUID, char * security){
   	
	GOptionContext * context;
	GOptionGroup * gatt_group, *params_group, *char_rw_group;
	GIOChannel * channel;
	GError * error;

	if(UUID != NULL){
		target_dev = strdup(UUID);
	}
	else if(security != NULL){
		security_level = strdup(security);
	}
	
	//context = g_option_context_new(NULL);
	channel = gatt_connect(source_dev, target_dev, addr_type, 
			security_level, 0, 0, connect_cb, &error);
}
static gboolean read_characteristic(gpointer data){
	
	GAttrib * attribute = data;
	
	gatt_read_char(attrib, 0x0001, read_characteristic_cb, attribute);
	
	return false;
}
#endif
