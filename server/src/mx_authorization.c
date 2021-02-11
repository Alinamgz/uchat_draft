#include "server.h"

void mx_authorization(t_cl_data *client, t_list *cur_client, int *leave_fl) {
    char buf[BUF_SZ] = "";

    if (recv(cur_client->sock_fd, buf, BUF_SZ, 0) <= 0
		|| strlen(buf) < 2
		|| strlen(buf) >= NAME_LEN - 1) {
			write(STDERR_FILENO, NAME_ERR, sizeof(NAME_ERR) - 1);
			*leave_fl = 1;
	}
	else {
		strcpy(cur_client->name, buf);
		strcat(buf, " has joined!");
		mx_send_msg(buf, cur_client, client);
	}
}

// void mx_authorization(t_cl_data *client, t_list *cur_client, int *leave_fl)  {
//     if (client) {
//         mx_get_type();
//         mx_get_login();
//         mx_get_password();
//         mx_get_name();
//     }
// }
