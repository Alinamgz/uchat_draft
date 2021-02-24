#include "client.h"

// volatile sig_atomic_t flag_ex = 1;

static inline void leaks_ch(int sig) {
	printf("\n--------------- Signal ------------\n");
	system("leaks -q uchat");

	exit(sig);
}

int main(int argc, char **argv) {
	if (argc!= 3)
		mx_usg_err(argv[0]);

	t_client *client = (t_client*)malloc(sizeof(t_client));

	client->ui = (t_ui*)malloc(sizeof(t_ui));
	client->argv = argv;
	client->prev_scene = -1;
	client->th_ret = 1;
	client->auth_req = NULL;
	// client->search_req = NULL;
	client->req = NULL;
	client->chats = NULL;

	pthread_mutex_init(&client->mut, NULL);
	pthread_mutex_init(&client->connection_mut, NULL);
	pthread_cond_init(&client->req_cond, NULL);
	pthread_mutex_init(&client->req_sig_mut, NULL);
	pthread_mutex_init(&client->resp_mut, NULL);
	pthread_mutex_init(&client->render_search_mut, NULL);

	signal(SIGINT, leaks_ch);

	mx_init_gtk_app(client);

	printf("\n---------------------------------------------\n");
	system("leaks -q uchat");
	return 0;
}
