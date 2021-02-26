#include "client.h"

static void create_search_req_str(t_client *client, const gchar *search_str);

void create_search_req(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client*)data;
    char *err = NULL;

    const gchar *search_str = mx_get_n_check_entry(&err, 0, widget, client->ui->search_status);
    if (!err[0] && search_str[0]) {
        create_search_req_str(client, search_str);
    }
}

void mx_stop_search_room(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(widget), "");
}

static void create_search_req_str(t_client *client, const gchar *search_str) {
    cJSON *req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "type", SEARCH);
    cJSON_AddStringToObject(req, "search_str", search_str);

    pthread_mutex_lock(&client->req_sig_mut);

    // client->search_req = cJSON_PrintUnformatted(req);
    client->req = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    pthread_mutex_unlock(&client->req_sig_mut);
    pthread_cond_signal(&client->req_cond);
}
