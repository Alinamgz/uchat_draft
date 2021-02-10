#include "client.h"

bool mx_isalpha_digital(char c) {

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
