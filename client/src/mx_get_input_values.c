#include "client.h"

// void mx_get_input_values(GtkWidget *widget, gpointer data) {
//     t_client *client = (t_client*)data;
//     const gchar *l_username;
//     const gchar *l_password;
//     char *err = NULL;
//     if(!gtk_widget_get_sensitive(client->ui->login_btn))
//         return;

//     l_username = gtk_entry_get_text(GTK_ENTRY(client->ui->l_username_entry));
//     err = mx_validate_input(l_username, TRUE);
//     gtk_label_set_text(GTK_LABEL(client->ui->l_username_status), err);
//     if (err[0]) {
//         gtk_widget_grab_focus(client->ui->l_username_entry);
//         return;
//     }

//     l_password = gtk_entry_get_text(GTK_ENTRY(client->ui->l_pass_entry));
//     err = mx_validate_input(l_password, TRUE);
//     gtk_label_set_text(GTK_LABEL(client->ui->l_pass_status), err);
//     if (err[0]) {
//         gtk_widget_grab_focus(client->ui->l_pass_entry);
//         return;
//     }

//     gtk_widget_set_sensitive(client->ui->login_btn, 0);
//     gtk_widget_set_sensitive(client->ui->show_registration_btn, 0);
//     printf("\n-------------------\nLogin: %s\nPassw: %s\n-------------------\n", l_username, l_password);

// }

void mx_get_input_values(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client*)data;
    char *err = NULL;

    const gchar *username;
    const gchar *password;

    const gchar *conf_password;
    const gchar *first_name;
    const gchar *last_name;

    if(!gtk_widget_get_sensitive(client->ui->login_btn))
        return;
// TODO: finish adding register handlers, check if register ID's are ok
    if (widget == client->ui->login_btn){
        username = get_n_check(&err, TRUE, client->ui->l_pass_entry);
    }
    else if (widget == client->ui->register_btn){
        username = get_n_check(&err, TRUE, client->ui->r_pass_entry);
    }

    if (err[0]) {
        gtk_widget_grab_focus(client->ui->l_username_entry);
        return;
    }

    password = gtk_entry_get_text(GTK_ENTRY(client->ui->l_pass_entry));
    err = mx_validate_input(l_password, TRUE);
    gtk_label_set_text(GTK_LABEL(client->ui->l_pass_status), err);
    if (err[0]) {
        gtk_widget_grab_focus(client->ui->l_pass_entry);
        return;
    }

    gtk_widget_set_sensitive(client->ui->login_btn, 0);
    gtk_widget_set_sensitive(client->ui->show_registration_btn, 0);
    printf("\n-------------------\nLogin: %s\nPassw: %s\n-------------------\n", l_username, l_password);

}

static const gchar *get_n_check(char **err, bool is_req, gpointer entry, gpointer status) {
    const gchar *rslt = gtk_entry_get_text(GTK_ENTRY(entry));

    *err = mx_validate_input(rslt, is_req);
    gtk_label_set_text(GTK_LABEL(status), *err);

    return rslt;
}
