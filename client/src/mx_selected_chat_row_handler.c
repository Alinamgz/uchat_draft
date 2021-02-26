#include "client.h"

void mx_selected_chat_row_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {
    t_client *client = (t_client*)user_data;
    gint row_ind = -1;
    int peer_uid = -1;
    char *cur_name = NULL;

    if (box && row) {
        if (gtk_list_box_row_is_selected(row)) {
            row_ind = gtk_list_box_row_get_index(row);
            if (row_ind > -1) {
                if(client->selected_chat) {
                    free(client->selected_chat);
                }

                    client->selected_chat = (t_chats*)malloc(sizeof(t_chats));

                    client->selected_chat->chat_name = client->chats[row_ind]->chat_name;
                    client->selected_chat->chat_id = client->chats[row_ind]->chat_id;
                    client->selected_chat->from_uid = client->self->uid;

                    peer_uid = client->self->uid != client->chats[row_ind]->to_uid
                             ? client->chats[row_ind]->to_uid
                             : client->chats[row_ind]->from_uid;
                    client->selected_chat->to_uid = peer_uid;

                cur_name = client->chats[row_ind]->chat_name;

                mx_get_peer_name(client, cur_name);

                gtk_label_set_text(GTK_LABEL(client->ui->selected_chat_name), cur_name);

            }
            else {
                printf("\nERR: can't get row index\n");
            }
        }
        else {
                printf("\nERR: can't check if row is selected\n");
        }
    }
}
