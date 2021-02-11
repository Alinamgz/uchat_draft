#include "client.h"

static void init_widgets(GtkBuilder *builder, t_client *client);
static void connect_signals(t_client *client);

void mx_init_login_window(t_client *client) {
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("client/templates/login_window.ui");
    if (!builder)
        g_critical("Builder getting error!");

    gtk_builder_connect_signals(builder, NULL);

    init_widgets(builder, client);
    connect_signals(client);

    g_object_unref(builder);
}

static void init_widgets(GtkBuilder *builder, t_client *client) {
    if (!builder) return;

    client->ui->login_window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    if (!client->ui->login_window)
        g_critical("Can't get login_window");

    client->ui->l_username_entry = GTK_WIDGET(gtk_builder_get_object(builder, "l_username_entry"));
    if (!client->ui->l_username_entry)
        g_critical("Can't get l_username_entry");

    client->ui->l_username_status = GTK_WIDGET(gtk_builder_get_object(builder, "l_username_status"));
    if (!client->ui->l_username_status)
        g_critical("Can't get l_username_status");

    client->ui->l_pass_entry = GTK_WIDGET(gtk_builder_get_object(builder, "l_pass_entry"));
    if (!client->ui->l_pass_entry)
        g_critical("Can't get l_pass_entry");

    client->ui->l_pass_status = GTK_WIDGET(gtk_builder_get_object(builder, "l_pass_status"));
    if (!client->ui->l_pass_status)
        g_critical("Can't get l_pass_status");

    client->ui->login_btn = GTK_WIDGET(gtk_builder_get_object(builder, "login_btn"));
    if (!client->ui->login_btn)
        g_critical("Can't get login_btn");

    client->ui->show_registration_btn = GTK_WIDGET(gtk_builder_get_object(builder,"show_registration_btn"));
    if (!client->ui->show_registration_btn)
        g_critical("Can't get show_registration_btn");

}
static void connect_signals(t_client *client) {
    // TODO: what's the point in signal_conect "activate" for entry?
    g_signal_connect(G_OBJECT(client->ui->l_username_entry),
                              "activate",
                              G_CALLBACK(mx_get_input_values),
                              client);

    g_signal_connect(G_OBJECT(client->ui->l_pass_entry),
                              "activate",
                              G_CALLBACK(mx_get_input_values),
                              client);

    g_signal_connect(G_OBJECT(client->ui->login_btn),
                              "clicked",
                              G_CALLBACK(mx_submit_login_handler),
                              client);

    g_signal_connect(G_OBJECT(client->ui->show_registration_btn),
                              "clicked",
                              G_CALLBACK(mx_switch_form),
                              client);
}
