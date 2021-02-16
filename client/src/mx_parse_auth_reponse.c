#include "client.h"

static void parse_ok_response(t_client *client, cJSON *res);

void mx_parse_n_proceed_auth_response(t_client *client, char *res_buf) {
    int res_code;
    char res_msg[40] = "";

    cJSON *res = cJSON_Parse(res_buf);
    cJSON *code = cJSON_GetObjectItemCaseSensitive(res, "code");
    cJSON *resp_msg = cJSON_GetObjectItemCaseSensitive(res, "msg");

    res_code = code->valueint;

    switch(res_code) {
        case OK:
            parse_ok_response(client, res);
            client->prev_scene = client->scene;
            client->scene = CHAT;
            break;
        case CREATED:
            gtk_label_set_text(GTK_LABEL(client->ui->l_username_status), "");
            gtk_label_set_text(GTK_LABEL(client->ui->l_pass_status), "");
            client->prev_scene = client->scene;
            client->scene = LOGIN;
            break;
        case BAD_REQ:
            gtk_label_set_text(GTK_LABEL(client->ui->r_username_status), resp_msg->valuestring);
            break;
        case UNAUTHORIZED:
            gtk_label_set_text(GTK_LABEL(client->ui->l_username_status), resp_msg->valuestring);
            gtk_label_set_text(GTK_LABEL(client->ui->l_pass_status), resp_msg->valuestring);
            break;
        case NOT_FOUND:
            gtk_label_set_text(GTK_LABEL(client->ui->l_username_status), resp_msg->valuestring);
            break;
        default:
            if (client->scene == LOGIN)
                gtk_label_set_text(GTK_LABEL(client->ui->l_username_status), resp_msg->valuestring);
            else if (client->scene == REGISTRATION)
                gtk_label_set_text(GTK_LABEL(client->ui->r_username_status), resp_msg->valuestring);
            break;
    }

    cJSON_Delete(res);

    printf("============= auth resp pars ===============\n");
    fflush(stdout);
    system("leaks -q uchat");
    printf("============= ============== ===============\n");
    fflush(stdout);
}

static void parse_ok_response(t_client *client, cJSON *res) {
    client->self = (t_self*)malloc(sizeof(t_self));

    cJSON *uid = cJSON_GetObjectItemCaseSensitive(res, "uid");
    cJSON *username = cJSON_GetObjectItemCaseSensitive(res, "username");
    cJSON *first_name = cJSON_GetObjectItemCaseSensitive(res, "first_name");
    cJSON *last_name = cJSON_GetObjectItemCaseSensitive(res, "last_name");

    client->self->uid = uid->valueint;
    client->self->username = username->valuestring;
    client->self->first_name = first_name->valuestring;
    client->self->last_name = last_name->valuestring;
}