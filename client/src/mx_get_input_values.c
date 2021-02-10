#include "client.h"

void mx_get_input_values(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client*)data;
    const gchar *l_username;
    const gchar *l_password;
    char *err = NULL;
    if(!gtk_widget_get_sensitive(client->ui->login_btn))
        return;

    l_username = gtk_entry_get_text(GTK_ENTRY(client->ui->l_username_entry));
    err = mx_validate_input(l_username, TRUE);

    gtk_label_set_text(GTK_LABEL(client->ui->l_username_status), err);
    if (err[0]) {
        gtk_widget_grab_focus(client->ui->l_username_entry);
        return;
    }

    l_password = gtk_entry_get_text(GTK_ENTRY(client->ui->l_pass_entry));
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
