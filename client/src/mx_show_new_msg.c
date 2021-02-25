#include "client.h"

static GtkWidget *create_new_msg_row(t_client *client, t_msgs *cur_msg);

void mx_show_new_msg(t_client *client, t_msgs *cur_msg) {
    if (client->selected_chat && client->new_msg->chat_id == client->selected_chat->chat_id){
        GtkWidget *row = create_new_msg_row(client, cur_msg);

        if (row && cur_msg) printf("row\n");
    }
    else {
        printf("new msg in chat %d\n", client->new_msg->chat_id);
    }
}

static GtkWidget *create_new_msg_row(t_client *client, t_msgs *cur_msg) {
    GtkWidget *row = gtk_list_box_row_new();
if (cur_msg) printf("\n");
    // GtkWidet *msg_box = gtk_box_new
    return row;
}
