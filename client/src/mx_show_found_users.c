#include "client.h"

static GtkWidget *create_user_row(t_self *cur_rslt);

static void check_enter(void);
static int ch = 0;

void mx_show_found_users(t_client *client) {

    GtkWidget *row = NULL;

    for (int i = 0; client->found_users && client->found_users[i]; i++) {
        row = create_user_row(client->found_users[i]);

        gtk_list_box_insert((GtkListBox *)client->ui->users_list, row, -1);
    }
    gtk_widget_show_all(GTK_WIDGET(client->ui->users_list));

}

static GtkWidget *create_user_row(t_self *cur_rslt) {
    //  full_name[50] = "";
    GtkWidget *row = gtk_list_box_row_new();

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    GtkWidget *username_label = gtk_label_new(cur_rslt->username);

    gtk_widget_set_size_request(box, 280, 20);

    gtk_label_set_xalign(GTK_LABEL(username_label), 0.1);

    gtk_label_set_line_wrap(GTK_LABEL(username_label), TRUE);

    gtk_box_pack_start(GTK_BOX(box), username_label, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(row), box);

    return row;
}
