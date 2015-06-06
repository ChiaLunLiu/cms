#ifndef __CMC2_H__
#define __CMC2_H__

void cmc_print_error(int err);
int CMSGetValue2(void* client,char *key,char **value,int OPID);
int  CMSSetValue2(void* client, char *key,char *value,int OPID);

int CMSSetMultiValue2(void*client, char **  keys, char **  values, int number,int OPID);
int CMSSetMultiValuef2(void*client, int OPID, int i, ...);

int CMSGetMultiValue2(void*client, char **  keys, char *** values, int number,int OPID);
int CMSGetMultiValuef2(void* client, int OPID, char*** values,int i, ...);
int CMSDumpFile2(void*client, char *filename,int OPID);

int CMSDelValue2(void* client,char *key_to_send,int OPID);
int CMSAddParam2(void*client,char *key_to_send,char *fileName,char *value_to_send,int OPID);
int CMSWriteParam2(void* client);
#endif
