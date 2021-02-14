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

	char *insert_init_val_sql = "INSERT OR IGNORE INTO users (uid,user_name,password,first_name,last_name)"\
		  		"VALUES (0, '__head', 'head_pass', '__head_name', 'head_lastname');"\
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
