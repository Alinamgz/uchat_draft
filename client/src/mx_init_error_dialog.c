#include "client.h"


static void do_connect_retry(GtkWidget *widget, gpointer data);

void mx_init_error_dialog(t_client *client) {
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("client/templates/err_dialog.ui");
    if (!builder) g_critical("Builder getting error!");
    gtk_builder_connect_signals(builder, NULL);

    client->ui->err_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "err_dialog"));
    if (!client->ui->err_dialog) g_critical("Can't get err_dialog");
    client->ui->fail_reason_msg = GTK_WIDGET(gtk_builder_get_object(builder, "fail_reason_msg"));
    if (!client->ui->fail_reason_msg) g_critical("Can't get fail_reason_msg");

    client->ui->retry_btn = GTK_WIDGET(gtk_builder_get_object(builder, "retry_btn"));
    if (!client->ui->retry_btn) g_critical("Can't get fail_reason_msg");

    g_signal_connect(G_OBJECT(client->ui->retry_btn), "clicked", G_CALLBACK(do_connect_retry), client);

    // while(gtk_events_pending())
	//     gtk_main_iteration();
}

 static void do_connect_retry(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client*)data;

    gtk_label_set_text(GTK_LABEL(client->ui->fail_reason_msg), "Retrying...");

    while(gtk_events_pending())
	    gtk_main_iteration();

    client->scene = CONNECTION_ERR;
    // mx_connect_retry_gtk(widget, data);
    mx_connection_retry_th(widget, data);
}
