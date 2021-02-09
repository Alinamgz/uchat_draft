#include "client.h"

void mx_connect_retry_gtk(GtkWidget *widget, gpointer data) {
	t_client *client = (t_client*)data;

	struct sockaddr_in srvr_addr;
	int numsec = 0;
	socklen_t addr_len = sizeof(srvr_addr);

	mx_set_addr(&srvr_addr, client->argv[1],  client->argv[2]);

	client->sock_fd = -1;
write(STDOUT_FILENO, "------ conn ----\n", strlen("------ conn ----\n"));

	for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
		if ((client->sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			g_critical("%s\n", strerror(errno));
			gtk_label_set_text(GTK_LABEL(client->ui->fail_reason_msg), strerror(errno));
		    client->scene = CONNECTION_ERR;
			return;
		}

		if (connect(client->sock_fd, (struct sockaddr *)&srvr_addr, addr_len) == 0) {
		    client->scene = LOGIN;
			return;
		}

		close(client->sock_fd);
printf("\t---- NO conn --- \n");

		perror("Connecting to server");
		// gtk_label_set_text(GTK_LABEL(client->ui->fail_reason_msg), strerror(errno));;
		// client->sock_fd = -1;
		// return;

		write(2, "\tretrying...\n", 13);
		if (numsec <= MAXSLEEP)
			sleep(numsec);
	}
		g_critical("%s\n", strerror(errno));
		gtk_label_set_text(GTK_LABEL(client->ui->fail_reason_msg), strerror(errno));;
		client->scene = CONNECTION_ERR;
		client->sock_fd = -1;
		return;
}
