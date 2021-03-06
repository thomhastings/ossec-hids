/* @(#) $Id$ */

/* Copyright (C) 2009 Trend Micro Inc.
 * All rights reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */


#include "mem_op.h"


/* Add pointer to array. */
void **os_AddPtArray(void *pt, void **array)
{
    int i = 0;
    void **ret = NULL;

    if(array)
    {
        while(array[i])
        {
            i++;
        }
    }

    os_realloc(array, (i + 2)*sizeof(char *), ret);
    ret[i] = pt;
    ret[i + 1] = NULL;

    return(ret);
}


/* Add a string to an array. */
char **os_AddStrArray(char *str, char **array)
{
    int i = 0;
    char **ret = NULL;
    if(array)
    {
        while(array[i])
        {
            i++;
        }
    }

    os_realloc(array, (i + 2)*sizeof(char *), ret);
    os_strdup(str, ret[i]);
    ret[i + 1] = NULL;

    return(ret);
}


/* Check if String is on array (Must be NULL terminated) */
int os_IsStrOnArray(char *str, char **array)
{
    if(!str || !array)
    {
        return(0);
    }

    while(*array)
    {
        if(strcmp(*array, str) == 0)
        {
            return(1);
        }
        array++;
    }
    return(0);
}


/* Clear the memory of one char and one char** */
void os_FreeArray(char *ch1, char **ch2)
{
    /* Cleaning char * */
    if(ch1)
    {
        free(ch1);
        ch1 = NULL;
    }
    
    /* Cleaning chat ** */
    if(ch2)
    {
        char **nch2 = ch2;
            
        while(*ch2 != NULL)
        {
            free(*ch2);
            ch2++;
        }
    
        free(nch2);
        nch2 = NULL;
    }
    
    return;
}


/* os_LoadString: v0.1
 * Allocate memory at "*at" and copy *str to it.
 * If *at already exist, realloc the memory and strcat str
 * on it.
 * It will return the new string on success or NULL on memory error.
 */
char *os_LoadString(char *at, char *str)
{
    if(at == NULL)
    {
        at = strdup(str);
        if(!at)
        {
            merror(MEM_ERROR,ARGV0);
        }
        return(at);
    }
    else /*at is not null. Need to reallocat its memory and copy str to it*/
    {
        char *newat;
        int strsize = strlen(str);
        int finalsize = strsize + strlen(at) + 1;

        newat = realloc(at, finalsize*sizeof(char));
        if(newat == NULL)
        {
            free(at);
            merror(MEM_ERROR,ARGV0);
            return(NULL);
        }
        at = newat;

        strncat(at, str, strsize);
        at[finalsize -1] = '\0';

        return(at);
    }

    return(NULL);
}


/* EOF */
