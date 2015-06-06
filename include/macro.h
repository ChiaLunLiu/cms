#ifndef __GEMTEK_MACRO_H__
#define __GEMTEK_MACRO_H__
#include <stdlib.h>
#include <stdio.h>
#define CMSGETVALUE		1	/* get param value */
#define CMSSETVALUE		2	/* set param value */
#define CMSDELVALUE		3	/* del param value from conf file */
#define CMSWRITEPARAMNOW		4
#define CMS_OK       5	/* OK */
#define CMS_FAIL     6	/* FAIL */
#define CMSPUSH 8
#define CMSPOP 9
#define CMSSETMULTIVALUE 10
#define CMSGETMULTIVALUE 11
#define CMSDUMPFILE 12
#define CMSADDVALUE 13 /* add new variable to conf file */

#define LINE_BUF_SIZE 2048  /* buf size for reading line in a file */


/* socket location for server & client */
#define CMS_PORT 12345

/* debug */
#if defined(DEBUG)
 #define dbg(fmt, args...) do{ fprintf(stderr, "%s(%d)/%s: " fmt, \
    __FILE__, __LINE__, __func__, ##args); }while(0)
#else
 #define dbg(fmt, args...) do{}while(0)/* Don't do anything in release builds */
#endif

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); }while(0)

#endif
