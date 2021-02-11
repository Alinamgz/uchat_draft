#include "client.h"

static const gchar *get_n_check(char **err, bool is_req, gpointer entry, gpointer status);

void mx_get_input_values(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client*)data;
    char *err = NULL;

    const gchar *username;
    const gchar *password;

    const gchar *conf_password;
    // const gchar *first_name;
    // const gchar *last_name;

    if(!gtk_widget_get_sensitive(client->ui->login_btn))
        return;
// TODO: finish adding register handlers, check if register ID's are ok
    if (widget == client->ui->login_btn){
        username = get_n_check(&err,
                               TRUE,
                               client->ui->l_username_entry,
                               client->ui->l_username_status);
        if (err[0])
            return;

        password = get_n_check(&err,
                               TRUE,
                               client->ui->l_pass_entry,
                               client->ui->l_pass_status);
        if (err[0])
            return;
    }
    else if (widget == client->ui->register_btn){
        username = get_n_check(&err,
                               TRUE,
                               client->ui->r_username_entry,
                               client->ui->r_username_status);
        if (err[0])
            return;

        password = get_n_check(&err,
                               TRUE,
                               client->ui->r_pass_entry,
                               client->ui->r_pass_status);
        if (err[0])
            return;

        conf_password = get_n_check(&err,
                               TRUE,
                               client->ui->conf_pass_entry,
                               client->ui->conf_pass_status);
        if (strcmp(password, conf_password)) {
            gtk_label_set_text(GTK_LABEL(client->ui->conf_pass_status), "Passwords don't match");
            gtk_widget_grab_focus(client->ui->conf_pass_entry);
            return;
        }

        if (err[0])
            return;
    }

    gtk_widget_set_sensitive(client->ui->login_btn, 0);
    gtk_widget_set_sensitive(client->ui->show_registration_btn, 0);
    printf("\n-------------------\nLogin: %s\nPassw: %s\nConf: %s\n-------------------\n", username, password, conf_password);
    gtk_widget_set_sensitive(client->ui->login_btn, 1);
    gtk_widget_set_sensitive(client->ui->show_registration_btn, 1);

}

static const gchar *get_n_check(char **err, bool is_req, gpointer entry, gpointer status) {
    const gchar *rslt = gtk_entry_get_text(GTK_ENTRY(entry));

    *err = mx_validate_input(rslt, is_req);
    gtk_label_set_text(GTK_LABEL(status), *err);

    if (err[0])
        gtk_widget_grab_focus(entry);
    // return;

    return rslt;
}
