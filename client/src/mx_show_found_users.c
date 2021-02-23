#include "client.h"

static GtkWidget *create_chat_row(t_self *cur_rslt);
static void user_row_selected_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data);

void mx_show_found_users(t_client *client) {

    GtkWidget *row = NULL;

    for (int i = 0; client->found_users && client->found_users[i]; i++) {
        row = create_chat_row(client->found_users[i]);
        g_signal_connect(G_OBJECT(client->ui->users_list),
                              "row-selected",
                              G_CALLBACK(user_row_selected_handler),
                              client);

        gtk_list_box_insert((GtkListBox *)client->ui->users_list, row, -1);
    }
    gtk_widget_show_all(GTK_WIDGET(client->ui->users_list));

}

static GtkWidget *create_chat_row(t_self *cur_rslt) {
    char full_name[50] = "";
    GtkWidget *row = gtk_list_box_row_new();

    memset(full_name, 0, 50);
    if (cur_rslt->first_name[0] && cur_rslt->last_name[0])
        sprintf(full_name, "%s %s", cur_rslt->first_name, cur_rslt->last_name);
    else if (cur_rslt->first_name[0])
        sprintf(full_name, "%s", cur_rslt->first_name);
    else if (cur_rslt->last_name[0])
        sprintf(full_name, "%s", cur_rslt->last_name);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *username_label = gtk_label_new(cur_rslt->username);
    GtkWidget *fullname_label = gtk_label_new(full_name);

    gtk_widget_set_size_request(box, 280, 20);

    gtk_label_set_xalign(GTK_LABEL(username_label), 0.1);
    gtk_label_set_xalign(GTK_LABEL(fullname_label), 0.2);

    gtk_label_set_line_wrap(GTK_LABEL(username_label), TRUE);

    gtk_box_pack_start(GTK_BOX(box), username_label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(box), fullname_label, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(row), box);

    return row;
}

static void user_row_selected_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {
    t_client *client = (t_client*)user_data;
    gint row_ind = -1;
    char *cur_name = NULL;

    if (box && row) {
        if (gtk_list_box_row_is_selected(row) && (row_ind= gtk_list_box_row_get_index(row)) > -1) {

            if(client->selected_user) {
                free(client->selected_user);
            }
            client->selected_user = (t_self*)malloc(sizeof(t_self));

            client->selected_user->uid = client->found_users[row_ind]->uid;
            client->selected_user->username = strdup(client->found_users[row_ind]->username);
            client->selected_user->first_name = strdup(client->found_users[row_ind]->first_name);
            client->selected_user->last_name = strdup(client->found_users[row_ind]->last_name);
// ---------- TODO: kill it. find how to check and req for chat without repeat ------------------
bool have_chat = 0;
            for (int i = 0; client->chats[i]; i++) {

            if (client->chats[i]->from_uid == client->selected_user->uid
                || client->chats[i]->to_uid == client->selected_user->uid) {
                    printf("have to create new chat for %d and %d\n",
                    client->selected_user->uid, client->self->uid);

                    printf("already have chat for %d and %d\n",
                    client->selected_user->uid, client->self->uid);
                    have_chat = 1;
                    break;
                    // mx_create_chats_req(client, client->self->uid; client->selected_user->uid);
                }
            }


 have_chat ? printf("already have chat for %d and %d\n",
                client->selected_user->uid, client->self->uid)
            : printf("have to create new chat for %d and %d\n",
                client->selected_user->uid, client->self->uid);
// ----------------------------------------------------------------------------
        }
        else {
            printf("\nERR: can't check if row is selected OR get row index\n");
        }
    }
}
