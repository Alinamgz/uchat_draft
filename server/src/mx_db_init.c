#include "server.h"

void mx_db_init(t_cl_data *client) {
	// sqlite3 *db;
	char *err = NULL;
	char *create_users_sql = "CREATE TABLE IF NOT EXISTS users ("\
				"uid		INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ,"\
				"user_name  VARCHAR NOT NULL UNIQUE,"\
				"password   VARCHAR NOT NULL,"\
				"first_name VRCHAR NOT_NULL,"\
				"last_name 	VARCHAR );";

	char *create_connected_users_sql = "CREATE TABLE IF NOT EXISTS connected_users ("\
				"connection_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL  ,"\
				"uid		INTEGER NOT NULL ,"\
				"fd			INTEGER NOT NULL );";

	char *create_chats_table_sql = "CREATE TABLE IF NOT EXISTS chats ("\
				"chat_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"\
				"chat_name VARCHAR NOT NULL,"\
				"from_uid INTEGER NOT_NULL,"\
				"to_uid INTEGER NOT_NULL );";

// TODO: delete qwer and gogi
	char *insert_init_val_sql = "INSERT OR IGNORE INTO users (uid,user_name,password,first_name,last_name)"\
		  		"VALUES (0, '__head', 'head_pass', '__head_name', 'head_lastname');"\
				"INSERT OR IGNORE INTO users (user_name,password,first_name,last_name)"\
		  		"VALUES ('qwer', 'f6f2ea8f45d8a057c9566a33f99474da2e5c6a6604d736121650e2730c6fb0a3', 'qwer', '');"\
				/*
				"INSERT OR IGNORE INTO users (user_name,password,first_name,last_name)"\
		  		"VALUES ('gogi', '9fbccf7674cf9191d7a16fa528a89b6ba3b894d3aa947f4401b500e55603ac2c', '', '');" */ \
				"INSERT OR IGNORE INTO connected_users (connection_id,uid,fd)"\
		  		"VALUES (0, 0, -1);";

	if (sqlite3_open("uchat.db", &client->db)) {
		fprintf(stderr, "%s%s\n", DB_OPEN_ERR, sqlite3_errmsg(client->db));
	}

	if (sqlite3_exec(client->db, create_users_sql, NULL, NULL, &err)) {
		fprintf(stderr, "%s%s\n", DB_EXEC_ERR, sqlite3_errmsg(client->db));
		free(err);
	}
	else {
		printf("Created table users\n");
	}

	if (sqlite3_exec(client->db, create_connected_users_sql, NULL, NULL, &err)) {
		fprintf(stderr, "%s%s\n", DB_EXEC_ERR, sqlite3_errmsg(client->db));
		free(err);
	}
	else {
		printf("Created table connected_users\n");
	}

	if (sqlite3_exec(client->db, create_chats_table_sql, NULL, NULL, &err)) {
		fprintf(stderr, "%s%s\n", DB_EXEC_ERR, sqlite3_errmsg(client->db));
		free(err);
	}
	else {
		printf("Created table for chats\n");
	}

// TODO: first check for _head, then add if neeed;
	if (sqlite3_exec(client->db, insert_init_val_sql, NULL, NULL, &err)) {
		fprintf(stderr, "%s%s\n", DB_EXEC_ERR, sqlite3_errmsg(client->db));
		free(err);
	}
	else {
		printf("Inserted init values\n");
	}

	// sqlite3_close(client->db);
	return;
}
