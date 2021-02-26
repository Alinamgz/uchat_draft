#include "client.h"

static void init_windows(t_client *client);
static gboolean check_scene(void *param);

void mx_init_gtk_app(t_client *client) {
    gtk_init(NULL, NULL);

    init_windows(client);

	gtk_label_set_text(GTK_LABEL(client->ui->fail_reason_msg), "Trying to reach...");

    mx_connection_retry_th(NULL, (gpointer*)client);

    g_timeout_add(50, mx_check_scene, client);

    while(gtk_events_pending())
	    gtk_main_iteration();

    gtk_main();
}

static void init_windows(t_client *client) {
    mx_init_error_dialog(client);
    mx_init_login_window(client);
    mx_init_registration_window(client);
    // mx_init_chat_window(client);

    mx_chat_messenger(client);
}

 void exit_gtk(GtkWidget *widget, void *param) {
    t_client *client = (t_client *)param;

    gtk_main_quit();
    pthread_mutex_destroy(&client->mut);
	pthread_mutex_destroy(&client->connection_mut);
	pthread_cond_destroy(&client->req_cond);
	pthread_mutex_destroy(&client->req_sig_mut);

    close(client->sock_fd);
    system("leaks -q uchat");
    exit(0);
}
