#include "client.h"

static GtkWidget *create_new_msg_row(t_client *client, t_msgs *cur_msg);

void mx_show_new_msg(t_client *client, t_msgs *cur_msg) {
    if (client->selected_chat && cur_msg->chat_id == client->selected_chat->chat_id){
        GtkWidget *row = create_new_msg_row(client, cur_msg);
printf("nu sho_2\n");

        if (row && cur_msg) printf("row\n");
    }
    else {
        printf("new msg in chat %d\n", cur_msg->chat_id);
    }
}

static GtkWidget *create_new_msg_row(t_client *client, t_msgs *cur_msg) {
    char sender[30] = "";
    memset(sender, 0, 30);

    GtkWidget *row = gtk_list_box_row_new();

    GtkWidget *msg_outer_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    GtkWidget *msg_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    GtkWidget *empty_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);


    if (client->self->uid == cur_msg->from_uid) {
        sprintf(sender, "%s:", client->self->username);
        gtk_box_pack_start(GTK_BOX(msg_outer_box), empty_box, TRUE, TRUE, 5);
        gtk_box_pack_end(GTK_BOX(msg_outer_box), msg_box, TRUE, TRUE, 5);
    }
    else {
        sprintf(sender, "%s:", client->selected_chat->peer_name);
        gtk_box_pack_start(GTK_BOX(msg_outer_box), msg_box, TRUE, TRUE, 5);
        gtk_box_pack_end(GTK_BOX(msg_outer_box), empty_box, TRUE, TRUE, 5);
    }

    GtkWidget *sender_name = gtk_labelnew(sender);
    printf("|| msg check: %s\n", cur_msg->msg);
    GtkWidget *msg = gtk_label_new(cur_msg->msg);
    GtkWidget *date = gtk_label_new(cur_msg->timestamp);

    gtk_label_set_line_wrap(GTK_LABEL(msg), TRUE);
    gtk_label_set_xalign(GTK_LABEL(date), 1.0);

    gtk_box_pack_start(GTK_BOX(msg_box), sender_name, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(msg_box), msg, TRUE, TRUE, 5);
    gtk_box_pack_end(GTK_BOX(msg_box), date, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(row), msg_outer_box);
    gtk_list_box_insert((GtkListBox *)client->ui->msg_list, row, -1);

    printf("sender: %s\n", sender);

    return row;
}
