#include "client.h"

static void free_found_users(t_client *client);

void mx_proceed_search_response(t_client *client, char *resp_str) {
    // to get rid of previous found results
    mx_delete_old_rows(client, (GtkListBox *)client->ui->users_list);
    free_found_users(client);

    mx_parse_search_response(client, resp_str);

    mx_show_found_users(client);
}

static void free_found_users(t_client *client) {
    t_self **runner = client->found_users;

    if (runner && *runner){
        for (int i = 0; runner[i]; i++) {

            if (runner[i]->username) {
                free(runner[i]->username);
                runner[i]->username = NULL;
            }
            if (runner[i]->first_name) {
                free(runner[i]->first_name);
                runner[i]->first_name = NULL;
            }
            if (runner[i]->last_name) {
                free(runner[i]->last_name);
                runner[i]->last_name = NULL;
            }
            free(runner[i]);
            runner[i] = NULL;
        }
    }
    free(client->found_users);
    client->found_users = NULL;
}
