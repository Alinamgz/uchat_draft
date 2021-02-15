#include "server.h"

// void mx_authorization(t_cl_data *client, t_list *cur_client, int *leave_fl) {
//     char buf[BUF_SZ] = "";

//     if (recv(cur_client->sock_fd, buf, BUF_SZ, 0) <= 0
// 		|| strlen(buf) < 2
// 		|| strlen(buf) >= NAME_LEN - 1) {
// 			write(STDERR_FILENO, NAME_ERR, sizeof(NAME_ERR) - 1);
// 			*leave_fl = 1;
// 	}
// 	else {
// 		strcpy(cur_client->name, buf);
// 		strcat(buf, " has joined!");
// 		mx_send_msg(buf, cur_client, client);
// 	}
// }

static void do_auth(sqlite3 *db, t_list *cur_client);
static void send_response(int fd, t_auth_req *req_res);
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
            send_response(cur_client->sock_fd, cur_client->auth_req_res);
            
            auth_req_memfree(cur_client->auth_req_res);
        }
    }

    free(cur_client->auth_req_res);
    cur_client->auth_req_res = NULL;
// TODO: delete it:
    printf("\t-- auth kinda done --\n");
    fflush(stdout);
}

static void do_auth(sqlite3 *db, t_list *cur_client) {
    int req_type = cur_client->auth_req_res->req_type;

    switch (req_type) {
        case LOGIN:
            mx_do_login(db, cur_client);
            break;
        case REGISTRATION:
            mx_do_registration(db, cur_client->auth_req_res);
            break;
        default:
            break;
    }
}

static void send_response(int fd, t_auth_req *req_res) {
    char *resp = mx_create_auth_res(req_res);

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
