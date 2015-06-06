/**************************************************************************
 *
 * GEMTEK CONFIDENTIAL
 *
 * Copyright 2006-2007 Gemtek Technology Co.,Ltd. All Rights Reserved.
 *
 * The source code contained or described herein and all documents related
 * to the source code ("Material") are owned by Gemtek Technology Co.,Ltd.
 * or its suppliers or licensors. Title to the Material remains with
 * Gemtek Technology Co.,Ltd. or its suppliers and licensors. The Material
 * contains trade secrets and proprietary and confidential information of
 * Gemtek or its suppliers and licensors. The Material is protected by
 * worldwide copyright and trade secret laws and treaty provisions.
 * No part of the Material may be used, copied, reproduced, modified,
 * published, uploaded, posted, transmitted, distributed, or disclosed
 * in any way without Gemtek's prior express written permission.
 *
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you by
 * disclosure or delivery of the Materials, either expressly, by
 * implication, inducement, estoppel or otherwise. Any license under such
 * intellectual property rights must be express and approved by Gemtek in
 * writing.
 *
 **************************************************************************/
#include "macro.h"
#include "cmc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<getopt.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define CMSCFG_GETVALUE 1
#define CMSCFG_SETVALUE 2
#define CMSCFG_DELVALUE 3
#define CMSCFG_ADDVALUE 4
#define CMSCFG_DUMPFILE 6
#define CMSCFG_FLUSH 7
#define CMSCFG_PUSH 8
#define CMSCFG_POP  9
#define CMSCFG_SETMULTIVALUE  10
#define CMSCFG_GETMULTIVALUE  11

char* debug;

#define dbg(fmt, args...) do{ \
	if(debug) \
	fprintf(stderr, "%s(%d)/%s: " fmt, \
    __FILE__, __LINE__, __func__, ##args); }while(0)

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); }while(0)

static const char *optstring="hsadgwpoq:Su:n:v:f:G:";

extern char	*optarg;

void printUsage()
{
	printf("Command Usage\n");
	printf("-h: print this help message\n");
	printf("-g: get value\n");
	printf("-s: set value\n");
	printf("-d: delete value\n");
	printf("-a: add value\n"
	       "\tcmscfg -an [ key ] -v [ value ] -f [ filename ]\n");
	printf("-f: file to add value, used with -a option\n");
	printf("-n: parameter name\n");
	printf("-v: parameter value\n");
	printf("-p: push command\n");
	printf("-o: pop command\n");
	printf("-w: write config file\n");
	printf("-u: dump config file\n"
	       "\tcmscfg -u [ absolute path]");
	printf("-G: get multiple parameter value\n");
	printf("-S: show variable name when -G is set\n");
	printf("-q: set multiple parameters' value\n"
	       "\tcmscfg -q [key1] [value1] [key2] [value2] .... [keyN] [valueN]\n");
}

int main(int argc,char *argv[])
{
	int ch;
	int cmsoption=0;
	int help=0;
	int i;
	char* c;
	char* key_to_send;
	char* value_to_send;
	char* value_to_recv;

	char** keys_to_send;	
	char** values_to_send;
	char** values_to_recv;
	char** keyValuePairs; /* even index: name, odd index: value */

	char* fileName;
	int err;
	int pr_cnt = 0;
	int num = 0;
	short isBlank;
	short showName = 0;
	int cnt = 0;
	int client;
        int r;
        struct sockaddr_in server;

	// init
	keys_to_send = values_to_send = values_to_recv  =NULL;
	keyValuePairs = NULL;
	key_to_send = value_to_send = value_to_recv = NULL;
	fileName = NULL;


	ch=getopt(argc,argv,optstring);
	if (ch==-1)
	{
	 printUsage();
	 return 0;
	}
	do 
	{
		switch (ch)
		{
			case'h':
				help=1;
				break;
			case's':
				cmsoption=CMSCFG_SETVALUE;
				break;
			case 'S':
				showName=1;
				break;
			case'g':
				cmsoption=CMSCFG_GETVALUE;
				break;
			case'd':
				cmsoption=CMSCFG_DELVALUE;
				break;
			case'a':
				cmsoption=CMSCFG_ADDVALUE;
				break;
			case'n':
				key_to_send = strdup(optarg);
				if(!key_to_send) handle_error("no memory");
				break;
			case'v':
				value_to_send = strdup(optarg);
				if(!value_to_send) handle_error("no memory");
				break;
			case'f':
				fileName = strdup(optarg);
				if(!fileName) handle_error("no memory");
				break;
			case'w':
				cmsoption=CMSCFG_FLUSH;
				break;
			case'p':
				cmsoption=CMSCFG_PUSH;
				break;
			case'o':
				cmsoption=CMSCFG_POP;
				break;
			case'u':
				cmsoption=CMSCFG_DUMPFILE;
				fileName = strdup(optarg);
				if(!fileName) handle_error("no memory");
				break;
			case 'G':
				cmsoption=CMSCFG_GETMULTIVALUE;
				optind--;
                		for( ;optind < argc && *argv[optind] != '-'; optind++)
				{
					if(cnt == num){
						if(num == 0){
							keys_to_send = realloc(keys_to_send,sizeof(char*));
							num = 1;
						}
						else{
							num<<=1;
							keys_to_send = realloc(keys_to_send,sizeof(char*)*num);
						}
						if(!keys_to_send){
							/* memory leak 
							 * when realloc fails, lose track of 
							 * all pointers to allocated memory in array element
							 */
							handle_error("no memory");
						}
					}

					isBlank = 1;

					for(c = argv[optind] ; *c ; c++)
					{
						if(*c != ' ')isBlank = 0;
					}
					if( isBlank ) continue;
					keys_to_send[cnt] = argv[optind];
					
					cnt++;
				}
                		break;
			case 'q':
                                cmsoption=CMSCFG_SETMULTIVALUE;
                                optind--;
                                for( ;optind < argc && *argv[optind] != '-'; optind++)
                                {
                                        if(pr_cnt == num){
                                                if(num == 0){
                                                        keyValuePairs = realloc(keyValuePairs,sizeof(char*));
                                                        num = 1;
                                                }
                                                else{
                                                        num<<=1;
                                                        keyValuePairs = realloc(keyValuePairs,sizeof(char*)*num);
                                                }
                                                if(!keyValuePairs){
							/* memory leak 
							 * when realloc fails, lose track of 
							 * all pointers to allocated memory in array element
							 */
                                                        handle_error("no memory");
                                                }
                                        }

                                        isBlank = 1;

                                        for(c = argv[optind] ; *c ; c++)
                                        {
                                                if(*c != ' ')isBlank = 0;
                                        }
                                        if( isBlank ) continue;

                                        keyValuePairs[pr_cnt] = argv[optind];
					pr_cnt++;
                                }
                                break;			

			default:
			  break;
		}
	}while ((ch=getopt(argc,argv,optstring))!=-1);

	if(help){
		printUsage();
		return 0;
	}
	/* make connection */
        client = socket(AF_INET,SOCK_STREAM,0);
        if(client == -1)
        {
                perror("could not create socket");
                return 1;
        }
        puts("Socket created");
        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_family = AF_INET;
        server.sin_port = htons(CMS_PORT);

        if (connect(client,(struct sockaddr*)&server,sizeof(server))<0)
        {
                perror("connect failed.");
                return 1;
        }
        puts("Connected");


	if (cmsoption==CMSCFG_GETVALUE)
	{
		if(!key_to_send){
			printf("The paramter is not specified\n");
			goto leave;
		}
		err=CMSGetValue(client,key_to_send,&value_to_recv);
		if(err!= CMS_OK){
			printf("CMS_FAIL\n");
		}
		else{
			printf("[CMS]%s='%s'\n",key_to_send,value_to_recv);
		}
	}
	else if (cmsoption==CMSCFG_SETVALUE)
	{
		if(!key_to_send || !value_to_send){
			if(!key_to_send)printf("The name is not specified\n");
			if(!value_to_send)printf("The value is not specified\n");
			goto leave;
		}
		err=CMSSetValue(client,key_to_send,value_to_send);
		if(err!= CMS_OK){
			printf("CMS_FAIL\n");
		}
		else{
			printf("Set param %s value '%s'\n",key_to_send,value_to_send);
		}
	}
	else if(cmsoption== CMSCFG_GETMULTIVALUE )
	{
		if(cnt == 0 ) goto leave;
		err = CMSGetMultiValue(client,keys_to_send,&values_to_recv,cnt);

		if( err!= CMS_OK){
			printf("CMS_FAIL\n");
		}
		else
		for(i=0; i<cnt; i++)
		{
			if(showName) printf("%s=",keys_to_send[i]);
			printf("'%s'\n",values_to_recv[i]);
		}
	}
	else if(cmsoption==CMSCFG_SETMULTIVALUE )
        {
                if(pr_cnt == 0 ) goto leave;
		else if(pr_cnt &1){
			printf("the number of parameters are not even in multi-set\n");
			goto leave;
		}
		cnt = pr_cnt / 2;
		keys_to_send  = malloc( sizeof(char*) *cnt);
		values_to_send= malloc( sizeof(char*)* cnt);

		if(!keys_to_send || ! values_to_send){
			if(keys_to_send) free(keys_to_send);
			if(values_to_send) free(values_to_send);
			printf("CMS_FAIL\n");
			goto leave;
		}			

		for(i=0;i<cnt;i++){
			keys_to_send[i] =keyValuePairs[2*i];
			values_to_send[i] =keyValuePairs[2*i+1];
			
		}
                err = CMSSetMultiValue(client,keys_to_send,values_to_send,cnt);
		if(err != CMS_OK){
				printf("CMS_FAIL\n");
		}
		else
			printf("CMS_OK\n");
        }
	else if (cmsoption==CMSCFG_DELVALUE)
	{
		if(!key_to_send){
			printf("The name is not specified\n");
			goto leave;
		}
		err=CMSDelValue(client,key_to_send);
		if(err!= CMS_OK )
			printf("CMS_FAIL\n");
		else
		printf("[CMS]successfully delete %s\n",key_to_send);
	}
	else if (cmsoption==CMSCFG_ADDVALUE)
	{
		if(!key_to_send || !value_to_send || !fileName){
			if(!key_to_send)  printf("The parameter name is not specified\n");
			if(!value_to_send)printf("The parameter value is not specified\n");
			if(!fileName)     printf("The file name is not specified\n");
			goto leave;
		}
		err=CMSAddParam(client,fileName,key_to_send,value_to_send);
		if(err != CMS_OK){
			printf("[CMS]:Add param %s value %s in file %s ( FAIL )\n",key_to_send,value_to_send,fileName);
		}
		else
		printf("[CMS]:Add param %s value %s in file %s ( OK )\n",key_to_send,value_to_send,fileName);
	}
	else if (cmsoption==CMSCFG_FLUSH)
	{
		err=CMSWriteParam(client);
		if(err != CMS_OK){
			puts("[CMS] flush all parameters from memory to file now ( FAIL )");
		}
		else
			puts("[CMS] flush all parameters from memory to file now ( OK )");
	}
	else if (cmsoption==CMSCFG_PUSH)
	{
		if(!key_to_send || !value_to_send){
			if(!key_to_send)printf("parameter name is not specified\n");
			if(!value_to_send)printf("parameter value is not specified\n");
			goto leave;
		}
		err=CMSPush(client,key_to_send,value_to_send);
		
		if(err != CMS_OK){
			printf("[CMS]Push key=%s, value=%s ( FAIL )\n", key_to_send,value_to_send);
		}
		else
			printf("[CMS]Push key=%s, value=%s ( OK )\n", key_to_send,value_to_send);
	}
	else if (cmsoption==CMSCFG_POP)
	{
		if(!key_to_send){
			if(!key_to_send)printf("parameter name is not specified\n");
			goto leave;
		}
		err= CMSPop(client,key_to_send,&value_to_recv);
		if(err!=CMS_OK){
			printf("[CMS]Pop command %s ( FAIL )\n",key_to_send);
		}
		else{
			printf("[CMS]Pop command '%s'='%s'\n",key_to_send,value_to_recv);
		}
	}
	else if (cmsoption==CMSCFG_DUMPFILE)
	{
		if(!fileName){     printf("fileName is not specified\n");
			goto leave;
		}
		err=CMSDumpFile(client,fileName);
		if(err!=CMS_OK){
			printf("[CMS]: Dump File to %s ( FAIL )\n",fileName);
		}
		else
			printf("[CMS]:Dump File to %s\n",fileName);
	}
	else if ( !help)
		printf("undefined options\n");

leave:
	close(client);
	if(key_to_send) free(key_to_send);
	if(value_to_send) free(value_to_send);
	if(value_to_recv) free(value_to_recv);
	if(fileName) free(fileName);
	if(keys_to_send) free(keys_to_send);
	if(values_to_send) free(values_to_send);
	if(keyValuePairs) free(keyValuePairs);
	if(values_to_recv){
		for(i=0;i<cnt;i++){
			free(values_to_recv[i]);
		}
		free(values_to_recv);
	}
	
	return 0;
}
