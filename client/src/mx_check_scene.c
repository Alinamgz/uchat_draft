#include "client.h"

gboolean mx_check_scene(void *param) {
    t_client *client = (t_client *)param;
    t_scene scene = client->scene;

    switch (scene) {
        case CONNECTION_ERR:
            client->prev_scene = client->scene;
            gtk_widget_show(client->ui->err_dialog);

            return G_SOURCE_CONTINUE;
        case LOGIN:
            if (client->scene != client->prev_scene) {
                if (client->prev_scene == CONNECTION_ERR) {
                    gtk_widget_hide(client->ui->err_dialog);
                }
                if (client->prev_scene == REGISTRATION) {
                    gtk_widget_hide(client->ui->registration_window);
                }

                client->prev_scene = client->scene;
                gtk_widget_show(client->ui->login_window);
                // gtk_widget_show_all(client->ui->login_window);
            }
            return G_SOURCE_CONTINUE;
        case REGISTRATION:
            if (client->scene != client->prev_scene) {
                if (client->prev_scene == LOGIN) {
                    gtk_widget_hide(client->ui->login_window);
                }
                // gtk_widget_show_all(client->ui->registration_window);
                client->prev_scene = client->scene;
                gtk_widget_show(client->ui->registration_window);
            }
            return G_SOURCE_CONTINUE;

        case CHAT:
            if (client->scene != client->prev_scene) {
                gtk_widget_hide(client->ui->login_window);
                gtk_widget_show_all(client->ui->login_window);
            }
            return G_SOURCE_CONTINUE;
        default:
            gtk_main_quit();
            return G_SOURCE_REMOVE;
    }
}
