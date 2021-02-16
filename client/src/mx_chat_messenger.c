#include "client.h"

pthread_t watcher;
int sock = 0;
char msg_buf[MESSAGE_BUF_SIZE];

cJSON *mx_message(t_client *client, const char *str, t_scene type) {
    cJSON *str_line = cJSON_CreateObject();
    cJSON *rq_type = NULL;
    cJSON *msg = cJSON_CreateString(str);

    // printf("%s\n", str->string);
    // cJSON *message = msg_chat->string ? cJSON_CreateString(msg_chat->string) : NULL;

    cJSON_AddItemToObject(str_line, "message", msg);

    // if (type == CHAT) {
    //     cJSON_AddNullToObject(str_line, "string", msg);
    // }
    return str_line;
}

struct chat_msg *chat_message(char type, unsigned line_count) {
    struct chat_msg *res = malloc(sizeof(struct chat_msg));
    res->type = type;
    res->line_count = line_count;
    res->lines = calloc(line_count, sizeof(struct proto_line));
    return res;
}

void message_str_len(struct chat_msg *msg, unsigned line_no, const char *value, unsigned len) {
    if (line_no >= msg->line_count)
        return;
    if (msg->lines[line_no].data)
        free(msg->lines[line_no].data);
    msg->lines[line_no].length = len;
    msg->lines[line_no].data = malloc(len + 1);
    if (len)
        memcpy(msg->lines[line_no].data, value, len);
    msg->lines[line_no].data[len] = 0;
}

void message_string(struct chat_msg *msg, unsigned line_no, const char *value) {
    message_str_len(msg, line_no, value, strlen(value));
}

void message_send(const char *type) {
    struct chat_msg *msg = chat_message('r', 1);
    message_string(msg, 0, type);
}

void message_str(GtkWidget *widget,  gpointer data) {
    t_client *client = (t_client *)data;
    // struct msg_chat string = {NULL};
    cJSON *msg = NULL;

    if(!gtk_widget_get_sensitive(client->ui->entry_msg))
        return;
    const gchar *message = gtk_entry_get_text(GTK_ENTRY(client->ui->field));
    if (!message || !*message)
        return;
    if (message[0] == '\\' && message[1]) {
        if (message[1] == 'h' && (!message[2] || message[2] == ' ')) {
            gtk_entry_set_text(GTK_ENTRY(client->ui->field), "");
            return;
        }
        if (message[1] == 'l' && (!message[2] || message[2] == ' ')) {
            // message_list();
            gtk_entry_set_text(GTK_ENTRY(client->ui->field), "");
            return;
        }
        if (message[1] == 'k' && message[2] == ' ') {
            int uid;
            char num[16];
            int i = 3, j = 0;
            while (message[i] == ' ')
                ++i;
            while (j < 15 & message[i] != ' ' && message[i])
                num[j++] = message[i++];
            if (j == 15) {
                gtk_entry_set_text(GTK_ENTRY(client->ui->field), "");
                return;
            }
            num[j] = 0;
            sscanf(num, "%d", &uid);
            while(message[i] == ' ')
                ++i;
            // message_kick_user(uid, &message[i]);
            gtk_entry_set_text(GTK_ENTRY(client->ui->field), "");
            return;
        }
        gtk_entry_set_text(GTK_ENTRY(client->ui->field), "");
        return;
    }

    char *msg_str = malloc(strlen(message) + 1);

    //struct for msg_str  !!!!!!!!!!!!!!!!!!!!!!!!


    client->msg_from_client = *(t_msg_from_client *)malloc(sizeof(t_msg_from_client));

    strcpy(msg_str, message);
    gtk_entry_set_text(GTK_ENTRY(client->ui->field), "");
    message_send(msg_str);
    client->msg_from_client.msg_str = msg_str;
    printf("%s\n",client->msg_from_client.msg_str);
    msg = mx_message(client, msg_str, client->scene);
    GtkTreeIter iter;
    gtk_list_store_append(GTK_LIST_STORE(client->ui->messagesListStore), &iter);
    gtk_list_store_set(GTK_LIST_STORE(client->ui->messagesListStore), &iter, 0, msg_str, -1);
    gtk_adjustment_set_value(client->ui->vAdjust, gtk_adjustment_get_upper(client->ui->vAdjust) - gtk_adjustment_get_page_size(client->ui->vAdjust));
    if (msg) printf("message\n%s\n", cJSON_Print(msg));
    // printf("%s\n", msg_str);
    free(msg_str);
}

static void init_chat_window(GtkBuilder * builder, t_client *client) {
    if (!builder) return;

    //window
    client->ui->uchat_client = GTK_WIDGET(gtk_builder_get_object(builder, "uchat_client"));
    if (!client->ui->uchat_client)
        g_critical("Can`t get uchat_client");

    client->ui->field = GTK_WIDGET(gtk_builder_get_object(builder, "field"));
    if (!client->ui->field) {
        g_critical("Error field");
    }

    client->ui->entry_msg = GTK_WIDGET(gtk_builder_get_object(builder, "entry_msg"));
    if (!client->ui->entry_msg) {
        g_critical("Error button");
    }

    client->ui->messagesTreeView = GTK_WIDGET(gtk_builder_get_object(builder,"messagesTreeView"));
    if (!client->ui->messagesTreeView) {
        g_critical("Error messagesTreeView");
    }

    client->ui->messagesListStore = GTK_LIST_STORE(gtk_builder_get_object(builder,"messagesListStore"));
    if (!client->ui->messagesListStore) {
        g_critical("Error messagesListStore");
    }

    client->ui->scrolledWindow = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder,"scrolledWindow"));
    if(!client->ui->scrolledWindow) {
        g_critical("Error scrolledWindow");
    }

    client->ui->vAdjust = gtk_scrolled_window_get_vadjustment(client->ui->scrolledWindow);
    if(!client->ui->vAdjust) {
        g_critical("Error vAdjust");
    }
    // pthread_create(&watcher, 0, watcher_thread, (void *)client);
    // printf("Hi");
}

static void init_siganl(t_client *client) {
    g_signal_connect(G_OBJECT(client->ui->field), "activate", G_CALLBACK(message_str), client);
    g_signal_connect(G_OBJECT(client->ui->entry_msg), "clicked", G_CALLBACK(message_str), client);
}

void mx_chat_messenger(t_client *client) {
    GtkBuilder *builder = gtk_builder_new_from_file("client/templates/chat.glade");
    if (!builder) {
        g_critical("Builder getting error!");
    }
    
    gtk_builder_connect_signals(builder, NULL);

    init_chat_window(builder, client);
    init_siganl(client);

    g_object_unref(builder);
}
