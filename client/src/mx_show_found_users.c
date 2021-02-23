#include "client.h"

static GtkWidget *create_chat_row(t_self *cur_rslt);
static void  delete_old_rslts(t_client *client);
// static void user_row_selected_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data);

void mx_show_found_users(t_client *client) {
    GtkWidget *row = NULL;

     delete_old_rslts(client);

    for (int i = 0; client->found_users && client->found_users[i]; i++) {
        row = create_chat_row(client->found_users[i]);
        // g_signal_connect(G_OBJECT(client->ui->users_list),
        //                       "row-selected",
        //                       G_CALLBACK(user_row_selected_handler),
        //                       client);

        gtk_list_box_insert((GtkListBox *)client->ui->users_list, row, -1);
    }
    gtk_widget_show_all(GTK_WIDGET(client->ui->users_list));
}

static void  delete_old_rslts(t_client *client) {
    GtkListBoxRow *cur_row = NULL;
    GtkListBox *cur_box = (GtkListBox*)client->ui->users_list;

    for (int i = 0; (cur_row = gtk_list_box_get_row_at_index(cur_box, i)); i++)
        gtk_widget_destroy(GTK_WIDGET(cur_row));

}

static GtkWidget *create_chat_row(t_self *cur_rslt) {
    char full_name[50] = "";
    GtkWidget *row = gtk_list_box_row_new();

    memset(full_name, 0, 50);
    if (cur_rslt->first_name[0] && cur_rslt->last_name[0])
        sprintf(full_name, "%s %s", cur_rslt->first_name, cur_rslt->last_name);
    else if (cur_rslt->first_name[0])
        sprintf(full_name, "%s", cur_rslt->first_name);
    else if (cur_rslt->last_name[0])
        sprintf(full_name, "%s", cur_rslt->last_name);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *username_label = full_name[0] ? gtk_label_new(full_name) : gtk_label_new(cur_rslt->username);
    // GtkWidget *fullname_label = gtk_label_new(full_name);

    gtk_widget_set_size_request(box, 280, 20);

    gtk_label_set_xalign(GTK_LABEL(username_label), 0.1);
    // gtk_label_set_xalign(GTK_LABEL(fullname_label), 0.2);

    gtk_label_set_line_wrap(GTK_LABEL(username_label), TRUE);

    gtk_box_pack_start(GTK_BOX(box), username_label, TRUE, TRUE, 5);
    // gtk_box_pack_start(GTK_BOX(box), fullname_label, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(row), box);

    return row;
}

// static void user_row_selected_handler(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {
//     t_client *client = (t_client*)user_data;
//     gint row_ind = -1;
//     char *cur_name = NULL;

//     if (box && row) {
//         if (gtk_list_box_row_is_selected(row)) {
//             row_ind = gtk_list_box_row_get_index(row);
//             if (row_ind > -1) {
//                 if(client->cur_chat) {
//                     free(client->cur_chat);
//                 }
//                     client->cur_chat = (t_chats*)malloc(sizeof(t_chats));

//                     client->cur_chat->chat_name = client->chats[row_ind]->chat_name;
//                     client->cur_chat->chat_id = client->chats[row_ind]->chat_id;
//                     client->cur_chat->from_uid = client->chats[row_ind]->from_uid;
//                     client->cur_chat->to_uid = client->chats[row_ind]->to_uid;


//                 cur_name = client->chats[row_ind]->chat_name;
//                 printf("\t--------- %s --------\n", cur_name);
//                 printf("chat_id: %d\t from: %d\t to: %d\n",
//                         client->cur_chat->chat_id,
//                         client->cur_chat->from_uid,
//                         client->cur_chat->to_uid);

//                 gtk_label_set_text(GTK_LABEL(client->ui->cur_chat_name), cur_name);

//             }
//             else {
//                 printf("\nERR: can't get row index\n");
//             }
//         }
//         else {
//                 printf("\nERR: can't check if row is selected\n");
//         }
//     }
// }
