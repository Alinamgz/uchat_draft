#pragma once

// ===== defines =====
#define BUF_SZ 1024
#define NAME_LEN 32

#include "err_msg_defines.h"

// ===== includes =====
#include "srvr_includes.h"

// ===== structs =====

typedef enum e_req_type {
	ERR,
	LOGIN,
	REGISTRATION,
	USER_SEARCH,
	NEW_CHAT,
	NEW_MSG,
	TOTAL
}			 t_req_type;

typedef struct s_auth_req {
	char *username;
	char *password;
	char *first_name;
	char *last_name;
	int req_type;
	int res_code;
	int uid;
}			   t_auth_req;

typedef struct s_chat_req_res {
	char *chat_id;
	char *name_or_msg;
	char *from_uid;
	char *to_uid;
}			   t_chat_req_res;

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
	t_auth_req *auth_req_res;
	t_chat_req_res **found_users;
	t_chat_req_res **chat_req_res;
	int res_code;
	int cur_row;
	int rows_cnt;
	int *receivers;
	struct s_list *next;
}			   t_list;

typedef struct s_cl_data {
	struct sockaddr_in addr;
	socklen_t addr_len;
	t_list *cl_list;
	int sock_fd;
	unsigned uid;
	pthread_mutex_t *mut;
	pthread_mutex_t req_mut;

	sqlite3 *db;
	t_req_type req_type;
	t_auth_req *auth_req;
}			   t_cl_data;


// ===== funcs =====
bool mx_switch_nonblock(int sock_fd, bool turn_on);

int mx_create_listener(int argc, char *app_name);

void mx_add_cl_node(t_cl_data *client);
void mx_authorization(sqlite3 *db, t_list *cur_client, int *leave_fl);

void mx_db_init(t_cl_data *client);

void mx_init_server_and_client(int argc, char **argv, t_srvr_data *srvr, t_cl_data *client);
void *mx_handle_client(void *arg);
void mx_send_msg(char *msg, t_list *cur_clietn, t_cl_data *client);
void mx_server_err(int err, int serv_fd, int client_fd);
void mx_set_addr(struct sockaddr_in *srvr_addr, struct sockaddr_in *cl_addr, char *port_str);
void mx_remove_cl_node(unsigned cur_uid, t_cl_data *client);
void mx_usg_err(char *name);

// ----------------------------------
// draft for parsing and selecting search or msg req
void mx_parse_and_proceed_req(char *buf, t_list *cur_client, t_cl_data *client);
void mx_send_response(char *msg, t_list *cur_client, t_cl_data *client);

// login and registration
void mx_parse_auth_req(t_auth_req **data, const char *req_str);
void mx_do_login(sqlite3 *db, t_list *cur_client);
void mx_do_registration(sqlite3 *db, t_list *cur_client);
char *mx_create_auth_res(t_list *cur_client);

// select chats test
void mx_count_chat_rows(sqlite3 *db, t_list *cur_client, int my_uid);
void mx_select_chats(sqlite3 *db, t_list *cur_client, int my_uid);
void mx_create_new_chat(sqlite3 *db, t_list *cur_client, int peer_uid, char *peer_name);
void mx_proceed_newchat_req(char *buf, t_list *cur_client, t_cl_data *client);
char *mx_create_chats_response(t_list *cur_client);
void mx_chat_req_memfree(t_list *cur_client);

// search for users
void mx_proceed_search_req(char *buf, t_list *cur_client, t_cl_data *client);
void mx_do_search_user(sqlite3 *db, t_list *cur_client, char *search_str);
char *mx_create_search_response(t_list *cur_client);

// new msg
void mx_proceed_newmsg_req(char *buf, t_list *cur_client, t_cl_data *client);
int mx_do_add_new_msg(sqlite3 *db, t_list *cur_client, t_chat_req_res *new_msg, bool is_sent);
char *mx_create_newmsg_response(sqlite3 *db,
								t_list *cur_client,
								t_chat_req_res *new_msg,
								int newmsg_id);

void mx_set_receivers(t_list *cur_client, int my_uid, int peer_uid);
