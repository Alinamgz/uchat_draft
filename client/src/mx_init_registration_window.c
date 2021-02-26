#include "client.h"

static void init_widgets(GtkBuilder *builder, t_client *client);
static void connect_signals(t_client *client);

void mx_init_registration_window(t_client *client) {
    //---------- CSS Style ------------//
    GtkCssProvider *provider = gtk_css_provider_new();
    char *style = CHAT_REG_CSS;

    gtk_css_provider_load_from_path(provider, style, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                               GTK_STYLE_PROVIDER(provider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    client->ui->registration_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(client->ui->registration_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_resizable(GTK_WINDOW(client->ui->registration_window), FALSE);
    gtk_window_set_position(GTK_WINDOW(client->ui->registration_window), GTK_WIN_POS_CENTER);

    GtkWidget *fixed = gtk_fixed_new();
    gtk_widget_set_name(fixed, "fixed");
    gtk_container_add(GTK_CONTAINER(client->ui->registration_window), fixed);
    gtk_widget_show_all(fixed);


    gtk_widget_show_all(client->ui->registration_window);
    //---------------------------------//
    GtkBuilder *builder;

    builder = gtk_builder_new_from_file("client/templates/reg_uchat.ui");
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
    client->ui->registration_window = GTK_WIDGET(gtk_builder_get_object(builder, "registration_window"));
    if (!client->ui->registration_window)
        g_critical("Can't get registration_window");

// ----------- username ---------------
    client->ui->r_username_entry = GTK_WIDGET(gtk_builder_get_object(builder, "r_username_entry"));
    if (!client->ui->r_username_entry)
        g_critical("Can't get r_username_entry");

    client->ui->r_username_status = GTK_WIDGET(gtk_builder_get_object(builder, "r_username_status"));
    if (!client->ui->r_username_status)
        g_critical("Can't get r_username_status");

// ----------- password ---------------
    client->ui->r_pass_entry = GTK_WIDGET(gtk_builder_get_object(builder, "r_pass_entry"));
    if (!client->ui->r_pass_entry)
        g_critical("Can't get r_pass_entry");

    client->ui->r_pass_status = GTK_WIDGET(gtk_builder_get_object(builder, "r_pass_status"));
    if (!client->ui->r_pass_status)
        g_critical("Can't get r_pass_status");

// ----------- confirm password ---------------
    client->ui->conf_pass_entry = GTK_WIDGET(gtk_builder_get_object(builder, "conf_pass_entry"));
    if (!client->ui->conf_pass_entry)
        g_critical("Can't get conf_pass_entry");

    client->ui->conf_pass_status = GTK_WIDGET(gtk_builder_get_object(builder, "conf_pass_status"));
    if (!client->ui->conf_pass_status)
        g_critical("Can't get conf_pass_status");

// ----------- firstname ---------------
    client->ui->firstname_entry = GTK_WIDGET(gtk_builder_get_object(builder, "firstname_entry"));
    if (!client->ui->firstname_entry)
        g_critical("Can't get firstname_entry");

    client->ui->firstname_status = GTK_WIDGET(gtk_builder_get_object(builder, "firstname_status"));
    if (!client->ui->firstname_status)
        g_critical("Can't get firstname_status");

// ----------- lastname ---------------
    client->ui->lastname_entry = GTK_WIDGET(gtk_builder_get_object(builder, "lastname_entry"));
    if (!client->ui->lastname_entry)
        g_critical("Can't get lastname_entry");

    client->ui->lastname_status = GTK_WIDGET(gtk_builder_get_object(builder, "lastname_status"));
    if (!client->ui->lastname_status)
        g_critical("Can't get lastname_status");

// ----------- submit btn ---------------
    client->ui->register_btn = GTK_WIDGET(gtk_builder_get_object(builder, "register_btn"));
    if (!client->ui->register_btn)
        g_critical("Can't get register_btn");

// ----------- change form btn ---------------
    client->ui->show_login_btn = GTK_WIDGET(gtk_builder_get_object(builder,"show_login_btn"));
    if (!client->ui->show_login_btn)
        g_critical("Can't get show_login_btn");

}

static void connect_signals(t_client *client) {
// ------------------ window --------------------
    g_signal_connect(G_OBJECT(client->ui->registration_window),
                              "destroy",
                              G_CALLBACK(exit_gtk),
                              client);

// ------------------ entry --------------------
    g_signal_connect(G_OBJECT(client->ui->r_username_entry),
                              "activate",
                              G_CALLBACK(mx_submit_registration_handler),
                              client);

    g_signal_connect(G_OBJECT(client->ui->r_pass_entry),
                              "activate",
                              G_CALLBACK(mx_submit_registration_handler),
                              client);
    g_signal_connect(G_OBJECT(client->ui->conf_pass_entry),
                              "activate",
                              G_CALLBACK(mx_submit_registration_handler),
                              client);

    g_signal_connect(G_OBJECT(client->ui->firstname_entry),
                              "activate",
                              G_CALLBACK(mx_submit_registration_handler),
                              client);

    g_signal_connect(G_OBJECT(client->ui->lastname_entry),
                              "activate",
                              G_CALLBACK(mx_submit_registration_handler),
                              client);

// ------------------ btns --------------------
    g_signal_connect(G_OBJECT(client->ui->register_btn),
                              "clicked",
                              G_CALLBACK(mx_submit_registration_handler),
                              client);

    g_signal_connect(G_OBJECT(client->ui->show_login_btn),
                              "clicked",
                              G_CALLBACK(mx_switch_form),
                              client);
}


