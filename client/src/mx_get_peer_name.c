#include "client.h"

void mx_get_peer_name(t_client *client, char *chat_name) {
    char *buf = strdup(chat_name);

    char *ptr = strtok(buf, " <-> ");

    while (ptr && client) {
        if (strcmp(client->self->username, ptr)) {
            client->selected_chat->peer_name = strdup(ptr);
            break;
        }

        ptr = strtok(NULL, " <-> ");
    }

    printf("me: %s, peer: %s\n", client->self->username, client->selected_chat->peer_name);

    free(buf);
}
