#pragma once

// ===== defines =====
#define BUF_SZ 2048
#define NAME_LEN 32

#define USG_ERR_START "Usage: "
#define USG_ERR_END " [port number]\n"
#define NAME_ERR "Enter the name correctly. Name len: min 2, max 31\n"
#define HANDLER_LOOP_ERR "recv err in while loop in client handling\n"

// ===== includes =====
#include <arpa/inet.h>

#include <errno.h>
#include <fcntl.h>

#include <netinet/in.h>

#include <pthread.h>

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

// ===== structs =====
typedef struct s_srvr_data {
	struct sockaddr_in addr;
	int sock_fd;
	unsigned uid;
	pthread_mutex_t mut;
}			   t_srvr_data;

typedef struct s_list {
	struct sockaddr_in addr;
	int sock_fd;
	unsigned uid;
	char name[NAME_LEN];
	struct s_list *next;
}			   t_list;

typedef struct s_cl_data {
	struct sockaddr_in addr;
	socklen_t addr_len;
	t_list *cl_list;
	int sock_fd;
	unsigned uid;
	pthread_mutex_t *mut;
}			   t_cl_data;


// ===== funcs =====
bool mx_switch_nonblock(int sock_fd, bool turn_on);

int mx_create_listener(int argc, char *app_name);
void mx_init_server_and_client(int argc, char **argv, t_srvr_data *srvr, t_cl_data *client);

void mx_set_addr(struct sockaddr_in *srvr_addr, struct sockaddr_in *cl_addr, char *port_str);

void mx_server_err(int err, int serv_fd, int client_fd);
void mx_usg_err(char *name);

void mx_add_cl_node(t_cl_data *client);

void mx_remove_cl_node(unsigned cur_uid, t_cl_data *client);

void *mx_handle_client(void *arg);
void mx_send_msg(char *msg, t_list *cur_clietn, t_cl_data *client);

