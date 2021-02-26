#include "client.h"

static void free_found_history(t_client *client);

void mx_proceed_history_response(t_client *client, char *resp_str) {
    mx_delete_old_rows(client, (GtkListBox *)client->ui->msg_list);

    free_found_history(client);

    mx_parse_history_response(client, resp_str);
printf("nu sho\n");
    for (int i = 0; client->msg_arr && client->msg_arr[i]; i++) {
        mx_show_new_msg(client, client->msg_arr[i]);
    }

    gtk_widget_show_all(GTK_WIDGET(client->ui->msg_list));
}

static void free_found_history(t_client *client) {
    t_msgs **runner = client->msg_arr;

    if (runner && *runner) {
        for(int i = 0; runner[i]; i++) {
            if(runner[i]->msg) {
                free(runner[i]->msg);
                runner[i]->msg = NULL;
            }
            if(runner[i]->timestamp) {
                free(runner[i]->timestamp);
                runner[i]->timestamp = NULL;
            }
        free(runner[i]);
        runner[i] = NULL;
        }

        free(client->msg_arr);
        client->msg_arr = NULL;
    }
}
