#include "client.h"

static void create_search_req(t_client *client, const gchar *search_str);

void mx_do_search_req(GtkWidget *widget, gpointer data) {
    t_client *client = (t_client*)data;
    char *err = NULL;

    const gchar *search_str = mx_get_n_check_entry(&err, 0, widget, client->ui->search_status);
    if (!err[0] && search_str[0]) {
        // if (search_str[0])
            create_search_req(client, search_str);
    }
    // else if (!err[0]) {
    //     gtk_entry_set_text(GTK_ENTRY(widget), "");
    // }
}

void mx_stop_search_room(GtkWidget *widget, gpointer data) {
    gtk_entry_set_text(GTK_ENTRY(widget), "");
}


static void create_search_req(t_client *client, const gchar *search_str) {
    cJSON *req = cJSON_CreateObject();
    cJSON_AddNumberToObject(req, "type", SEARCH);
    cJSON_AddStringToObject(req, "search_str", search_str);

    pthread_mutex_lock(&client->msg_sig_mut);

    client->search_req = cJSON_PrintUnformatted(req);
    cJSON_Delete(req);

    pthread_mutex_unlock(&client->msg_sig_mut);
    pthread_cond_signal(&client->msg_cond);
}
