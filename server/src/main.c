#include "server.h"

	pthread_mutex_t client_mutex = PTHEAD_MUTEX_INITIALIZER;
	t_client *client_arr[SOMAXCONN];


static void str_overwrite_stdout(void) {
	printf("\r> ");
	fflush(stdout);
}

static void str_trim_lf(char *str, int len) {
	for(int i = 0; i < len; i++) {
		if(str[i] == '\n') {
			str[i] = '\0';
			break;
		}
	}
}

static void queue_add (t_client **cl_arr, t_client *cl, pthread_mutex_t *cl_mut) {
	pthread_mutex_lock(cl_mut);

	for (int i = 0; i < SOMAXCONN; i++) {
		if (!cl_arr[i]) {
			cl_arr[i] = cl;
			break;
		}
	}

	pthread_mutex_unlock(cl_mut);
}

static void queue_remove (t_client **cl_arr, int uid, pthread_mutex_t *cl_mut) {
	pthread_mutex_lock(cl_mut);

	for (int i = 0; i < SOMAXCONN; i++) {
		if (cl_arr[i] && cl_arr[i]->uid == uid) {
			cl_arr[i] = NULL;
			break;
		}
	}
	
	pthread_mutex_unlock(cl_mut);
}

static void send_msg(char *msg, int from_uid, t_client **cl_arr, pthread_mutex_t *cl_mut) {
	pthread_mutex_lock(cl_mut);

	for (int i = 0; i < SOMAXCONN; i++) {
		if (cl_arr[i] && cl_arr[i]->uid != from_uid) {
			if (send(cl_arr[i]->sock_fd, msg, sizeof(msg), 0) < 0) {
				perror("Seding msg err");
				break;
			}
		}
	}
	pthread_mutex_unlock(cl_mut);
}

static void handle_client (void *arg) {
	char buf[BUF_SZ];
	char name[NAME_LEN];
	int leave_flg = 0;
	int recv_rslt = 0;
	t_client *client = arg;

// Name
	if (recv(client->sock_fd, name, NAME_LEN, 0) <= 0
		|| strlen(name) < 2
		|| strlen(name) >= NAME_LEN - 1
		) {
			write(2, "Enter the name correctly\n", strlen("Enter the name correctly\n"));
			leave_flg = 1;
	}
	else {
		strcpy(client->name, name);
		// TODO: google this func
		sprintf(buf, "%s has joined\n", client->name);
		printf("%s", buf);
		send_msg(buf, client->uid, client_arr,  &client_mutex);
	}
	memset(buf, 0, sizeof(buf));

	while(!leave_flg) {
		recv_rslt = recv(client->sock_fd, buf, BUF_SZ, 0);
		if (recv_rslt > 0) {
			if (strlen(buf) > 0) {
				send_msg(buf, client->uid, client_arr, &client_mutex);
				str_trim_lf(buf, strlen(buf));
				printf("%s -> %s\n", buf, client->name);
			}
		}
		else if (recv_rslt == 0 || !strcmp(buf, "exit")) {
			sprintf(buf, "%s has left\n", client->name);
			printf("%s\n", buf);
			send_msg(buf, client->uid, client_arr, &client_mutex);
			leave_flg = 1;
		}
		else {
			perror("Clinet handle recv");
			leave_flg = 1;
		}
	memset(buf, 0, sizeof(buf));
	}

	close(client->sock_fd);
	queue_remove(client_arr, client->uid, &client_mutex);
	free(client);
	pthread_detach(pthread_self());

	return;
}

int main(int argc, char **argv) {
	int port = (argc == 2) ? atoi(argv[1]) : 0;
	int option = 1;
	int listenfd = -1;
	int connfd = -1;
	int uid = 0;
	struct sockaddr_in srvr_addr;
	struct sockaddr_in client_addr;
	pthread_t tid;
	socklen_t client_len = 0;
	t_client *client = NULL;
	// t_client *client_arr[SOMAXCONN];

	if (!port || argc != 2) {
		mx_usg_err(argv[0]);
	}

// socket settigs
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&srvr_addr, 0, sizeof(srvr_addr));
	srvr_addr.sin_family = AF_INET;
	srvr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvr_addr.sin_port = htons(port);

// signals
	signal(SIGPIPE, SIG_IGN);

	if (setsockopt(listenfd,
				   SOL_SOCKET,
				   SO_REUSEADDR,
				   (char*)&option, sizeof(option)
				   ) < 0) {
		write(2, "signals\t", strlen("signals\t"));
		mx_server_err(errno, listenfd, connfd);
	}

// Binding
	if (bind(listenfd, (struct sockaddr*)&srvr_addr, sizeof(srvr_addr)) < 0) {
		write(2, "binding\t", strlen("binding\t"));
		mx_server_err(errno, listenfd, connfd);
	}

// Listen
	if (listen(listenfd, SOMAXCONN) < 0) {
		write(2, "listening\t", strlen("listening\t"));
		mx_server_err(errno, listenfd, connfd);
	}

	printf("\t ==== WELCOME ====\n");

	while (1) {
		client_len = sizeof(client_addr);
		connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len);

		if (connfd < 0) {
			write(2, "accepting\t", strlen("accepting\t"));
			mx_server_err(errno, listenfd, connfd);
		}

//  Client settings
		client = (t_client*)malloc(sizeof(t_client));
		client->addr = client_addr;
		client->sock_fd = connfd;
		client->uid = uid++;

// Add client to queue
		queue_add(client_arr, client, &client_mutex);
		pthread_create(&tid, NULL, &handle_client, (void*)client);

// Reduce CPU usage
		sleep(1);
	}

	system("leaks -q uchat_server");
	return 0;
}
