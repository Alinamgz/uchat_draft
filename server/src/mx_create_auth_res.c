#include "server.h"

char *mx_create_auth_res(t_auth_req *req_res) {
    char *rslt = NULL;
    cJSON *res = cJSON_CreateObject();

    cJSON_AddNumberToObject(res, "code", req_res->res_code);

    switch(req_res->res_code) {
        case OK:
            cJSON_AddStringToObject(res, "msg", OK_MSG);
            cJSON_AddNumberToObject(res, "uid", req_res->uid);
            cJSON_AddStringToObject(res, "username", req_res->username);
            cJSON_AddStringToObject(res, "first_name", req_res->first_name);
            cJSON_AddStringToObject(res, "last_name", req_res->last_name);
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
        case INTERNAL_SRVR_ERR:
            cJSON_AddStringToObject(res, "msg", INTERNAL_SRVR_ERR_MSG);
            break;
        default:
            cJSON_AddStringToObject(res, "msg", "Unknown status code\n");
            break;
    }

    rslt = cJSON_PrintUnformatted(res);
    cJSON_Delete(res);

    return rslt;
}
