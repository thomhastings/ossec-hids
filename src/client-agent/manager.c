/*   $OSSEC, manager.c, v0.1, 2005/09/24, Daniel B. Cid$   */

/* Copyright (C) 2005 Daniel B. Cid <dcid@ossec.net>
 * All right reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include <errno.h>

#include "os_crypto/md5/md5_op.h"
#include "os_net/os_net.h"
#include "headers/defs.h"
#include "headers/debug_op.h"
#include "headers/file_op.h"
#include "error_messages/error_messages.h"

#include "agentd.h"

/* getfiles: Return the name of the files in a directory
 */
char *getsharedfiles()
{
    int m_size = 512;

    DIR *dp;

    struct dirent *entry;

    char *ret;
    char *tmp_ret;
    
    os_md5 md5sum;
    
    /* Opening the directory given */
    dp = opendir(SHAREDCFG_DIR);
    if(!dp) 
    {
        merror("%s: Error opening directory: '%s': %s ",
                ARGV0,
                SHAREDCFG_DIR,
                strerror(errno));
        return(NULL);
    }   


    /* we control these files, max size is m_size */
    ret = (char *)calloc(m_size, sizeof(char));
    if(!ret)
    {
        merror(MEM_ERROR);
        return(NULL);
    }
    tmp_ret = ret;

    while((entry = readdir(dp)) != NULL)
    {
        char tmp_dir[256];
        
        /* Just ignore . and ..  */
        if((strcmp(entry->d_name,".") == 0) ||
                (strcmp(entry->d_name,"..") == 0))
            continue;

        snprintf(tmp_dir, 255, "%s/%s", SHAREDCFG_DIR, entry->d_name);

        if(OS_MD5_File(tmp_dir, md5sum) != 0)
        {
            merror("%s: Error accessing file '%s'",tmp_dir);
            continue;
        }
        
        snprintf(tmp_ret, m_size, "%s %s\n",    tmp_dir,
                                                md5sum);
        
        m_size-=strlen(tmp_ret);

        tmp_ret+=strlen(tmp_ret);
       
        if(*tmp_ret == '\n')
        {
            printf("im \\n lala\n");
            tmp_ret++;
        }
        
    }

    closedir(dp);

    /* If we didn't use ret, free it and return null */
    if(*ret == '\0')
    {
        free(ret);
        ret = NULL;
    }
    
    return(ret);
}

/* start_mgr: Start manager thread */
void *start_mgr(void *arg)
{
    int sock;
    int *port = (int *)arg;
    int msg_size;

    
    char tmp_msg[1024];
    char *crypt_msg;
    char *uname;
    char *shared_files;
    
    printf("Starting manager thread on port %d..\n", *port);


    /* Connect to the server */
    if((sock =  OS_ConnectTCP(*port, logr->rip)) < 0)
    {
        merror(CONNS_ERROR, ARGV0, logr->rip);
        return(NULL);
    }
    
    /* Send the message.
     * Message is going to be the 
     * uname : file checksum : file checksum :
     */   
    
    /* Getting uname */
    uname = getuname();
    if(!uname)
    {
        uname = strdup("No system info available");
        if(!uname)
        {
            merror(MEM_ERROR,ARGV0);
            return(NULL);
        }
    }
    
    /* get shared files */
    shared_files = getsharedfiles();
    if(!shared_files)
    {
        shared_files = strdup("\0");
        if(!shared_files)
        {
            merror(MEM_ERROR,ARGV0);
            return(NULL);
        }
    }
    printf("connected \n");
    
    /* creating message */
    snprintf(tmp_msg, 1024, "%s\n%s",uname, shared_files);
    
    crypt_msg = CreateSecMSG(&keys, tmp_msg, 0, &msg_size);
    
    if(crypt_msg == NULL)
    {
        merror(SEC_ERROR,ARGV0);
        return(NULL);
    }
   
    /* sending message */
    if(write(sock, crypt_msg, msg_size) < msg_size)
    {
        merror("%s: Error sending message to server (write)",ARGV0);
    }
                                                                                     
    printf("message sent!\n");
    free(crypt_msg);
    return(NULL);
}

/* EOF */