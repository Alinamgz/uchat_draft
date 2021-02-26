#include "server.h"

static void do_auth(sqlite3 *db, t_list *cur_client);
static void send_response(int fd, t_list *cur_client);

static void auth_req_memfree(t_auth_req *auth_req_parsed);

void mx_authorization(sqlite3 *db, t_list *cur_client, int *leave_fl) {
    char auth_buf[BUF_SZ] = "";

    cur_client->auth_req_res = (t_auth_req *)malloc(sizeof(t_auth_req));
    cur_client->auth_req_res->res_code = 0;

    // TODO: watch this loop and ways to exit it if client closes auth indow
    while (*leave_fl == 0 && cur_client->auth_req_res->res_code != OK) {
        memset(auth_buf, 0, BUF_SZ);

        if (recv(cur_client->sock_fd, auth_buf, BUF_SZ, 0) <= 0) {
            *leave_fl = 1;
            write(STDERR_FILENO, AUTH_RECV_ERR, sizeof(AUTH_RECV_ERR));
        }
        else {
            mx_parse_auth_req(&cur_client->auth_req_res, auth_buf);
            do_auth(db, cur_client);
            send_response(cur_client->sock_fd, cur_client);

            mx_chat_req_memfree(cur_client);
            auth_req_memfree(cur_client->auth_req_res);
        }
    }

    free(cur_client->auth_req_res);
    cur_client->auth_req_res = NULL;
// TODO: delete it:
    printf("\t-- auth kinda done --\n");
    fflush(stdout);
    system("leaks -q uchat_server");
    printf("-----------------------\n");
    fflush(stdout);
}

static void do_auth(sqlite3 *db, t_list *cur_client) {
    int req_type = cur_client->auth_req_res->req_type;

    switch (req_type) {
        case LOGIN:
            mx_do_login(db, cur_client);
            if (cur_client->auth_req_res->res_code == OK) {
                mx_count_chat_rows(db, cur_client, cur_client->uid);
                mx_select_chats(db, cur_client, cur_client->uid);
            }
            break;
        case REGISTRATION:
            mx_do_registration(db, cur_client);
            mx_create_new_chat(db, cur_client,cur_client->uid, cur_client->auth_req_res->username);
            break;
        default:
            break;
    }
}

static void send_response(int fd, t_list *cur_client) {
    char *resp = mx_create_auth_res(cur_client);

    if (send(fd, resp, strlen(resp), 0) < 0)
        write(STDERR_FILENO, AUTH_SEND_ERR, sizeof(AUTH_SEND_ERR));

    free(resp);
}

static void auth_req_memfree(t_auth_req *auth_req_parsed) {
    if (auth_req_parsed) {
        if (auth_req_parsed->username) {
            free(auth_req_parsed->username);
            auth_req_parsed->username = NULL;
        }
        if (auth_req_parsed->password) {
            free(auth_req_parsed->password);
            auth_req_parsed->password = NULL;
        }
        if (auth_req_parsed->first_name) {
            free(auth_req_parsed->first_name);
            auth_req_parsed->first_name = NULL;
        }
        if (auth_req_parsed->last_name) {
            free(auth_req_parsed->last_name);
            auth_req_parsed->last_name = NULL;
        }
    }
}
