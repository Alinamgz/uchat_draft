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

    //client->msg_from_client = (t_msg_from_client*)malloc(sizeof(t_msg_from_client));
    // t_msg_from_client
    // client->msg_from_client.msg_str = (char *)malloc(strlen(msg_str) * sizeof(char));
    client->msg_from_client = *(t_msg_from_client *)malloc(sizeof(t_msg_from_client));

    strcpy(msg_str, message);
    gtk_entry_set_text(GTK_ENTRY(client->ui->field), "");
    message_send(msg_str);
    client->msg_from_client.msg_str = msg_str;
    printf("%s\n",client->msg_from_client.msg_str);
    msg = mx_message(client, msg_str, client->scene);
    // GtkTreeIter iter;
    // gtk_list_store_append(GTK_LIST_STORE(client->ui->messagesListStore), &iter);
    // gtk_list_store_set(GTK_LIST_STORE(client->ui->messagesListStore), &iter, 0, msg_str, -1);
    // gtk_adjustment_set_value(client->ui->vAdjust, gtk_adjustment_get_upper(client->ui->vAdjust) - gtk_adjustment_get_page_size(client->ui->vAdjust));
    if (msg) printf("message\n%s\n", cJSON_Print(msg));
    // printf("%s\n", msg_str);
    free(msg_str);
}
/////////////////////////////

static void write_num(unsigned a, char *buf)
{
    *(unsigned*)buf = (a << 24) | ((a & 0x0000ff00) << 8) | ((a & 0x00ff0000) >> 8) | (a >> 24);
}

void proto_free(struct chat_msg *s)
{
    if(!s)
        return;
    if(!s->lines)
    {
        free(s);
        return;
    }
    for(unsigned i=0;i<s->line_count;++i)
    {
        if(s->lines[i].data)
            free(s->lines[i].data);
    }
    free(s->lines);
    free(s);
}

unsigned proto_encode(struct chat_msg *msg, char **buf)
{
    unsigned length = 5;
    for(unsigned i=0;i<msg->line_count;++i)
    {
        length += 4 + msg->lines[i].length;
    }
    *buf = malloc(length);
    char *data = *buf;
    *data++ = msg->type;
    write_num(length - 5, data);
    data += 4;
    for(unsigned i=0;i<msg->line_count;++i)
    {
        write_num(msg->lines[i].length, data);
        data += 4;
        if(msg->lines[i].length)
            memcpy(data, msg->lines[i].data, msg->lines[i].length);
        data += msg->lines[i].length;
    }
    return length;
}

static void _send(struct chat_msg *p)
{
    char *buf;
    unsigned len = proto_encode(p, &buf);
    proto_free(p);
    send(sock, buf, len, 0);
    free(buf);
}


void proto_set_int(struct chat_msg *msg, unsigned line_no, int value)
{
    if(line_no >= msg->line_count)
        return;
    if(msg->lines[line_no].data)
        free(msg->lines[line_no].data);
    msg->lines[line_no].length = 4;
    msg->lines[line_no].data = malloc(5);
    write_num(value, msg->lines[line_no].data);
}

void message_request_history(int cnt)
{
    struct chat_msg *p = chat_message('h', 1);
    proto_set_int(p, 0, cnt);
    _send(p);
}

static unsigned read_num(char *buf) {
    unsigned a = *(unsigned*)buf;
    return (a << 24) | ((a & 0x0000ff00) << 8) | ((a & 0x00ff0000) >> 8) | (a >> 24);
}

static int recv_bytes(char *buf, int sock, int len) {
    while(len) {
        int t = recv(sock, buf, len, 0);
        if(t <= 0)
            return (t);
        len -= t;
        buf += t;
    }
    return 1;
}

int recv_packet(char *buf, int sock) {
    int r = recv_bytes(buf, sock, 5);
    if (r <= 0)
        return (0);
    int len = read_num(buf + 1);
    if (len > MESSAGE_BUF_SIZE - 5)
        return (0);
    r = recv_bytes(buf + 5, sock, len);
    if (r <= 0)
        return (0);
    return len + 5;
}

int message_receive(char **author, char **body) {
    int len = recv_packet(msg_buf, sock);
    if (len <= 0) {
        return (-1);
    }
    return 0;
}

void sleep_ms(int milliseconds)
{
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void add_list_entry(const char *a, const char *m, int sleep, t_client *client)
{
    GtkTreeIter iter;
    gtk_list_store_append(GTK_LIST_STORE(client->ui->messagesListStore), &iter);
    gtk_list_store_set(GTK_LIST_STORE(client->ui->messagesListStore), &iter, 0, a, 1, m, -1);
    if(sleep)
        sleep_ms(100);
    gtk_adjustment_set_value(client->ui->vAdjust, gtk_adjustment_get_upper(client->ui->vAdjust) - gtk_adjustment_get_page_size(client->ui->vAdjust));
}

void *watcher_thread(void *param) {
    // (void)param;
    t_client *client = (t_client *)param;
    char *author, *body;

    printf("%s\n",client->msg_from_client.msg_str);

    message_request_history(10);
    while (1) {
        int k = message_receive(&author, &body);
        if (k < 0) {
            gtk_widget_set_sensitive(client->ui->entry_msg, 1);
            break;
        }
        if (k == 0) {
            continue;
        }
        if(!author) {
            continue;
        }
        add_list_entry(author, body, k != 'h', client);
        printf("%s", body);
        free(author);
        free(body);
    }
    return param;
}

////////////////////////////
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
    pthread_create(&watcher, 0, watcher_thread, (void *)client);
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
