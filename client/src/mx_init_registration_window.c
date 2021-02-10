#include "client.h"

void mx_init_registration_window(t_client *client) {
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("client/templates/registration_window.ui");
    if (!builder)
        g_critical("Builder getting error!");
    gtk_builder_connect_signals(builder, NULL);

    client->ui->registration_window = GTK_WIDGET(gtk_builder_get_object(builder, "registration_window"));
    if (!client->ui->registration_window)
        g_critical("Can't get registration_window");

    client->ui->show_login_btn = GTK_WIDGET(gtk_builder_get_object(builder,"show_login_btn"));
    if (!client->ui->show_login_btn)
        g_critical("Can't get show_login_btn");
    g_signal_connect(G_OBJECT(client->ui->show_login_btn), "clicked", G_CALLBACK(mx_switch_form), client);

    g_object_unref(builder);
}

