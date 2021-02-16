#include "client.h"

void mx_submit_registration_handler(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client *)data;

    t_raw_inputs inputs = {NULL, NULL, NULL, NULL, NULL};
    bool is_err = 0;

    if (!gtk_widget_get_sensitive(client->ui->register_btn))
        return;

    is_err = mx_get_input_values(client, &inputs, client->scene);
    if (is_err) {
        return;
    }

    if (strcmp(inputs.password, inputs.conf_password)) {
        gtk_label_set_text(GTK_LABEL(client->ui->conf_pass_status), "Passwords don't match");
        gtk_widget_grab_focus(client->ui->conf_pass_entry);
        return;
    }

    gtk_widget_set_sensitive(client->ui->register_btn, 0);
    gtk_widget_set_sensitive(client->ui->show_login_btn, 0);

    mx_create_auth_req(client, &inputs, client->scene);

    if (pthread_create(&client->auth_th, NULL, mx_do_auth_th, (void *)client) != 0) {
        write(STDERR_FILENO, RECV_TH_ERR, sizeof(RECV_TH_ERR));
    }

    gtk_widget_set_sensitive(client->ui->register_btn, 1);
    gtk_widget_set_sensitive(client->ui->show_login_btn, 1);
}
