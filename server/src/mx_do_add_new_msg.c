#include "server.h"

void mx_do_add_new_msg(sqlite3 *db, t_list *cur_client, t_chat_req_res *new_msg, bool is_sent) {
    char sql[256] = "";
    char *err;
    memset(sql, 0, 256);

    sprintf(sql,
            "INSERT INTO messages (chat_id, from_uid, to_uid, msg, is_sent, "
            "is_delivered) VALUES ('%s', '%s', '%s', '%s', '%d', '%d');",
            new_msg->chat_id,
            new_msg->from_uid,
            new_msg->to_uid,
            new_msg->name_or_msg,
            (int)is_sent, 0);

    if (sqlite3_exec(db, sql, NULL, NULL, &err)) {
            fprintf(stderr, "Adding new msg err: %s\n", sqlite3_errmsg(db));
            free(err);
            cur_client->res_code = INTERNAL_SRVR_ERR;
    }
    else {
        cur_client->res_code = OK;
    }
}
