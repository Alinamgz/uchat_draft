#include "server.h"

void mx_proceed_history_req(char *buf, t_list *cur_client, t_cl_data *client) {
    char *resp = NULL;
    mx_do_history(client->db, cur_client, buf);

    resp = mx_create_history_response(cur_client);
    mx_set_receivers(cur_client, cur_client->uid, -1);
    mx_send_response(resp, cur_client, client);
    mx_chat_req_memfree(cur_client);

    free(resp);
}
