#include "server.h"

void mx_chat_req_memfree(t_list *cur_client) {
    t_chat_req_res **runner = cur_client->chat_req_res;

    if (runner){
        for (int i = 0; runner[i]; i++) {
            if (runner[i]->chat_id) {
                free(runner[i]->chat_id);
                runner[i]->chat_id = NULL;
            }
            if (runner[i]->name_or_msg) {
                free(runner[i]->name_or_msg);
                runner[i]->name_or_msg = NULL;
            }
            if (runner[i]->from_uid) {
                free(runner[i]->from_uid);
                runner[i]->from_uid = NULL;
            }
            if (runner[i]->to_uid) {
                free(runner[i]->to_uid);
                runner[i]->to_uid = NULL;
            }
            free(runner[i]);
            runner[i] = NULL;
        }
    }
    free(cur_client->chat_req_res);
    cur_client->chat_req_res = NULL;
}
