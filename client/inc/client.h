#pragma once

// ===== defines =====
#define MAXSLEEP 8
#define SRVR_PORT 9001
#define BUF_SZ 1024
#define NAME_LEN 32

#define USG_ERR_START "Usage: "
#define USG_ERR_END " [server IP address] [port number]\n"
#define NAME_ERR "\nERR: wrong name length! min 2, max 31 chars.\n NB: To exit enter 'q')\n >> "
#define SEND_TH_ERR "Err: failed to create sending msg handler thread\n"
#define RECV_TH_ERR "Err: failed to create receiving msg handler thread\n"

// response codes
#define OK 200
#define OK_CHATS 2001
#define CREATED 201
#define BAD_REQ 400
#define UNAUTHORIZED 401
#define NOT_FOUND 404
#define INTERNAL_SRVR_ERR 500

/*
 * Keybord
 */
#define MX_KEY_ENTER (gint)65293
#define MX_KEY_SHIFT (gint)65505
#define MX_MAX_MESSAGE 800

#define MX_LOG_FILE "info.log"
#define MX_ROOM_CTRL 0
#define MX_MSG_CTRL 1


// ===== includes =====
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <glib/gprintf.h>
#include <glib.h>

#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <pthread.h>
#include <signal.h>

#include "cJSON.h"
#include <gtk/gtk.h>

//----------- CSS3 -----------//
#define CHAT_MSG_CSS "client/templates/style/mx_window_login.css"

// ===== structs =====

typedef struct s_dtp t_dtp;

typedef struct s_raw_inputs {
	const gchar *username;
    const gchar *password;

    const gchar *conf_password;
    const gchar *first_name;
    const gchar *last_name;
}			   t_raw_inputs;

typedef enum e_scene {
	CONNECTION_ERR,
	LOGIN,
	REGISTRATION,
	SEARCH,
	NEW_CHAT,
	NEW_MSG,
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
	GtkWidget *conf_pass_entry;
	GtkWidget *conf_pass_status;
	GtkWidget *firstname_entry;
	GtkWidget *firstname_status;
	GtkWidget *lastname_entry;
	GtkWidget *lastname_status;
	GtkWidget *register_btn;
	GtkWidget *show_login_btn;

	// chat_client
	GtkBuilder *builder;
	GtkWidget *uchat_client;
	GtkWidget *chats_list;
	GtkWidget *chats_label;
	GtkWidget *selected_chat_name;
	GtkWidget *users_list;
	// GtkWidget *username_row_label;
	// GtkWidget *fullname_row_label;

	// window
	// GtkWidget *textview;
	// GtkWidget *btn_edit;
	// GtkWidget *btn_send;
	GObject *textview;
	GObject *btn_edit;
	GObject *btn_send;
	//сегодня добавил
	GObject *box_entry;
	GObject *box_header;
	GObject *box_editing;
	GObject *stack;

	gboolean visibility;

	// search for new chat
	GtkWidget *search_bar;
	GtkWidget *search_status;

	gboolean shift_hold;
	gboolean msg_placeholder;
	GDataOutputStream *out;
}			   t_ui;

typedef struct s_self {
	char *username;
	char *first_name;
	char *last_name;
	int uid;

}			   t_self;

typedef struct s_msg_from_client {

	char *msg_str;
} t_msg_from_client;


struct s_dtp {
    char *str;
    cJSON *json;
    size_t len;
    gint type;
};

typedef struct s_chats {
	int chat_id;
	int from_uid;
	int to_uid;
	char *chat_name;
}			   t_chats;

typedef struct s_client {
	pthread_t connection_th;
	pthread_t auth_th;
	pthread_mutex_t mut;
	pthread_mutex_t connection_mut;
	pthread_mutex_t auth_mut;
	pthread_cond_t req_cond;
	pthread_mutex_t req_sig_mut;
	pthread_mutex_t render_search_mut;
	// pthread_cond_t req_cond;
	pthread_mutex_t resp_mut;
	char *name;
	int th_ret;
	int sock_fd;

	char *req;
	char *msg_time;
	char *msg_req;
	char *auth_req;
	char *search_req;

	char **argv;
	t_ui *ui;
	t_scene scene;
	t_scene prev_scene;
	t_self *self;
	t_self *selected_user;
	t_self **found_users;
	t_chats **chats;
	t_chats *selected_chat;

	char *msg;
	t_dtp *data;
	// cJSON *auth_req;
}			   t_client;

// ====== funcs ======

void *mx_connect_retry_gtk(gpointer data);
void mx_connection_retry_th(GtkWidget *widget, gpointer data);
void mx_init_gtk_app(t_client *client);

void mx_authorization(t_client *client);
void mx_client_err(int err, int fd);
void mx_get_name(char **str);

void mx_init_client_gtk(t_client *client);

void *mx_recv_resp_handler(void *arg);
void *mx_send_req_handler(void *arg);

void mx_set_addr(struct sockaddr_in *srvr_addr, char *addr_str, char *port_str);
void mx_strtrim(char **str);
void mx_usg_err(char *name);

void mx_init_gtk_app(t_client *client);

// init gtk windows
void mx_init_error_dialog(t_client *client);
void mx_init_login_window(t_client *client);
void mx_init_registration_window(t_client *client);
gboolean mx_check_scene(void *param);

// switch between login/registration forms
void mx_switch_form(GtkWidget *widget, gpointer data);

// getting form input and validation it
void mx_submit_login_handler(GtkWidget *widget, gpointer data);
void mx_submit_registration_handler(GtkWidget *widget, gpointer data);
bool mx_get_input_values(t_client *client, t_raw_inputs *inputs, t_scene type);
const gchar *mx_get_n_check_entry(char **err, bool is_req, gpointer entry, gpointer status);

// create req
void mx_create_auth_req(t_client *client, t_raw_inputs *inputs, t_scene type);

// processing auth response
void mx_parse_n_proceed_auth_response(t_client *client, char *res_buf);

void *mx_do_auth_th(void *arg);
void exit_gtk(GtkWidget *widget, void *param);

#define MESSAGE_BUF_SIZE (1 << 17)
void mx_chat_messenger(t_client *client);
void mx_init_chat_ths(t_client *client);

void mx_req_send_message(GtkButton *btn, t_client *client);
gchar *mx_get_buffer_text(gchar *buff_name, t_client *client);

void mx_trim_message(gchar **message);
void mx_clear_buffer_text(gchar *buff_name, GtkBuilder *builder);
void mx_req_edit_message(GtkButton *btn, t_client *client);
void mx_req_send_message(GtkButton *btn, t_client *client);
gssize mx_send(t_client *client, t_dtp *dtp);
void mx_send_message_handle_shift(GtkTextView *textview, GdkEvent *event, t_client *client);
t_dtp *mx_request_creation(char *req_body);
void mx_free_request(t_dtp **request) ;
void mx_free(void **ptr);
void mx_logger(gchar *file_name, GLogLevelFlags flags, gchar *error);
gchar *mx_get_buffer_text(gchar *buff_name, t_client *client);
gchar *mx_get_text_from_buffer(GtkTextBuffer *buffer);


// ------------- general resp handler -----------
void mx_parse_n_proceed_response(t_client *client, char *resp_str);

// ----- chats ------
void mx_create_chats_req(t_client *client, int peer_uid, char *peer_name);
void mx_proceed_chat_response(t_client *client, char *resp_str);
void mx_parse_chats_response(t_client *client, char *resp_str);
void mx_show_chats(t_client *client);
void mx_selected_chat_row_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data);

// ---- search ----
void create_search_req(GtkWidget *widget, gpointer data);
void mx_stop_search_room(GtkWidget *widget, gpointer data);
void mx_proceed_search_response(t_client *client, char *resp_str);
void mx_parse_search_response(t_client *client, char *resp_str);
void mx_show_found_users(t_client *client);
void mx_selected_user_row_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data);

void mx_delete_old_rows(t_client *client, GtkListBox *cur_box);
