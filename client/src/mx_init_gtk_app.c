#include "client.h"

// static void init_login();
// static void init_err();
//  void exit_gtk();
static void con_retry();
// static gboolean check_connection();
//  void chck_clk();
static gboolean check_scene(void *param);

void mx_init_gtk_app(t_client *client) {
    gtk_init(NULL, NULL);

    // init_err(client);
    // init_login(client);
    mx_init_error_dialog(client);
	gtk_label_set_text(GTK_LABEL(client->ui->fail_reason_msg), "Trying to reach...");
    // client->scene = CONNECTION_ERR;
    // gtk_widget_show_now(client->ui->err_dialog);


    mx_connect_retry_gtk(NULL, (gpointer*)client);
    // g_timeout_add(50, check_connection, client);
    g_timeout_add(50, check_scene, client);

    gtk_main();
}

// static void init_login(t_client *client) {
//     GtkBuilder *builder;

//     builder = gtk_builder_new_from_file("client/templates/login_window.ui");
//     if (!builder) g_critical("Builder getting error!");
//     gtk_builder_connect_signals(builder, NULL);

//     client->ui->login_window = GTK_WIDGET(gtk_builder_get_object(builder, "login_window"));
//     if (!client->ui->login_window) g_critical("Can't get login_window");

//     g_object_unref(builder);
// }

// static void init_err(t_client *client) {
//     GtkBuilder *builder;

//     builder = gtk_builder_new_from_file("client/templates/err_dialog.ui");
//     if (!builder) g_critical("Builder getting error!");
//     gtk_builder_connect_signals(builder, NULL);

//     client->ui->err_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "err_dialog"));
//     if (!client->ui->err_dialog) g_critical("Can't get err_dialog");
//     client->ui->fail_reason_msg = GTK_WIDGET(gtk_builder_get_object(builder, "fail_reason_msg"));
//     if (!client->ui->fail_reason_msg) g_critical("Can't get fail_reason_msg");

//     client->ui->retry_btn = GTK_WIDGET(gtk_builder_get_object(builder, "retry_btn"));
//     if (!client->ui->retry_btn) g_critical("Can't get fail_reason_msg");

//     g_signal_connect(G_OBJECT(client->ui->retry_btn), "clicked", G_CALLBACK(chck_clk), client);
//     // g_signal_connect(G_OBJECT(client->ui->retry_btn), "clicked", G_CALLBACK(mx_connect_retry_gtk), client);

//     while(gtk_events_pending())
// 	    gtk_main_iteration();
//     // g_signal_connect(G_OBJECT(client->ui->retry_btn), "clicked", G_CALLBACK(chck_clk), client);
// }

//  void chck_clk(GtkWidget *widget, gpointer data) {
//     t_client *client = (t_client*)data;

// printf("====== Click ==== \n");
//     gtk_label_set_text(GTK_LABEL(client->ui->fail_reason_msg), "Retrying...");

//         while(gtk_events_pending())
// 	        gtk_main_iteration();
//             // gtk_widget_hide(client->ui->fail_reason_msg);
//             // gtk_widget_show_now(client->ui->fail_reason_msg);
// printf("====== Click_2 ==== \n");
//     client->scene = CONNECTION;
//     mx_connect_retry_gtk(widget, data);
// return;
//     // if(client && client->sock_fd) {printf("\n-- click param ---\n");
//     // }else printf("--- click NO param ---");
// }

 void exit_gtk(GtkWidget *widget, void *param) {
    // gtk_widget_hide(widget);
    gtk_main_quit();
    exit(0);
}

// static gboolean check_connection(void *param) {
//     t_client *client = (t_client*)param;
//     int fd = client->sock_fd;

//     if ( fd > 0) {
//         gtk_widget_hide(client->ui->err_dialog);
//         gtk_widget_show_all(client->ui->ok_window);
//         return G_SOURCE_REMOVE;
//     }
//     else {
//         gtk_widget_show_all(client->ui->err_dialog);
//         return G_SOURCE_CONTINUE;
//     }
// }

static gboolean check_scene(void *param) {
    t_client *client = (t_client*)param;
    t_scene scene = client->scene;

    switch(scene) {
        case CONNECTION:
            // gtk_widget_hide(client->ui->err_dialog);
            if (!client->ui->err_dialog) {
                mx_init_error_dialog(client);
            }
            gtk_widget_show_all(client->ui->err_dialog);
            return G_SOURCE_CONTINUE;
        case CONNECTION_ERR:
            // gtk_widget_hide(client->ui->err_dialog);
            if (!client->ui->err_dialog) {
                mx_init_error_dialog(client);
            }
            gtk_widget_show_all(client->ui->err_dialog);
            return G_SOURCE_CONTINUE;
        case LOGIN:
            if (client->ui->err_dialog)
                gtk_widget_hide(client->ui->err_dialog);
            if (client->ui->registration_window)
                gtk_widget_hide(client->ui->registration_window);
            if (!client->ui->login_window) {
                mx_init_login_window(client);
            }
            gtk_widget_show(client->ui->login_window);
            // gtk_widget_show_all(client->ui->login_window);
            return G_SOURCE_CONTINUE;
        case REGISTRATION:
            gtk_widget_hide(client->ui->login_window);
            // gtk_widget_hide(client->ui->registration_window);
             if (!client->ui->registration_window) {
                mx_init_registration_window(client);
            }
            // gtk_widget_show_all(client->ui->registration_window);
            gtk_widget_show(client->ui->registration_window);
            return G_SOURCE_CONTINUE;
        case CHAT:
            gtk_widget_hide(client->ui->login_window);
            gtk_widget_show_all(client->ui->login_window);
            return G_SOURCE_CONTINUE;
        default:
            gtk_main_quit();
            return G_SOURCE_REMOVE;
    }
}
