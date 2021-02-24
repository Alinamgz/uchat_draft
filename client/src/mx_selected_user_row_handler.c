#include "client.h"

void selected_user_row_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {
    t_client *client = (t_client*)user_data;
    gint row_ind = -1;
    char *cur_name = NULL;

    if (box && row) {
        if (gtk_list_box_row_is_selected(row) && (row_ind= gtk_list_box_row_get_index(row)) > -1) {

            if(client->selected_user) {
                free(client->selected_user);
            }
            // printf("\tch_01\n");
            client->selected_user = (t_self*)malloc(sizeof(t_self));

            // printf("\tch_02\n");
            client->selected_user->uid = client->found_users[row_ind]->uid;
            client->selected_user->username = strdup(client->found_users[row_ind]->username);
            client->selected_user->first_name = strdup(client->found_users[row_ind]->first_name);
            client->selected_user->last_name = strdup(client->found_users[row_ind]->last_name);
            // printf("\tch_03\n");
// ---------- TODO: kill it. find how to check and req for chat without repeat ------------------
bool have_chat = 0;
            for (int i = 0; client->chats && client->chats[i]; i++) {

            // printf("\tch_04\n");
            if (client->chats[i]->from_uid == client->selected_user->uid
                || client->chats[i]->to_uid == client->selected_user->uid) {

                    have_chat = 1;
                    break;
                }
            }
            // printf("\tch_05\n");


 have_chat ? printf("already have chat for %d and %d\n",
                client->selected_user->uid, client->self->uid)
            : printf("have to create new chat for %d and %d\n",
                client->selected_user->uid, client->self->uid);

// mx_create_chats_req(client, client->self->uid; client->selected_user->uid);
// ----------------------------------------------------------------------------
        }
        else {
            printf("\nERR: can't check if row is selected OR get row index\n");
        }
    }
}
