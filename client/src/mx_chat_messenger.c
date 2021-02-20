#include "client.h"

pthread_t watcher;
int sock = 0;
char msg_buf[MESSAGE_BUF_SIZE];

char *mx_message(t_client *client, t_scene type) {
    char *msg_req = NULL;

    cJSON *str_line = cJSON_CreateObject();
    cJSON *msg = cJSON_CreateString(client->msg_from_client.msg_str);
    cJSON *username = cJSON_CreateString(client->self->username);
    printf("%s\n", client->msg_time);
    cJSON *time = cJSON_CreateString(client->msg_time);

    // datetime

    cJSON_AddNumberToObject(str_line, "req_type", NEW_MSG);
    // TODO: change to chat id
    cJSON_AddNumberToObject(str_line, "chat_id", client->self->uid);
    cJSON_AddNumberToObject(str_line, "from_id", client->self->uid);
    cJSON_AddItemToObject(str_line, "message", msg);
    cJSON_AddItemToObject(str_line, "username", username);
    cJSON_AddItemToObject(str_line, "msg_time", time);

    msg_req = cJSON_PrintUnformatted(str_line);
    cJSON_Delete(str_line);

    return msg_req;
}

gboolean mx_widget_is_visible(gchar *widget_name, GtkBuilder *builder) {
    GObject *widget = gtk_builder_get_object(builder, widget_name);

    return gtk_widget_is_visible(GTK_WIDGET(widget));
}

void mx_send_message_handle_enter(GtkTextView *textview, GdkEvent *event, t_client *client) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);

    printf("buffer");

    if (event->key.keyval == MX_KEY_SHIFT)
        client->ui->shift_hold = TRUE;
    g_usleep(0);
    if (event->key.keyval == MX_KEY_ENTER) {
        if (client->ui->shift_hold)
            gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
        else if (!mx_widget_is_visible("box_editing_msg", client->ui->builder)){
            mx_req_send_message(NULL, client);
            mx_clear_buffer_text("buffer_message", client->ui->builder);
        }
        else
            mx_req_edit_message(NULL, client);
    }
}


void mx_switch_room_header(t_client *client, GtkBuilder *builder, gint page_index) {
    client->ui->stack = gtk_builder_get_object(builder, "stack_room_header");
    GObject *page = NULL;

    if (page_index == MX_ROOM_CTRL)
        page = gtk_builder_get_object(builder, "box_room_ctrl");
    else if (page_index == MX_MSG_CTRL)
        page = gtk_builder_get_object(builder, "box_msg_ctrl");
    gtk_stack_set_visible_child(GTK_STACK(client->ui->stack), GTK_WIDGET(page));
}

static void init_chat_window(GtkBuilder *builder, t_client *client) {
    if (!builder) return;
    //window
    client->ui->uchat_client = GTK_WIDGET(gtk_builder_get_object(builder, "uchat_client"));
    if (!client->ui->uchat_client) {
        g_critical("Can`t get uchat_client");
    }

    client->ui->textview = gtk_builder_get_object(builder, "msg_entry");
    client->ui->btn_send = gtk_builder_get_object(builder, "btn_send_msg");
    // client->ui->btn_edit = gtk_builder_get_object(builder, "btn_edit_msg_apply");
    // client->ui->box_entry = gtk_builder_get_object(builder, "box_entry_field");
    // client->ui->box_header = gtk_builder_get_object(builder, "box_room_header");
    client->ui->box_editing = gtk_builder_get_object(builder, "box_editing_msg");
    // if (client->ui->visibility) {
    //     mx_switch_room_header(client, builder, MX_ROOM_CTRL);
    //     gtk_widget_show_all(GTK_WIDGET(client->ui->box_entry));
    //     gtk_widget_show_all(GTK_WIDGET(client->ui->box_header));
    // }
    // else {
    //     gtk_widget_hide(GTK_WIDGET(client->ui->box_entry));
    //     gtk_widget_hide(GTK_WIDGET(client->ui->box_header));
    //     gtk_widget_hide(GTK_WIDGET(client->ui->box_editing));

    // }
}

static void init_siganl(t_client *client) {
    printf("start handler entry\n");
    g_signal_connect(client->ui->textview, "key-press-event", G_CALLBACK(mx_send_message_handle_enter), client);
    printf("end handler entry\n");
    g_signal_connect(client->ui->textview, "key-release-event", G_CALLBACK(mx_send_message_handle_shift), client);
    printf("start button 1\n");
    g_signal_connect(client->ui->btn_send, "clicked", G_CALLBACK(mx_req_send_message), client);
    printf("end button1\n");
    g_signal_connect(client->ui->btn_send, "clicked", G_CALLBACK(mx_req_edit_message), client);
}

void mx_chat_messenger(t_client *client) {
    // GtkBuilder *builder = gtk_builder_new_from_file("client/templates/chat.glade");
    GtkBuilder *builder = gtk_builder_new_from_file("client/templates/chat_n_search.glade");
    if (!builder) {
        g_critical("Builder getting error!");
    }

    gtk_builder_connect_signals(builder, NULL);

    // mx_activate_content_test(client);
    init_chat_window(builder, client);
    init_siganl(client);

    g_object_unref(builder);
}
