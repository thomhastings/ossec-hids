/* @(#) $Id$ */

/* Copyright (C) 2009 Trend Micro Inc.
 * All right reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */



#ifndef __CRALERT_H
#define __CRALERT_H

#define CRALERT_MAIL_SET    0x001 
#define CRALERT_EXEC_SET    0x002
#define CRALERT_READ_ALL    0x004
#define CRALERT_FP_SET      0x010


/* File queue */
typedef struct _alert_data
{
    int rule;
    int level;
    char *date;
    char *location;
    char *comment;
    char *group;
    char *srcip;
    char *user;
    char **log;
}alert_data;


alert_data *GetAlertData(int flag, FILE *fp);
void FreeAlertData(alert_data *al_data);


#endif
