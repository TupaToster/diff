#include "protos.h"

int GetN (char** s) {

    int val = 0;
    assert (**s >= '0' and **s <= '9');
    while (**s >= '0' and **s <= '9') {

        val = 10*val + **s - '0';
        *s += 1;
    }
    return val;
}

int GetP (char** s) {

    int val = 0;
    if (**s == '(') {

        *s += 1;
        val = GetE (s);
        assert (**s == ')');
        *s += 1;
        return val;
    }
    else return GetN (s);
}

int GetT (char** s) {

    int val = GetP (s);

    while (**s == '*' or **s == '/') {

        *s+=1;
        if (*(*s-1) == '*') val *= GetP (s);
        else val /= GetP (s);
    }
    return val;
}

int GetE (char** s) {

    int val = GetT(s);

    while (**s == '-' or **s == '+') {

        *s += 1;
        if (*(*s-1) == '-') val -= GetT (s);
        else val += GetT (s);
    }
    return val;
}

int GetG (char* s) {

    int val = GetE (&s);

    assert (*s == '\0');
    return val;
}
