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

void mx_authorization(t_cl_data *client, t_list *cur_client, int *leave_fl) {
    char auth_buf[BUF_SZ] = "";
    char *resp = NULL;

    t_auth_req *auth_req_parsed = (t_auth_req *)malloc(sizeof(t_auth_req));
    auth_req_parsed->res_code = 0;

    if (client) {
        // TODO: watch this loop and ways to exit it if client closes auth
        // window
        while (*leave_fl == 0) {
            memset(auth_buf, 0, BUF_SZ);

            if (recv(cur_client->sock_fd, auth_buf, BUF_SZ, 0) <= 0) {
                write(STDERR_FILENO, "/!\\Err: recv auth response failed\n",
                      strlen("/!\\Err: recv auth response failed\n"));
                // *leave_fl = 1;
                break;
            }
            else {
                printf("recvd: %s", auth_buf);
                fflush(stdout);

                mx_parse_auth_req(&auth_req_parsed, auth_buf);

                if (auth_req_parsed->req_type == LOGIN) {
                    mx_do_login(client, auth_req_parsed);
                }
                // else if (auth_req_parsed->req_type == REGISTRATION) {
                //     mx_do_registration();
                // }

                resp = mx_create_auth_res(auth_req_parsed->res_code);

                if (send(cur_client->sock_fd, resp, strlen(resp), 0) < 0)
                    write(STDERR_FILENO, "Err: send auth response failed\n",
                          strlen("/!\\Err: send auth response failed\n"));
                free(resp);

                if (auth_req_parsed->res_code == OK) {
                    auth_req_memfree(auth_req_parsed);
                    break;
                }
                auth_req_memfree(auth_req_parsed);
            }
        }

        free(auth_req_parsed);

        printf("\t-- auth kinda done --\n");
        fflush(stdout);
    }
}
