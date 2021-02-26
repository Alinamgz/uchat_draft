#pragma once

#define DB_OPEN_ERR "/!\\ ERR! Can't open database: "
#define DB_EXEC_ERR "/!\\ ERR: Can't execute query: "
#define USG_ERR_START "Usage: "
#define USG_ERR_END " [port number]\n"
#define NAME_ERR "Enter the name correctly. Name len: min 2, max 31\n"
#define HANDLER_LOOP_ERR "recv err in while loop in client handling\n"

#define AUTH_RECV_ERR "/!\\Err: recv auth response failed\n"
#define AUTH_SEND_ERR "/!\\Err: send auth response failed\n"
#define DB_UNIQUE_USER_ERR "UNIQUE constraint failed: users.user_name"

// response codes
#define OK 200
#define CREATED 201
#define BAD_REQ 400
#define UNAUTHORIZED 401
#define NOT_FOUND 404
#define INTERNAL_SRVR_ERR 500

#define OK_MSG "Ok\n"
#define CREATED_MSG "New user created\n"
#define BAD_REQ_MSG "Err: username already exists\n"
#define UNAUTHORIZED_MSG "Err: invalid username and/or password\n"
#define NOT_FOUND_MSG "Err: user doesn't exist\n"
#define INTERNAL_SRVR_ERR_MSG "Err: internal server error\n"

