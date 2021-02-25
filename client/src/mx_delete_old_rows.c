#include "client.h"

void mx_delete_old_rows(t_client *client, GtkListBox *cur_box) {
    pthread_mutex_lock(&client->render_search_mut);

    GtkListBoxRow *cur_row = NULL;

    while((cur_row = gtk_list_box_get_row_at_index(cur_box, 0))) {
        gtk_widget_destroy(GTK_WIDGET(cur_row));
    }

    pthread_mutex_unlock(&client->render_search_mut);
}
