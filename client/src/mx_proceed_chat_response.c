#include "client.h"

static void free_found_chats(t_client *client);

void mx_proceed_chat_response(t_client *client, char *resp_str) {
    // to get rid of previous found results
    mx_delete_old_rows(client, (GtkListBox *)client->ui->chats_list);
    free_found_chats(client);

    mx_parse_chats_response(client, resp_str);

    mx_show_chats(client);
}

static void free_found_chats(t_client *client) {
    t_chats **runner = client->chats;

    if (runner && *runner){
        for (int i = 0; runner[i]; i++) {

            if (runner[i]->chat_name) {
                free(runner[i]->chat_name);
                runner[i]->chat_name = NULL;
            }
            free(runner[i]);
            runner[i] = NULL;
        }
    }
    free(client->found_users);
    client->found_users = NULL;
}
