#include "client.h"

static char *validate_input(const gchar *str, bool is_req);
static bool is_alpha_digital(char c);

const gchar *mx_get_n_check_entry(char **err, bool is_req, gpointer entry, gpointer status) {
    const gchar *rslt = gtk_entry_get_text(GTK_ENTRY(entry));

    *err = validate_input(rslt, is_req);
    gtk_label_set_text(GTK_LABEL(status), *err);

    if (err[0])
        gtk_widget_grab_focus(entry);

    return rslt;
}

static char *validate_input(const gchar *str, bool is_req) {
    if (is_req && (!str || !*str))
        return "Required field!";

    if (is_req &&  strlen(str) < 4)
        return "Min length is 4 chars";

    for (int i = 0; str[i]; i++) {
        if (! is_alpha_digital(str[i])) {
            return "Only letters and numbers allowed";
        }
    }
    return "";
}

static bool is_alpha_digital(char c) {

    if (c >= '0' && c < '9') {
        return 1;
    }
    else if (c >= 'A' && c < 'Z') {
        return 1;
    }
    else if (c >= 'a' && c <= 'z') {
        return 1;
    }
    else {
        return 0;
    }
}
