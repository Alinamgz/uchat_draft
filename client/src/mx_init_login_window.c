#include "client.h"

static void init_widgets(GtkBuilder *builder, t_client *client);
static void connect_signals(t_client *client);

void mx_init_login_window(t_client *client) {

    //---------- CSS Style ------------//
    GtkCssProvider *provider = gtk_css_provider_new();
    char *style = CHAT_LOGIN_CSS;

    gtk_css_provider_load_from_path(provider, style, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(provider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    client->ui->login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(client->ui->login_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_resizable(GTK_WINDOW(client->ui->login_window), FALSE);
    gtk_window_set_position(GTK_WINDOW(client->ui->login_window), GTK_WIN_POS_CENTER);

    GtkWidget *fixed = gtk_fixed_new();
    gtk_widget_set_name(fixed, "fixed");
    gtk_container_add(GTK_CONTAINER(client->ui->login_window), fixed);
    gtk_widget_show_all(fixed);


    gtk_widget_show_all(client->ui->login_window);
    //---------------------------------//

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

// ----------- window ---------------
    client->ui->login_window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
    if (!client->ui->login_window)
        g_critical("Can't get login_window");

// ----------- username ---------------
    client->ui->l_username_entry = GTK_WIDGET(gtk_builder_get_object(builder, "l_username_entry"));
    if (!client->ui->l_username_entry)
        g_critical("Can't get l_username_entry");

    client->ui->l_username_status = GTK_WIDGET(gtk_builder_get_object(builder, "l_username_status"));
    if (!client->ui->l_username_status)
        g_critical("Can't get l_username_status");

// ----------- password ---------------
    client->ui->l_pass_entry = GTK_WIDGET(gtk_builder_get_object(builder, "l_pass_entry"));
    if (!client->ui->l_pass_entry)
        g_critical("Can't get l_pass_entry");

    client->ui->l_pass_status = GTK_WIDGET(gtk_builder_get_object(builder, "l_pass_status"));
    if (!client->ui->l_pass_status)
        g_critical("Can't get l_pass_status");

// ----------- submit btn ---------------
    client->ui->login_btn = GTK_WIDGET(gtk_builder_get_object(builder, "login_btn"));
    if (!client->ui->login_btn)
        g_critical("Can't get login_btn");

// ----------- change form btn ---------------
    client->ui->show_registration_btn = GTK_WIDGET(gtk_builder_get_object(builder,"show_registration_btn"));
    if (!client->ui->show_registration_btn)
        g_critical("Can't get show_registration_btn");

}

static void connect_signals(t_client *client) {
// ------------------ window --------------------
    g_signal_connect(G_OBJECT(client->ui->login_window),
                              "destroy",
                              G_CALLBACK(exit_gtk),
                              client);

// ------------------ entry --------------------
    g_signal_connect(G_OBJECT(client->ui->l_username_entry),
                              "activate",
                              G_CALLBACK(mx_submit_login_handler),
                              client);

    g_signal_connect(G_OBJECT(client->ui->l_pass_entry),
                              "activate",
                              G_CALLBACK(mx_submit_login_handler),
                              client);

// ------------------ btns --------------------
    g_signal_connect(G_OBJECT(client->ui->login_btn),
                              "clicked",
                              G_CALLBACK(mx_submit_login_handler),
                              client);

    g_signal_connect(G_OBJECT(client->ui->show_registration_btn),
                              "clicked",
                              G_CALLBACK(mx_switch_form),
                              client);
}
