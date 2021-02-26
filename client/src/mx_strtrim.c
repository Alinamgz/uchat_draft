#include "client.h"

static inline bool isspace(char c) {
    bool rslt = (c == 32 || (c >= 9 && c <= 13)) ? 1 : 0;

    return rslt;
}

void mx_strtrim(char **str) {
    char *tmp = *str;
    int rslt_len = NAME_LEN;
    char rslt[rslt_len];

    memset(rslt, 0, NAME_LEN);

    if (tmp) {
        while (isspace(*tmp))
            tmp++;

        strncpy(rslt, tmp,(rslt_len - 1));

        rslt_len = strlen(rslt);
        while (rslt_len > 0 && isspace(tmp[rslt_len - 1])) {
            rslt[rslt_len - 1] = '\0';
            rslt_len--;
        }
    }
    strcpy(*str, rslt);
}

/*
* white space chars : 
* '' space; 32
* \t horisontal tab; 9
* \n newline; 10
* \v vertical tab; 11
* \f from feed; 12
* \r carrige return; 13
*/
