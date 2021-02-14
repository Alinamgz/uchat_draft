#include "server.h"

char *mx_create_auth_res(int res_code) {
    char *rslt = NULL;
    cJSON *res = cJSON_CreateObject();

    cJSON_AddNumberToObject(res, "code", res_code);

    switch(res_code) {
        case OK:
            cJSON_AddStringToObject(res, "msg", OK_MSG);
            break;
        case CREATED:
            cJSON_AddStringToObject(res, "msg", CREATED_MSG);
            break;
        case BAD_REQ:
            cJSON_AddStringToObject(res, "msg", BAD_REQ_MSG);
            break;
        case UNAUTHORIZED:
            cJSON_AddStringToObject(res, "msg", UNAUTHORIZED_MSG);
            break;
        case NOT_FOUND:
            cJSON_AddStringToObject(res, "msg", NOT_FOUND_MSG);
            break;
        default:
            cJSON_AddStringToObject(res, "msg", "Unknown status code\n");
            break;
    }

    rslt = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);

    return rslt;
}
