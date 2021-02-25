#include "client.h"

static void new_msg_memfree(t_client *client);

void mx_proceed_newmsg_response(t_client *client, char *resp_str) {
    mx_parse_newmsg_response(client, resp_str);
    if (client->new_msg) {
        mx_show_new_msg(client, client->new_msg);
    }

    new_msg_memfree(client);
}

static void new_msg_memfree(t_client *client) {
    if (client) {
        free(client->new_msg->msg);
        client->new_msg->msg = NULL;

        free(client->new_msg->timestamp);
        client->new_msg->timestamp = NULL;
        
        free(client->new_msg);
        client->new_msg = NULL;
    }
}
