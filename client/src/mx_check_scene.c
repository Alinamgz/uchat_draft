#include "client.h"

gboolean mx_check_scene(void *param) {
    t_client *client = (t_client*)param;
    t_scene scene = client->scene;

    switch(scene) {
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
