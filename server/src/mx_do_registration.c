#include "server.h"

void mx_do_registration(sqlite3 *db, t_auth_req *req_parsed) {
    char sql[256] = "";
    char *err = NULL;

    memset(sql, 0, 256);
    sprintf(sql,
            "INSERT INTO users (user_name, password, first_name, last_name)"\
            "VALUES ('%s', '%s', '%s', '%s');",
            req_parsed->username,
            req_parsed->password,
            req_parsed->first_name,
            req_parsed->last_name);

    if (sqlite3_exec(db, sql, NULL, NULL, &err)) {
        req_parsed->res_code = strcmp(DB_UNIQUE_USER_ERR, err) ? INTERNAL_SRVR_ERR : BAD_REQ;

        fprintf(stderr, "Creating new user err: %s\n", sqlite3_errmsg(db));
        free(err);
    }
    else {
        req_parsed->res_code = CREATED;
    }
}
