#include "client.h"

<<<<<<< HEAD
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
=======
// pthread_t watcher;

// char *mx_message(t_client *client, t_scene type) {
//     char *msg_req = NULL;

//     cJSON *str_line = cJSON_CreateObject();
//     cJSON *msg = cJSON_CreateString(client->msg_from_client.msg_str);
//     cJSON *username = cJSON_CreateString(client->self->username);
//     printf("%s\n", client->msg_time);
//     cJSON *time = cJSON_CreateString(client->msg_time);

//     // datetime

//     cJSON_AddNumberToObject(str_line, "req_type", NEW_MSG);
//     // TODO: change to chat id
//     cJSON_AddNumberToObject(str_line, "chat_id", client->self->uid);
//     cJSON_AddNumberToObject(str_line, "from_id", client->self->uid);
//     cJSON_AddItemToObject(str_line, "message", msg);
//     cJSON_AddItemToObject(str_line, "username", username);
//     cJSON_AddItemToObject(str_line, "msg_time", time);

//     msg_req = cJSON_PrintUnformatted(str_line);
//     cJSON_Delete(str_line);

//     return msg_req;
// }

// void mx_get_buffer_text_unit(GtkButton *btn, t_client *client) {
//     client->ui->textview = g_object_get_data(G_OBJECT(btn), "textview");
//     GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(client->ui->textview));
//     gchar *buf = mx_get_text_from_buffer(buffer);
//     cJSON_Minify(buf);
//     t_dtp *req = mx_request_creation(buf);

//     mx_send(client, req);
//     mx_free_request(&req);
// }

// void mx_activate_content_test(t_client *client) {
//     GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
//     GtkWidget *textview = gtk_text_view_new();
//     GtkWidget *btn = gtk_button_new_with_label("Send");

//     g_object_set_data(G_OBJECT(btn), "textview", textview);
//     gtk_text_view_set_left_margin(GTK_TEXT_VIEW(textview), 8);
//     gtk_text_view_set_top_margin(GTK_TEXT_VIEW(textview), 8);
//     gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);
//     gtk_widget_set_margin_top(box, 15);
//     gtk_widget_set_margin_bottom(box, 15);
//     gtk_widget_set_margin_end(box, 8);
//     gtk_widget_set_margin_start(box, 8);
//     gtk_box_pack_start(GTK_BOX(box), textview, TRUE, TRUE, 5);
//     gtk_box_pack_start(GTK_BOX(box), btn, FALSE, TRUE, 5);
//     g_signal_connect(btn, "clicked", G_CALLBACK(mx_get_buffer_text_unit),
//                      client);
// }

void mx_trim_message(gchar **message) {
    gchar *trimed = NULL;

    if (strlen(*message) > MX_MAX_MESSAGE) {
        trimed = g_strndup(*message, MX_MAX_MESSAGE);
        g_free(*message);
        *message = g_strdup(trimed);
        g_free(trimed);
    }
>>>>>>> f4b639c5eb29a6ad8f2a05fdc87763e60cf1718a
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
<<<<<<< HEAD
}
=======
    // timestamp now

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    assert(strftime(s, sizeof(s), "%c", tm));
    client->msg_time = strdup(s);
    printf("time is %s\n", client->msg_time);

    // time_t t = time(NULL);
    // struct tm tm = *localtime(&t);
    // printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    // char time;
    // time = concat("dfgh", "dfghj");
    // printf("%s", time);

    char *msg_str = malloc(strlen(message) + 1);

    //struct for msg_str  !!!!!!!!!!!!!!!!!!!!!!!!

    client->msg_from_client = *(t_msg_from_client *)malloc(sizeof(t_msg_from_client));

    strcpy(msg_str, message);
    gtk_entry_set_text(GTK_ENTRY(client->ui->text_msg), "");
    message_send(msg_str);
    client->msg_from_client.msg_str = msg_str;

    // ------------------ new code from Alina ---------------------------
    // ------------------ added lines 133, 137, 138 ---------------------
    pthread_mutex_lock(&client->msg_sig_mut);

    client->msg_req = mx_message(client, client->scene);

    pthread_mutex_unlock(&client->msg_sig_mut);
    pthread_cond_signal(&client->msg_cond);
    // ------------------ ------------------- ---------------------------
>>>>>>> 776fb99933f8b8209e3104d5ba96bfa712a9a43f


void mx_switch_room_header(t_client *client, GtkBuilder *builder, gint page_index) {
    client->ui->stack = gtk_builder_get_object(builder, "stack_room_header");
    GObject *page = NULL;

<<<<<<< HEAD
    if (page_index == MX_ROOM_CTRL)
        page = gtk_builder_get_object(builder, "box_room_ctrl");
    else if (page_index == MX_MSG_CTRL)
        page = gtk_builder_get_object(builder, "box_msg_ctrl");
    gtk_stack_set_visible_child(GTK_STACK(client->ui->stack), GTK_WIDGET(page));
=======
    // ------------------ new code from Alina ---------------------------
    // --- commented  lines 151 - 154 'cause of conflict with sending to srvr ----
    // ------------------------------------------------------------------
//     if (client->msg_req) printf("message\n%s\n", (client->msg_req));
//     fflush(stdout);
//     free(client->msg_req);
//     client->msg_req = NULL;
//  // ------------------ ------------------- ---------------------------
    // printf("%s\n", msg_str);
    free(msg_str);
>>>>>>> 776fb99933f8b8209e3104d5ba96bfa712a9a43f
}

static void init_chat_window(GtkBuilder *builder, t_client *client) {
    if (!builder) return;
    //window
    client->ui->uchat_client = GTK_WIDGET(gtk_builder_get_object(builder, "uchat_client"));
    if (!client->ui->uchat_client) {
        g_critical("Can`t get uchat_client");
    }

<<<<<<< HEAD
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
=======
    client->ui->button_msg = GTK_WIDGET(gtk_builder_get_object(builder, "button_msg"));
    if (!client->ui->button_msg) {
        g_critical("Error button_msg");
    }

    // client->ui->stack_messages = gtk_stack_set_visible_child(builder, "stack_messages"));
    // if (!client->ui->stack_messages) {
    //     g_critical("Error stack_messages");
    // }



    // client->ui->field = GTK_WIDGET(gtk_builder_get_object(builder, "field"));
    // if (!client->ui->field) {
    //     g_critical("Error field");
    // }

    // client->ui->entry_msg = GTK_WIDGET(gtk_builder_get_object(builder, "entry_msg"));
    // if (!client->ui->entry_msg) {
    //     g_critical("Error button");
    // }

    // client->ui->messagesTreeView = GTK_WIDGET(gtk_builder_get_object(builder,"messagesTreeView"));
    // if (!client->ui->messagesTreeView) {
    //     g_critical("Error messagesTreeView");
    // }

    // client->ui->messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
    // if (!client->ui->messagesListStore) {
    //     g_critical("Error messagesListStore");
>>>>>>> 776fb99933f8b8209e3104d5ba96bfa712a9a43f
    // }
    // else {
    //     gtk_widget_hide(GTK_WIDGET(client->ui->box_entry));
    //     gtk_widget_hide(GTK_WIDGET(client->ui->box_header));
    //     gtk_widget_hide(GTK_WIDGET(client->ui->box_editing));

    // }
<<<<<<< HEAD
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
=======

    // client->ui->vAdjust = gtk_scrolled_window_get_vadjustment(client->ui->scrolledWindow);
    // if(!client->ui->vAdjust) {
    //     g_critical("Error vAdjust");
    // }
    // pthread_create(&watcher, 0, watcher_thread, (void *)client);
    // printf("Hi");

//     // --------------------- Search bar ---------------------------
//     client->ui->search_bar = GTK_WIDGET(gtk_builder_get_object(builder, "search_bar"));
//     if (!client->ui->search_bar)
//         g_critical("Can't get search_bar");
//     client->ui->search_status = GTK_WIDGET(gtk_builder_get_object(builder, "search_status"));
//     if (!client->ui->search_status)
//         g_critical("Can't get search_status");
}

static void init_siganl(t_client *client) {
    g_signal_connect(G_OBJECT(client->ui->text_msg), "activate", G_CALLBACK(message_str), client);
    g_signal_connect(G_OBJECT(client->ui->button_msg), "clicked", G_CALLBACK(message_str), client);

    // --------------------- Search bar ---------------------------
    // g_signal_connect(G_OBJECT(client->ui->search_bar),
    //                           "search-changed",
    //                           G_CALLBACK(mx_do_search_req),
    //                           client);
}

void mx_chat_messenger(t_client *client) {
    //---------- CSS Style ------------//
    // GtkCssProvider *provider = gtk_css_provider_new();
    // char *style = CHAT_MSG_CSS;

    // gtk_css_provider_load_from_path(provider, style, NULL);
    // gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
    //                            GTK_STYLE_PROVIDER(provider),
    //                            GTK_STYLE_PROVIDER_PRIORITY_USER);
    // client->ui->uchat_client = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // g_signal_connect(client->ui->uchat_client, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // gtk_window_set_resizable(GTK_WINDOW(client->ui->uchat_client), FALSE);
    // gtk_window_set_position(GTK_WINDOW(client->ui->uchat_client), GTK_WIN_POS_CENTER);

    // GtkWidget *fixed = gtk_fixed_new();
    // gtk_widget_set_name(fixed, "fixed");
    // gtk_container_add(GTK_CONTAINER(client->ui->uchat_client), fixed);
    // // fill_with_contant(fixed);
    // gtk_widget_show_all(fixed);


    // gtk_widget_show_all(client->ui->uchat_client);
    //---------------------------------//

>>>>>>> 776fb99933f8b8209e3104d5ba96bfa712a9a43f
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
