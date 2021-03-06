#include "text.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char *pool=NULL;
int  next=0;
int  allocated=0;
int  increment=10000;

void text_increase_size ( int required )
{
    if ( required > increment ) increment = required;
    pool = realloc ( pool, allocated + increment );
    if ( pool == NULL ) {
        fprintf(stderr,"Could not create text pool.  size=%d\n",
            allocated+increment);
        exit(1);
    }
    allocated += increment;
    increment *= 2;
}

Text text_create(char *s)
{
    int t;
    int len;

    t = next;
    len = strlen(s);
    if ( t + len + 1 > allocated ) text_increase_size ( len+1 );
    strcpy ( pool+t, s );
    next = t + len + 1;
    return t;
}

Text text_cats(Text t, char *s)
{
    int new_length;
    int len_t;
    int len_s;
    int v;

    len_t = strlen(pool+t);
    len_s = strlen(s);
    v = next;
    new_length = len_s + len_t;
    if ( t + new_length + 1 > allocated ) text_increase_size ( new_length+1 );
    strcpy ( pool+v, pool+t );
    strcat ( pool+v+len_t, s );
    next = v+len_t+len_s+1;
    return v;
}

int text_cat(Text t, Text s)
{
    int new_length;
    int len_t;
    int len_s;
    int v;

    len_t = strlen(pool+t);
    len_s = strlen(pool+s);
    v = next;
    new_length = len_s + len_t;
    if ( t + new_length + 1 > allocated ) text_increase_size ( new_length+1 );
    strcpy ( pool+v, pool+t );
    strcat ( pool+v+len_t, pool+s );
    next = v+len_t+len_s+1;
    return v;
}

char *text_string ( Text t )
{
    return pool+t;
}
