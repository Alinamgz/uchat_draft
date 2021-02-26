#include "client.h"

static GtkWidget *create_chat_row(char *chat_name_str, bool is_unread);

void mx_show_chats(t_client *client) {
    GtkWidget *row = NULL;
    // TODO: delete it
    bool is_unread = 0;

    for (int i = 0; client->chats && client->chats[i]; i++) {
        // TODO: delete it
        is_unread = i % 2 ? 1 : 0;

        row = create_chat_row(client->chats[i]->chat_name, is_unread);

        gtk_list_box_insert((GtkListBox *)client->ui->chats_list, row, -1);
        printf("inserted_row %d\n", i);
    }
    gtk_widget_show_all(GTK_WIDGET(client->ui->chats_list));
}

static GtkWidget *create_chat_row(char *chat_name_str, bool is_unread) {
    GtkWidget *row = gtk_list_box_row_new();

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    GtkWidget *chat_name = gtk_label_new(chat_name_str);
    GtkWidget *new_msg = is_unread ? gtk_label_new("*new!") : gtk_label_new(NULL);

    gtk_widget_set_size_request(box, 280, 30);

    gtk_label_set_xalign(GTK_LABEL(chat_name), 0.0);
    gtk_label_set_xalign(GTK_LABEL(new_msg), 1.0);

    gtk_label_set_line_wrap(GTK_LABEL(chat_name), TRUE);

    gtk_box_pack_start(GTK_BOX(box), chat_name, TRUE, TRUE, 15);
    gtk_box_pack_start(GTK_BOX(box), new_msg, TRUE, TRUE, 10);

    gtk_container_add(GTK_CONTAINER(row), box);

    return row;
}
