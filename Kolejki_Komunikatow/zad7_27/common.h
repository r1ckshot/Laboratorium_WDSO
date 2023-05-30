#ifndef COMMON_H
#define COMMON_H

#define M_TYPE 1
#define MAX_MSG_SIZE 100

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};

#endif
