#include "client.h"

cJSON *mx_create_auth_req(t_client *client, t_raw_inputs *inputs, t_scene type) {
    cJSON *req = cJSON_CreateObject();
    cJSON *rq_type = NULL;
    cJSON *user_name = cJSON_CreateString(inputs->username);
	cJSON *password = cJSON_CreateString(inputs->password);
	cJSON *conf_password = cJSON_CreateString(inputs->conf_password);

    cJSON *first_name = inputs->first_name ? cJSON_CreateString(inputs->first_name) : NULL;
    cJSON *last_name = inputs->last_name ? cJSON_CreateString(inputs->last_name) : NULL;

    switch(type) {
        case LOGIN:
            rq_type = cJSON_CreateString("login");
            break;
        case REGISTRATION:
            rq_type = cJSON_CreateString("registration");
            break;
        default:
            break;
    }

    cJSON_AddItemToObject(req, "type", rq_type);
    cJSON_AddItemToObject(req, "user_name", user_name);
    cJSON_AddItemToObject(req, "password", password);

    if (type == LOGIN) {
        cJSON_AddNullToObject(req, "conf_password");
        cJSON_AddNullToObject(req, "first_name");
        cJSON_AddNullToObject(req, "last_name");
    }
    else if (type == REGISTRATION) {
        cJSON_AddItemToObject(req, "conf_password", conf_password);
        if (first_name) {
            cJSON_AddItemToObject(req, "first_name", first_name);
        }
        else {
            cJSON_AddNullToObject(req, "first_name");
        }

        if (last_name) {
            cJSON_AddItemToObject(req, "last_name", last_name);
        }
        else {
            cJSON_AddNullToObject(req, "last_name");
        }
    }

    return req;
}
