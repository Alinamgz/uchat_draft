#pragma once

// ===== defines =====
#define MAXSLEEP 8
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

#include "cJSON.h"
#include <gtk/gtk.h>

// ===== structs =====
typedef enum e_scene {
	CONNECTION_ERR,
	LOGIN,
	REGISTRATION,
	CHAT,
	TOTAL
}			 t_scene;

typedef struct s_ui {
	// err dialog
	GtkWidget *err_dialog;
	GtkWidget *retry_btn;
	GtkWidget *fail_reason_msg;

	// login window
	GtkWidget *login_window;
	GtkWidget *l_username_entry;
	GtkWidget *l_username_status;
	GtkWidget *l_pass_entry;
	GtkWidget *l_pass_status;
	GtkWidget *login_btn;
	GtkWidget *show_registration_btn;

	// registration
	GtkWidget *registration_window;
	GtkWidget *r_username_entry;
	GtkWidget *r_username_status;
	GtkWidget *r_pass_entry;
	GtkWidget *r_pass_status;
	GtkWidget *register_btn;
	GtkWidget *show_login_btn;
}			   t_ui;

typedef struct s_client {
	pthread_t connection_th;
	pthread_mutex_t mut;
	pthread_mutex_t connection_mut;
	char *name;
	int th_ret;
	int sock_fd;

	char **argv;
	t_ui *ui;
	t_scene scene;
	t_scene prev_scene;
}			   t_client;

// ====== funcs ======

void *mx_connect_retry_gtk(gpointer data);
void mx_connection_retry_th(GtkWidget *widget, gpointer data);
void mx_init_gtk_app(t_client *client);


void mx_authorization(t_client *client);
void mx_client_err(int err, int fd);
void mx_get_name(char **str);

void mx_init_client_gtk(t_client *client);


void *mx_recv_msg_handler(void *arg);
void *mx_send_msg_handler(void *arg);

void mx_set_addr(struct sockaddr_in *srvr_addr, char *addr_str, char *port_str);
void mx_strtrim(char **str);
void mx_usg_err(char *name);

void mx_init_gtk_app(t_client *client);

// init gtk windows
void mx_init_error_dialog(t_client *client);
void mx_init_login_window(t_client *client);
void mx_init_registration_window(t_client *client);
gboolean mx_check_scene(void *param);

void mx_switch_form(GtkWidget *widget, gpointer data);
void mx_get_input_values(GtkWidget *widget, gpointer data);
char *mx_validate_input(const gchar *username, bool is_req);

bool mx_isalpha_digital(char c);
