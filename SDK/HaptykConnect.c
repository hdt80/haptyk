// Kameron Ramey
// 12 Jan 2019

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "HaptykConnect.h"

#include "bluez/lib/uuid.h"
#include "bluez/btio/btio.h"
#include "att.h"
#include "gattrib.h"
#include "gatt.h"
#include "gatttool.h"

static char * source_dev = NULL; 		// Specify client bluetooth adapter
static char * target_dev = "E7:2F:7B:39:84:52"; // Haptyk BLE addr
static char * addr_type = "random";       	// Haptyk BLE addr type
static char * security_level = "low";		// Default Security

void connect_cb(GIOChannel *io, GError * error, gpointer data){
	GAttrib *attribute;
	
	if (error){
		g_printerr("%s\n", error->message);
		//g_main_loop_quit(main_loop);
	}
	attribute = g_attrib_new(io);
}

void read_characteristic_cb(guint8 status, const guint8* pdu, guint16 plen,
		gpointer user_data) {

	uint8_t value[plen];
	ssize_t vlen;
	int i;

	if (status != 0) {
		g_printerr("Characteristic value/descriptor read failed: %s\n",
							att_ecode2str(status));
	}

	vlen = dec_read_resp(pdu, plen, value, sizeof(value));
	if (vlen < 0) {
		g_printerr("Protocol error\n");
	}
	g_print("Characteristic value/descriptor: ");
	for (i = 0; i < vlen; i++)
		g_print("%02x ", value[i]);
	g_print("\n");
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

gboolean read_characteristic(gpointer data){
	GAttrib * attribute = data;
	
	gatt_read_char(attribute, 0x0001, read_characteristic_cb, attribute);
	
	return FALSE;
}
