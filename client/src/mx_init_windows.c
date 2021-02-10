#include "client.h"

static void switch_form(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client*)data;

    if (client->scene == LOGIN) {
        if (!client->ui->registration_window) {
            mx_init_registration_window(client);
        }
        client->scene = REGISTRATION;
    }
    else if (client->scene == REGISTRATION)  {
        client->scene = LOGIN;
    }
}

 static void chck_clk(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client*)data;

    gtk_label_set_text(GTK_LABEL(client->ui->fail_reason_msg), "Retrying...");

    while(gtk_events_pending())
	    gtk_main_iteration();

    client->scene = CONNECTION_ERR;
    mx_connect_retry_gtk(widget, data);
}

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

    g_signal_connect(G_OBJECT(client->ui->retry_btn), "clicked", G_CALLBACK(chck_clk), client);

    // while(gtk_events_pending())
	//     gtk_main_iteration();
}

void mx_init_login_window(t_client *client) {
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("client/templates/login_window.ui");
    if (!builder)
        g_critical("Builder getting error!");
    gtk_builder_connect_signals(builder, NULL);

    client->ui->login_window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    if (!client->ui->login_window)
        g_critical("Can't get login_window");

    client->ui->show_registration_btn = GTK_WIDGET(gtk_builder_get_object(builder,"show_registration_btn"));
    if (!client->ui->show_registration_btn)
        g_critical("Can't get show_registration_btn");
    g_signal_connect(G_OBJECT(client->ui->show_registration_btn), "clicked", G_CALLBACK(switch_form), client);

    g_object_unref(builder);
}

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
    g_signal_connect(G_OBJECT(client->ui->show_login_btn), "clicked", G_CALLBACK(switch_form), client);

    g_object_unref(builder);
}

