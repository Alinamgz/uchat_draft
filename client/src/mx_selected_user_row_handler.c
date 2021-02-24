#include "client.h"

static bool already_have_chat(t_client *clietn);
static void free_selected_user(t_client *client);

void mx_selected_user_row_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {
    t_client *client = (t_client*)user_data;
    gint row_ind = -1;
    char *cur_name = NULL;

    if (box && row) {
        if (!gtk_list_box_row_is_selected(row)
            || (row_ind = gtk_list_box_row_get_index(row)) < 0) {
            printf("\nERR: can't check if row is selected OR get row index\n");
        }
        else {
            if(client->selected_user) {
                free_selected_user(client);
            }

            client->selected_user = (t_self*)malloc(sizeof(t_self));

            client->selected_user->uid = client->found_users[row_ind]->uid;
            client->selected_user->username = strdup(client->found_users[row_ind]->username);
            client->selected_user->first_name = strdup(client->found_users[row_ind]->first_name);
            client->selected_user->last_name = strdup(client->found_users[row_ind]->last_name);


            if (!already_have_chat(client))
                mx_create_chats_req(client,
                                    client->selected_user->uid,
                                    client->selected_user->username);
        }
    }
}

static bool already_have_chat(t_client *client) {
    bool rslt = 0;
    GtkListBox *chats_list = (GtkListBox*)client->ui->chats_list;
    GtkListBoxRow *row_in_chats = NULL;

    for (int i = 0; client->chats && client->chats[i]; i++) {
        if (client->chats[i]->to_uid == client->selected_user->uid) {
            row_in_chats = gtk_list_box_get_row_at_index(chats_list, i);
            gtk_list_box_select_row(chats_list, row_in_chats);

            printf("already have chat for %d and %d\n",
                    client->selected_user->uid,
                    client->self->uid);

            return 1;
        }
    }

    return rslt;
}

static void free_selected_user(t_client *client) {
    free(client->selected_user->username);
    client->selected_user->username = NULL;

    free(client->selected_user->first_name);
    client->selected_user->first_name = NULL;

    free(client->selected_user->last_name);
    client->selected_user->last_name = NULL;

    free(client->selected_user);
    client->selected_user = NULL;
}
