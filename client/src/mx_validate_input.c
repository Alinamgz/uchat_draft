#include "client.h"

char *mx_validate_input(const gchar *str) {
    if (!str || !*str)
        return "Required field!";

    if (strlen(str) < 4)
        return "Min length is 4 chars";

    for (int i = 0; str[i]; i++) {
        if (! mx_isalpha_digital(str[i])) {
            return "Only letters and numbers allowed";
        }
    }
    return "";
}
