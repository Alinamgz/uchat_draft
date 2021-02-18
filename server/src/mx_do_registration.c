#include "server.h"

static int reqistration_callback(void *data, int argc, char **argv,
                                  char **azColName) {
    if (argc && argv && azColName) {
        *(int*)data = atoi(argv[0]);
    }

    printf("data: %s\n", data);
    return 0;
}

void mx_do_registration(sqlite3 *db, t_auth_req *req_parsed) {
    char sql[256] = "";
    char *err = NULL;
    int new_uid = -1;

    memset(sql, 0, 256);
    sprintf(sql,
            "INSERT INTO users (user_name, password, first_name, last_name)"
            "VALUES ('%s', '%s', '%s', '%s');"
            "SELECT uid FROM users WHERE user_name = '%s';",
            req_parsed->username,
            req_parsed->password,
            req_parsed->first_name,
            req_parsed->last_name,
            req_parsed->username);

    if (sqlite3_exec(db, sql, reqistration_callback, &new_uid, &err)) {
        req_parsed->res_code =
            strcmp(DB_UNIQUE_USER_ERR, err) ? INTERNAL_SRVR_ERR : BAD_REQ;

        fprintf(stderr, "Creating new user err: %s\n", sqlite3_errmsg(db));
        free(err);
    }
    else if (new_uid > 0) {
        printf("\tnew_uid OK: %d\n", new_uid);
        fflush(stdout);

        memset(sql, 0, 256);
        sprintf(sql,
                "INSERT INTO chats (chat_name, from_uid, to_uid)"
                "VALUES ('%s <-> %s', '%d', '%d');",
                req_parsed->username,
                req_parsed->username,
                new_uid, new_uid);

        if (sqlite3_exec(db, sql, NULL, NULL, &err)) {
            fprintf(stderr, "Creating self chat err: %s\n", sqlite3_errmsg(db));
            free(err);
            req_parsed->res_code = INTERNAL_SRVR_ERR;
        }
        else {
            req_parsed->res_code = CREATED;
        }
    }
    else {
        printf("\tnew_uid: %d\n", new_uid);
        fflush(stdout);
        req_parsed->res_code = INTERNAL_SRVR_ERR;
    }
}
