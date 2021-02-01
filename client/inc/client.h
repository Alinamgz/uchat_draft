#pragma once

// ===== defines =====
#define MAXSLEEP 16
#define SRVR_PORT 9001
#define BUF_SZ 128
#define NAME_LEN 32

#define USG_ERR_START "Usage: "
#define USG_ERR_END " [server IP address] [port number]\n"
#define NAME_ERR "\nERR: wrong name length! min 2, max 31 chars.\n NB: To exit enter 'q')\n >> "
#define SEND_TH_ERR "Err: failed to create sending msg handler thread\n"
#define RECV_TH_ERR "Err: failed to create receiving msg handler thread\n"

// ===== includes =====
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <pthread.h>
#include <signal.h>


// ===== structs =====
typedef struct s_client {
	pthread_mutex_t mut;
	char *name;
	int th_ret;
	int sock_fd;
}			   t_client;

// ====== funcs ======

int mx_connect_retry(const struct sockaddr *adr);

void mx_authorization(t_client *client);
void mx_client_err(int err, int fd);
void mx_get_name(char **str);
// TODO:
void mx_init_client(t_client *client, char *port);

void *mx_recv_msg_handler(void *arg);
void *mx_send_msg_handler(void *arg);
void mx_set_addr(struct sockaddr_in *srvr_addr, char *port_str);
void mx_strtrim(char **str);
void mx_usg_err(char *name);
