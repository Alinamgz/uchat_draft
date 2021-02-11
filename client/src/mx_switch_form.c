#include "client.h"

void mx_switch_form(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client*)data;

// printf("Scene: %d\n", client->scene);

    if (client->scene == LOGIN) {
        if (!client->ui->registration_window) {
            mx_init_registration_window(client);
        }
        client->prev_scene = client->scene;
        client->scene = REGISTRATION;
// printf("\tkuku\n");
    }
    else if (client->scene == REGISTRATION)  {
        client->prev_scene = client->scene;
        client->scene = LOGIN;
    }
}
