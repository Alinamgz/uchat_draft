#pragma once

// ===== defines =====
#define BUF_SZ 2048
#define NAME_LEN 32
#define USG_ERR_START "Usage: "
#define USG_ERR_END " [port number]\n"

// ===== includes =====
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

// ===== structs =====
typedef struct s_client {
	struct sockaddr_in addr;
	int sock_fd;
	int uid;
	char name[NAME_LEN];
}			   t_client;
// ===== funcs =====
void mx_server_err(int err, int serv_fd, int client_fd);

void mx_usg_err(char *name);

