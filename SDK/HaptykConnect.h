#pragma once

#include <glib-2.0/glib.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

void connect_haptyk(char* UUID, char* security);

gboolean read_characteristic(gpointer data);
