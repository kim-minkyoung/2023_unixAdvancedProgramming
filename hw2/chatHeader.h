#ifndef __CHAT_SHARE_MEMORY_H__
#define __CHAT_SHARE_MEMORY_H__

typedef struct chatinfo{
    char             userID[20];
    long             messageTime;
    char        message[40];
} CHAT_INFO;

#endif//__CHAT_SHARE_MEMORY_H__
