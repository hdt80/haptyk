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

static gboolean opt_char_read = FALSE;		// Service options	
static gboolean opt_char_write = FALSE;
static gboolean opt_listen = FALSE;
static char * opt_value = NULL;
static int opt_handle = -1;

static GMainLoop * event_loop;
static gboolean error_flag = FALSE;		// error flag
static GSourceFunc current_opt;			// used for handling passed operations



static GOptionEntry gatt_options[] = {
	{"char-read", 0, 0, G_OPTION_ARG_NONE, &opt_char_read, "Read a characteristic from peripheral", NULL},
	{"char-write", 0, 0, G_OPTION_ARG_NONE, &opt_char_write, "write to characteristic at peripheral", NULL},
	{"listen", 0, 0, G_OPTION_ARG_NONE, &opt_listen, "listen for notifications and indications", NULL},
	{ NULL }
};

static GOptionEntry options[] = {

	{"adapter", 'i', 0, G_OPTION_ARG_STRING, &source_dev, "set local bluetooth adapter", "hciX"},
	{"peripheral", 'b', 0, G_OPTION_ARG_STRING, &target_dev, "set peripheral address", "MAC"},
	{"addr-type", 't', 0, G_OPTION_ARG_STRING, &addr_type, "set address type", "[random | public]"},
	{"security", 'l', 0, G_OPTION_ARG_STRING, &security_level, "set security level", "[low | medium | high]"},
	{ NULL }
};

static GOptionEntry read_write_options[] = {

	{"handle", 'a', 0, G_OPTION_ARG_INT, &opt_handle, "specify the handle to read/write", "0x0001"},
	{"value", 'n', 0, G_OPTION_ARG_STRING, &opt_value, "Specify value to be written to characteristic", "0x0001"},
	{ NULL }
};

void connect_cb(GIOChannel *io, GError * error, gpointer data){		/* Callback for determining connection */
	GAttrib *attribute;						
	
	if (error){
		g_printerr("%s\n", error->message);	// could not connect to device
		g_main_loop_quit(event_loop);		// quit the main loop
	}
	attribute = g_attrib_new(io);			// create IO attribute		

	if(opt_listen){					// if we are listening for characteristic data
		g_idle_add(listen_start, attribute);	// add idle process to monitor changes
	}

	operation(attribute);				// run current operation from the attribute
}

static gboolean listen_start(gpointer data){	/* Register GATT attributes for listening in the event handler */
	GAttrib * attribute;
	
	// Notifications
	g_attrib_register(attribute, ATT_OP_HANDLE_NOTIFY, GATTRIB_ALL_HANDLES, event_handler, attribute, NULL);

	// Indications
	g_attrib_register(attribute, ATT_OP_HANDLE_IND, GATTRIB_ALL_HANDLES, event_handler, attribute, NULL);

	return FALSE;
}

static gboolean read_characteristic(gpointer data){				/* Read from a passed characteristic */	
	GAttrib * attribute = data;					
	
	gatt_read_char(attribute, 0x0001, read_characteristic_cb, attribute); // pass the read char callback func
	
	return FALSE;
}

void read_characteristic_cb(guint8 status, const guint8* pdu, guint16 plen,	/* Callback for returning read characteristic */
		gpointer user_data) {

	uint8_t value[plen];							
	ssize_t vlen;
	int i;

	if (status != 0) {	// the read failed print error												
		g_printerr("Characteristic value/descriptor read failed: %s\n", att_ecode2str(status));
	}

	vlen = dec_read_resp(pdu, plen, value, sizeof(value));		// get the length of the returned characteristic data
	if (vlen < 0) {
		g_printerr("Protocol error\n");				// if we got no data an error has occured	
	}
	g_print("Characteristic value/descriptor: ");
	for (i = 0; i < vlen; i++)
		g_print("%02x ", value[i]);				// loop through the data array printing characteristic display
	g_print("\n");


}

int main(int argc, char * argv[]){      //(char * UUID, char * security){ 			/* This is the main connect loop for haptyk */
	GOptionContext * context;				
	GOptionGroup * gatt_group, *params_group, *char_rw_group; 
	GIOChannel * channel;
	GError * error;

//	if(UUID != NULL){
//		target_dev = strdup(UUID);
//	}
//	else if(security != NULL){
//		security_level = strdup(security);
//	}
	
	context = g_option_context_new(NULL);
	g_option_context_add_main_entries(context, options, NULL);
	
	/* GATT Commands */

	gatt_group = g_option_group_new("gatt", "GATT commands", "Show all GATT commands", NULL, NULL);
	g_option_context_add_group(context, gatt_group);
	g_option_group_add_entries(gatt_group, gatt_options);

	/* Services and Characteristics Arguments */
	
		

	/* Characteristics Read/Write Argument*/



	operation = characteristics_read;				

	channel = gatt_connect(source_dev, target_dev, addr_type, 
			security_level, 0, 0, connect_cb, &error);

	if(channel == NULL){
		error_flag = TRUE;
		g_printerr("%s\n", error->message);
		g_clear_error(&gerr);
		
	}

	event_loop = g_main_loop_new(NULL, FALSE);	// Instantiate an event handler	

	g_main_loop_run(event_loop);			// run the handler until it completes

	g_main_loop_unref(event_loop);			// unreference the used resources

	if(error_flag)					// check if an error occurred
		exit(EXIT_FAILURE);
	else
		exit(EXIT_SUCCESS);
}


