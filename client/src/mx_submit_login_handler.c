#include "client.h"

void mx_submit_login_handler(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client *)data;
    t_raw_inputs inputs = {NULL, NULL, NULL, NULL, NULL};
    bool is_err = 0;
    client->auth_req = NULL;

    if (!gtk_widget_get_sensitive(client->ui->login_btn))
        return;

    is_err = mx_get_input_values(client, &inputs, client->scene);
    if (is_err) {
        return;
    }
    gtk_widget_set_sensitive(client->ui->login_btn, 0);
    gtk_widget_set_sensitive(client->ui->show_registration_btn, 0);

    mx_create_auth_req(client, &inputs, client->scene);

    if (pthread_create(&client->auth_th, NULL, mx_do_auth_th, (void *)client) != 0) {
        write(STDERR_FILENO, RECV_TH_ERR, sizeof(RECV_TH_ERR));
    }

    gtk_widget_set_sensitive(client->ui->login_btn, 1);
    gtk_widget_set_sensitive(client->ui->show_registration_btn, 1);
}
