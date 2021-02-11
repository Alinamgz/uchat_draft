#include "client.h"

void mx_submit_registration_handler(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client *)data;

    t_raw_inputs inputs = {NULL, NULL, NULL, NULL, NULL};
    bool is_err = 0;
    cJSON *auth_req = NULL;

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

    auth_req = mx_create_auth_req(client, &inputs, client->scene);

    if (auth_req) printf("Kukusiki\n%s\n", cJSON_Print(auth_req));

    gtk_widget_set_sensitive(client->ui->register_btn, 1);
    gtk_widget_set_sensitive(client->ui->show_login_btn, 1);
}
