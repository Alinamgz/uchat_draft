#include "server.h"

void mx_parse_auth_req(t_auth_req *req_parsed, const char *req_str) {
    cJSON *req = cJSON_Parse(req_str);
    cJSON *type = cJSON_GetObjectItemCaseSensitive(req, "type");
    cJSON *username = cJSON_GetObjectItemCaseSensitive(req, "username");
    cJSON *password = cJSON_GetObjectItemCaseSensitive(req, "password");
    cJSON *first_name = NULL;
    cJSON *last_name = NULL;

// TODO: check if strdup here needed. free() all allocated memory when don't need it anymore
    req_parsed->req_type = type->valueint;
    req_parsed->username = strdup(username->valuestring);
    req_parsed->password = strdup(password->valuestring);

    if (req_parsed->req_type == REGISTRATION) {
        first_name = cJSON_GetObjectItemCaseSensitive(req, "first_name");
        last_name = cJSON_GetObjectItemCaseSensitive(req, "last_name");

        req_parsed->first_name = strdup(first_name->valuestring);
        req_parsed->last_name = strdup(last_name->valuestring);
    }
    else {
        req_parsed->first_name = NULL;
        req_parsed->last_name = NULL;
    }

    cJSON_Delete(req);

    printf("\n===================================\n");
    system("leaks -q uchat_server");
    printf("\n===================================\n");
    fflush(stdout);
}
