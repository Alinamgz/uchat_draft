#include "client.h"

static GtkWidget *create_chat_row(char *chat_name_str, bool is_unread);
static void chat_row_selected_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data);

void mx_show_chats(t_client *client) {
    GtkWidget *row = NULL;
    // TODO: delete it
    bool is_unread = 0;

    for (int i = 0; client->chats && client->chats[i]; i++) {
        // TODO: delete it
        is_unread = i % 2 ? 1 : 0;

        row = create_chat_row(client->chats[i]->chat_name, is_unread);
        g_signal_connect(G_OBJECT(client->ui->chats_list),
                              "row-selected",
                              G_CALLBACK(chat_row_selected_handler),
                              client);

        gtk_list_box_insert((GtkListBox *)client->ui->chats_list, row, -1);
        printf("inserted_row %d\n", i);
    }
}

static GtkWidget *create_chat_row(char *chat_name_str, bool is_unread) {
    GtkWidget *row = gtk_list_box_row_new();

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
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

static void chat_row_selected_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {
    t_client *client = (t_client*)user_data;
    gint row_ind = -1;
    char *cur_name = NULL;

    if (box && row) {
        if (gtk_list_box_row_is_selected(row)) {
            row_ind = gtk_list_box_row_get_index(row);
            if (row_ind > -1) {
                if(client->selected_chat) {
                    free(client->selected_chat);
                }
                    client->selected_chat = (t_chats*)malloc(sizeof(t_chats));

                    client->selected_chat->chat_name = client->chats[row_ind]->chat_name;
                    client->selected_chat->chat_id = client->chats[row_ind]->chat_id;
                    client->selected_chat->from_uid = client->chats[row_ind]->from_uid;
                    client->selected_chat->to_uid = client->chats[row_ind]->to_uid;


                cur_name = client->chats[row_ind]->chat_name;
                printf("\t--------- %s --------\n", cur_name);
                printf("chat_id: %d\t from: %d\t to: %d\n",
                        client->selected_chat->chat_id,
                        client->selected_chat->from_uid,
                        client->selected_chat->to_uid);

                gtk_label_set_text(GTK_LABEL(client->ui->selected_chat_name), cur_name);

            }
            else {
                printf("\nERR: can't get row index\n");
            }
        }
        else {
                printf("\nERR: can't check if row is selected\n");
        }
    }
}
