#include "client.h"

void mx_send_message_handle_shift(GtkTextView *textview, GdkEvent *event, t_client *client) {
    if (event->key.keyval == MX_KEY_SHIFT)
        client->ui->shift_hold = FALSE;
    (void)textview;
}

static void log_lvl(FILE *fd, GLogLevelFlags flags) {
    if (flags == G_LOG_LEVEL_WARNING)
        g_fprintf(fd, "WARNING  ");
    else if (flags == G_LOG_LEVEL_MESSAGE)
        g_fprintf(fd, "MESSAGE  ");
    else if (flags == G_LOG_LEVEL_ERROR)
        g_fprintf(fd, "ERROR  ");
}

/*
 * Logs string to file
 */

void mx_logger(gchar *file_name, GLogLevelFlags flags, gchar *error) {
    FILE *fd = fopen(file_name, "a");
    gchar *date = NULL;
    GDateTime *dt = g_date_time_new_now_local();

    date = g_date_time_format(dt, "%T %e.%m.%Y");
    g_fprintf(fd, "date:%s  ", date);
    g_fprintf(fd, "pid:%d  ", getpid());
    log_lvl(fd, flags);
    g_fprintf(fd, "'%s'\n", error);
    g_free(date);
    fclose(fd);
    if (flags == G_LOG_LEVEL_ERROR)
        exit(1);
}

gssize mx_send(t_client *client, t_dtp *dtp) {
    GError *error = NULL;
    gsize size = 0;

    if (dtp) {
        if ((size = g_data_output_stream_put_string(client->ui->out, dtp->str, NULL, &error)) <= 0) {
            mx_logger(MX_LOG_FILE, G_LOG_LEVEL_WARNING, "Write string error");
        }
        if (error) {
            mx_logger(MX_LOG_FILE, G_LOG_LEVEL_WARNING, "Error send");
            g_clear_error(&error);
        }
    }
    return size;
}

void mx_free(void **ptr) {
    if (ptr && *ptr) {
        free(*ptr);
        *ptr = NULL;
    }
}

void mx_free_request(t_dtp **request) {
    if (request && *request) {
        cJSON_Delete((*request)->json);
        mx_free((void**)request);
    }
}

t_dtp *mx_get_transport_data(cJSON *json_result) {
    t_dtp *dtp = NULL;
    char *json_str = NULL;

    json_str = cJSON_PrintUnformatted(json_result);
    dtp = mx_request_creation(json_str);
    mx_free((void**)&json_str);
    cJSON_Delete(json_result);
    return dtp;
}

t_dtp *mx_msg_request(char *msg, t_client *client) {
    cJSON *json_result = cJSON_CreateObject();

    if (!cJSON_AddNumberToObject(json_result, "type", client->self->uid))
        return NULL;
    // if (!cJSON_AddNumberToObject(json_result, "room_id", room_id))
    //     return NULL;
    // if (!cJSON_AddStringToObject(json_result, "message", MX_J_STR(msg)))
    //     return NULL;
    // if (!cJSON_AddNumberToObject(json_result, "msg_type", DB_TEXT_MSG))
    //     return NULL;
    return mx_get_transport_data(json_result);
}

void mx_req_send_message(GtkButton *btn, t_client *client) {
    gchar *message_text = mx_get_buffer_text("buffer_message", client);
    // t_groom *room = mx_get_selected_groom(client,  MX_LOCAL_ROOMS);
    t_dtp *dtp = NULL;

    printf("start button 1\n");

    g_strstrip(message_text);
    printf("%s\n - send\n", message_text);
    // if (room && !chat->msg_placeholder && strlen(message_text) > 0) {
    if (!client->ui->msg_placeholder && strlen(message_text) > 0) {
        mx_trim_message(&message_text);
        // dtp = mx_msg_request(message_text, room->id);
        // dtp = mx_msg_request(message_text, client);
        mx_send(client, dtp);
        mx_free_request(&dtp);
        mx_clear_buffer_text("buffer_message", client->ui->builder);
    }
    g_free(message_text);
    // mx_reset_messege_room(NULL, client);
    (void)btn;
}

void mx_req_edit_message(GtkButton *btn, t_client *client) {
    // t_gmsg *gmsg = mx_get_selected_gmsg(client);
    gchar *new_text = mx_get_buffer_text("buffer_message", client);
    t_dtp *dtp = NULL;

    g_strstrip(new_text);
    if (strcmp(client->msg, new_text) && strlen(new_text) > 0) {
        mx_trim_message(&new_text);
        // dtp = mx_edit_msg_request(new_text, gmsg->room_id, gmsg->message_id);
        mx_send(client, dtp);
        mx_free_request(&dtp);
    }
    // else
    //     mx_switch_room_header(chat->builder, MX_MSG_CTRL);
    mx_clear_buffer_text("buffer_message", client->ui->builder);
    // mx_hide_msg_editing(NULL, chat->builder);
    g_free(new_text);
    (void)btn;
}
