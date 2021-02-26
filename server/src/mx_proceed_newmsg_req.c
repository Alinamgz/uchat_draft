#include "server.h"

static t_chat_req_res *parse_newmsg_req(const char *req_str, bool *is_sent);
static void new_msg_memfree(t_chat_req_res **new_msg);

void mx_proceed_newmsg_req(char *buf, t_list *cur_client, t_cl_data *client) {
    if (cur_client && client)
        printf("\n---------\nMsg req: %s\n----------\n", buf);

    bool is_sent = 0;
    t_chat_req_res *new_msg = parse_newmsg_req(buf, &is_sent);

    if (new_msg) {
        printf("New msg: chat %s, from %s, to %s\n", new_msg->chat_id, new_msg->from_uid, new_msg->to_uid);
        // mx_do_add_new_msg(client->db, cur_client, new_msg, is_sent);
    }

    new_msg_memfree(&new_msg);
}

static t_chat_req_res *parse_newmsg_req(const char *req_str, bool *is_sent) {
    t_chat_req_res *rslt = NULL;

    cJSON *req = cJSON_Parse(req_str);
    cJSON *chat_id = cJSON_GetObjectItemCaseSensitive(req, "chat_id");
    cJSON *from_uid = cJSON_GetObjectItemCaseSensitive(req, "from_uid");
    cJSON *to_uid = cJSON_GetObjectItemCaseSensitive(req, "to_uid");
    cJSON *msg = cJSON_GetObjectItemCaseSensitive(req, "message");
    cJSON *sent = cJSON_GetObjectItemCaseSensitive(req, "is_sent");

    if (req && chat_id && from_uid && to_uid && msg && is_sent) {
        rslt = (t_chat_req_res*)malloc(sizeof(t_chat_req_res));
        rslt->chat_id = strdup(chat_id->valuestring);
        rslt->from_uid = strdup(from_uid->valuestring);
        rslt->to_uid = strdup(to_uid->valuestring);
        rslt->name_or_msg = strdup(msg->valuestring);

        *is_sent = sent->valueint ? 1 : 0;
    }

    cJSON_Delete(req);
    return rslt;
}

static void new_msg_memfree(t_chat_req_res **new_msg) {
    t_chat_req_res *runner = *new_msg;

    if (runner) {
        if (runner->chat_id) {
            free(runner->chat_id);
            runner->chat_id = NULL;
        }
        if (runner->from_uid) {
            free(runner->from_uid);
            runner->from_uid = NULL;
        }
        if (runner->to_uid) {
            free(runner->to_uid);
            runner->to_uid = NULL;
        }
        if (runner->name_or_msg) {
            free(runner->name_or_msg);
            runner->name_or_msg = NULL;
        }

        free(*new_msg);
        *new_msg = NULL;
    }
}
