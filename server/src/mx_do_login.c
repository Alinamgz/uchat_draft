#include "server.h"

static int login_callback(void *data, int argc, char **argv, char **azColName) {
    t_auth_req *req_parsed = (t_auth_req *)data;

    if (argc == 4 && !strcmp(azColName[0], "password")) {
        req_parsed->res_code =
            strcmp(argv[0], req_parsed->password) ? UNAUTHORIZED : OK;
        // -----------------------------------------------------
        // NB! columns in azColName are in the same order as listed in SELECT
        // statement.
        //     if we SELECT *, coumns are in the order from CREATE TABLE
        //     statement
        // -----------------------------------------------------

        if (req_parsed->res_code == OK) {
            req_parsed->uid = atoi(argv[1]);
            req_parsed->first_name = strdup(argv[2]);
            req_parsed->last_name = strdup(argv[3]);
        }
    }

    return 0;
}

// static int select_chats_callback(void *data, int argc, char **argv, char **azColName) {

//     t_list *cur_client = (t_list *)data;

//     cur_client->chat_req_res[cur_client->cur_row] =(t_chat_req_res*)malloc(sizeof(t_chat_req_res));
//     memset(cur_client->chat_req_res[cur_client->cur_row], 0, sizeof(cur_client->chat_req_res));

//         printf("\n------------\nrow %d\n", cur_client->cur_row);
//     for (int i = 0; i <= argc; i++) {
//         if (!strcmp(azColName[i], "chat_id")) {
//             cur_client->chat_req_res[cur_client->cur_row]->chat_id = strdup(argv[i]);
//             // printf("chat_id: %s\n", cur_client->chat_req_res[cur_client->cur_row]->chat_id);

//         }
//         else if (!strcmp(azColName[i], "chat_name")) {
//             cur_client->chat_req_res[cur_client->cur_row]->name_or_msg = strdup(argv[i]);
//             // printf("name or string: %s\n", cur_client->chat_req_res[cur_client->cur_row]->name_or_msg);
//         }
//         else if (!strcmp(azColName[i], "from_uid")) {
//             cur_client->chat_req_res[cur_client->cur_row]->from_uid = strdup(argv[i]);
//             // printf("from_uid: %s\n", cur_client->chat_req_res[cur_client->cur_row]->from_uid);
//         }
//         else if (!strcmp(azColName[i], "to_uid")) {
//             cur_client->chat_req_res[cur_client->cur_row]->to_uid = strdup(argv[i]);
//             // printf("to_uid: %s\n", cur_client->chat_req_res[cur_client->cur_row]->to_uid);
//         }
//     }

//     cur_client->chat_req_res[++cur_client->cur_row] = NULL;

//     return 0;
// }

// static int count_callback(void *data, int argc, char **argv, char **azColName) {
//     if (argc && azColName) {
//         int *rows_cnt = (int *)data;
//         *rows_cnt = atoi(argv[0]);
//     }

//     return 0;
// }

void mx_do_login(sqlite3 *db, t_list *cur_client) {
    char sql[128] = "";
    char *err = NULL;
    // int rows = 0;

    memset(sql, 0, 128);
    sprintf(sql,
            "SELECT password, uid, first_name, last_name FROM users WHERE user_name = '%s';",
            cur_client->auth_req_res->username);

    if (sqlite3_exec(db, sql, login_callback, cur_client->auth_req_res, &err)) {
        cur_client->auth_req_res->res_code = INTERNAL_SRVR_ERR;
        fprintf(stderr, "select login from db: %s\n", sqlite3_errmsg(db));
        free(err);
    } else if (cur_client->auth_req_res->res_code == OK) {
        cur_client->uid = cur_client->auth_req_res->uid;
        strcpy(cur_client->name, cur_client->auth_req_res->username);

        mx_select_chats(db, cur_client);
        // memset(sql, 0, 128);
        // sprintf(sql,
        //         "SELECT COUNT(*) FROM chats WHERE from_uid = '%d' OR  to_uid = "
        //         "'%d';",
        //         cur_client->uid, cur_client->uid);

        // if (sqlite3_exec(db, sql, count_callback, &cur_client->rows_cnt,
        //                  &err)) {
        //     cur_client->auth_req_res->res_code = INTERNAL_SRVR_ERR;
        //     fprintf(stderr, "select login from db: %s\n", sqlite3_errmsg(db));
        //     free(err);
        // }

        // rows = cur_client->rows_cnt;

        // cur_client->chat_req_res = (t_chat_req_res **)malloc(sizeof(t_chat_req_res *));
        // cur_client->cur_row = 0;
        // // cur_client->chat_req_res[cur_client->cur_row] = NULL;

//         memset(sql, 0, 128);
//         sprintf(sql,
//                 "SELECT chat_id, chat_name, from_uid, to_uid FROM chats WHERE "
//                 "from_uid = '%d' OR  to_uid = '%d';",
//                 cur_client->uid, cur_client->uid);

//         if (sqlite3_exec(db, sql, select_chats_callback,
//                          cur_client, &err)) {
//             cur_client->auth_req_res->res_code = INTERNAL_SRVR_ERR;
//             fprintf(stderr, "select login from db: %s\n", sqlite3_errmsg(db));
//             free(err);
//         } else {
// printf("total rows: %d\n", cur_client->rows_cnt);
//             for (int i = 0; i < cur_client->cur_row; i++) {

//                 printf("\n-----------after sql json iter %d ----------------\n", i);
//                 printf("id: %s, from: %s, to: %s, name: %s\n",
//                         cur_client->chat_req_res[i]->chat_id,
//                         cur_client->chat_req_res[i]->from_uid,
//                         cur_client->chat_req_res[i]->to_uid,
//                         cur_client->chat_req_res[i]->name_or_msg);
//             }
//             printf("-----\t---- guess sho-to poluchilos\n");
//         }
    }
}
