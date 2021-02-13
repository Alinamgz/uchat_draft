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

void mx_authorization(t_cl_data *client, t_list *cur_client, int *leave_fl) {
    char auth_buf[BUF_SZ] = "";
    char *resp = "Nu taki sho-to priletelo";

    if (client) {
        if (recv(cur_client->sock_fd, auth_buf, BUF_SZ, 0) <= 0) {
            write(STDERR_FILENO, "Err: recv auth response failed\n",
                  strlen("/!\\Err: recv auth response failed\n"));
				  *leave_fl = 1;
        }
		else {
            printf("recvd: %s", auth_buf);
			fflush(stdout);
            if (send(cur_client->sock_fd, resp, strlen(resp), 0) < 0)
                write(STDERR_FILENO, "Err: send auth response failed\n",
                      strlen("/!\\Err: send auth response failed\n"));
        }
    }
}
