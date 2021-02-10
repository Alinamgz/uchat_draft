#include "client.h"

static void con_retry();
static gboolean check_scene(void *param);

void mx_init_gtk_app(t_client *client) {
    gtk_init(NULL, NULL);

    mx_init_error_dialog(client);
    mx_init_login_window(client);
	gtk_label_set_text(GTK_LABEL(client->ui->fail_reason_msg), "Trying to reach...");

    mx_connection_retry_th(NULL, (gpointer*)client);
    // mx_connect_retry_gtk(NULL, (gpointer*)client);

    g_timeout_add(50, mx_check_scene, client);

    while(gtk_events_pending())
	    gtk_main_iteration();

    gtk_main();
}

 void exit_gtk(GtkWidget *widget, void *param) {
    gtk_main_quit();
    exit(0);
}
