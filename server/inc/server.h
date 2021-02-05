#pragma once

// ===== defines =====
#define BUF_SZ 2048
#define NAME_LEN 32

#include "err_msg_defines.h"

// ===== includes =====
#include "srvr_includes.h"

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

void mx_add_cl_node(t_cl_data *client);
void mx_authorization(t_cl_data *client, t_list *cur_client, int *leave_fl);

void mx_db_init(void);

void mx_init_server_and_client(int argc, char **argv, t_srvr_data *srvr, t_cl_data *client);
void *mx_handle_client(void *arg);
void mx_send_msg(char *msg, t_list *cur_clietn, t_cl_data *client);
void mx_server_err(int err, int serv_fd, int client_fd);
void mx_set_addr(struct sockaddr_in *srvr_addr, struct sockaddr_in *cl_addr, char *port_str);
void mx_remove_cl_node(unsigned cur_uid, t_cl_data *client);
void mx_usg_err(char *name);




