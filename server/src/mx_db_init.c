#include "server.h"

void mx_db_init(void) {
	sqlite3 *db;
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

	char *insert_init_val_sql = "INSERT INTO users (uid,user_name,password,first_name,last_name)"\
		  		"VALUES (0, '__head', 'head_pass', '__head_name', 'head_lastname');"\
				"INSERT INTO connected_users (connection_id,uid,fd)"\
		  		"VALUES (0, 0, -1);";

	if (sqlite3_open("uchat.db", &db)) {
		fprintf(stderr, "%s%s\n", DB_OPEN_ERR, sqlite3_errmsg(db));
	}

	if (sqlite3_exec(db, create_users_sql, NULL, NULL, &err)) {
		fprintf(stderr, "%s%s\n", DB_EXEC_ERR, sqlite3_errmsg(db));
		free(err);
	}
	else {
		printf("Created table users\n");
	}

	if (sqlite3_exec(db, create_connected_users_sql, NULL, NULL, &err)) {
		fprintf(stderr, "%s%s\n", DB_EXEC_ERR, sqlite3_errmsg(db));
		free(err);
	}
	else {
		printf("Created table connected_users\n");
	}

	if (sqlite3_exec(db, insert_init_val_sql, NULL, NULL, &err)) {
		fprintf(stderr, "%s%s\n", DB_EXEC_ERR, sqlite3_errmsg(db));
		free(err);
	}
	else {
		printf("Inserted init values\n");
	}

	sqlite3_close(db);
	return;
}
