#include "server.h"

static int count_found_msgs(sqlite3 *db, t_list *cur_client, int search_str);
static int count_callback(void *data, int argc, char **argv, char **azColName);
static int history_callback(void *data, int argc, char **argv, char **azColName);

void mx_do_history(sqlite3 *db, t_list *cur_client, char *search_str) {
    cJSON *res = cJSON_Parse(search_str); 
    cJSON *chat_id = cJSON_GetObjectItemCaseSensitive(res, "chat_id");

    char sql[128] = "";
    char *err = NULL;
    int rows = 0;
    cur_client->cur_row = 0;
    
    rows = count_found_msgs(db, cur_client, chat_id->valueint);

    if (rows > 0) {
        cur_client->history_req_res = (t_chat_req_res**)calloc(1 + rows, sizeof(t_chat_req_res*));

        memset(sql, 0, 128);
        sprintf(sql, "SELECT chat_id, from_uid, to_uid, msg, timestamp FROM messages WHERE chat_id = '%d';", chat_id->valueint);
        
        if (sqlite3_exec(db, sql, history_callback, cur_client, &err)) {
            cur_client->res_code = INTERNAL_SRVR_ERR;
            fprintf(stderr, "count messages from db: %s\n", sqlite3_errmsg(db));
            free(err);
        }
        else if (cur_client->history_req_res) {
            cur_client->res_code = OK;
        }
    }
    else {
        cur_client->res_code = NOT_FOUND;
    }

    cJSON_Delete(res);

}

static int count_callback(void *data, int argc, char **argv, char **azColName) {
    if (argc && azColName) {
        int *rows_cnt = (int *)data;
        *rows_cnt = atoi(argv[0]);
    }

    return 0;
}

static int count_found_msgs(sqlite3 *db, t_list *cur_client, int search_str) {
    char sql[128] = "";
    char *err = NULL;
    cur_client->rows_cnt = 0;

;
    memset(sql, 0, 128);
    sprintf(sql, "SELECT COUNT(*) FROM messages WHERE chat_id = '%d';", search_str);

    if (sqlite3_exec(db, sql, count_callback, &cur_client->rows_cnt, &err)) {
        cur_client->res_code = INTERNAL_SRVR_ERR;
        fprintf(stderr, "select messages from db: %s\n", sqlite3_errmsg(db));
        free(err);
    }

    fflush(stdout);
    return cur_client->rows_cnt;
}



static int history_callback(void *data, int argc, char **argv, char **azColName) {
    t_list *cur_client = (t_list *)data;
    cur_client->history_req_res ? printf("mimo, %d\n", cur_client->cur_row) : printf("opa! %d  \n", cur_client->cur_row);

    cur_client->history_req_res[cur_client->cur_row] =(t_chat_req_res*)malloc(sizeof(t_chat_req_res));
    memset(cur_client->history_req_res[cur_client->cur_row], 0, sizeof(cur_client->history_req_res));

    for (int i = 0; i <= argc; i++) {
        if (!strcmp(azColName[i], "chat_id")) {
            cur_client->history_req_res[cur_client->cur_row]->chat_id = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "msg")) {
            cur_client->history_req_res[cur_client->cur_row]->name_or_msg = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "from_uid")) {
            cur_client->history_req_res[cur_client->cur_row]->from_uid = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "to_uid")) {
            cur_client->history_req_res[cur_client->cur_row]->to_uid = strdup(argv[i]);
        }
        else if (!strcmp(azColName[i], "timestamp")) {
            cur_client->history_req_res[cur_client->cur_row]->timestamp = strdup(argv[i]);
        }
    }

    cur_client->history_req_res[++cur_client->cur_row] = NULL;


printf("vanGogi bye\n");
    fflush(stdout);
    return 0;
}

