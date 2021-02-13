#include "client.h"

cJSON *mx_create_auth_req(t_client *client, t_raw_inputs *inputs,
                          t_scene type) {
    cJSON *req = cJSON_CreateObject();
    char *hashed_pass = g_compute_checksum_for_string(
        G_CHECKSUM_SHA256, inputs->password, -1);

    cJSON_AddNumberToObject(req, "type", type);
    cJSON_AddStringToObject(req, "username", inputs->username);
    cJSON_AddStringToObject(req, "password", hashed_pass);

    if (type == REGISTRATION) {
        cJSON_AddStringToObject(req, "first_name", inputs->first_name);
        cJSON_AddStringToObject(req, "last_name", inputs->last_name);
    }

    free(hashed_pass);
    return req;
}
