#ifndef __CMC_H__
#define __CMC_H__

#define GetValueUser 1   //get param value
#define SetValueUser 2   //set param value
#define GetValueISP  3   //get param value
#define SetValueISP  4   //set param value
#define SetValueBoth 5
#define DelValueUser 9    //del param value
#define DelValueISP  10   //del param value
#define DelValueBoth 11
#define AddValueUser 12    //del param value
#define AddValueISP  13   //del param value
#define AddValueBoth 14
#define AddValueTemp 15
#define WriteParamNow 16
#define ReplaceValueUser 17
#define ReplaceValueISP 18
#define ReplaceValueBoth 19
#define RespOK       6   //OK
#define RespFAIL     7   //FAIL
#define NoOP         8   //NoOP
 
#define PushCMCmd 33
#define PopCMCmd 34
#define SetMultiValue 35
#define GetMultiValue 36

#ifdef __cplusplus 
extern "C" { 
#endif

int CMSGetValue(int client,char *key_to_send,char **value_to_recv);
int CMSSetValue(int client, char *ParamName,char *value);
int CMSAddParam(int client, char* fileName,char *ParamName,char *value);
int CMSDelParam(int client,char *ParamName);
int CMSWriteParam(int client);
int CMSSetMultiValue(int client, char **  keys_to_send, char **  values_to_send, int number);
int CMSGetMultiValue(int client, char **  keys_to_send, char *** values_to_recv, int number);
int CMSSetMultiValuef(int  client, int number, ...);
int CMSGetMultiValuef(int client, char*** values_to_recv,int number, ...);
int CMSDumpFile(int client, char *FileName);
int CMSPush(int client, char *ParamName,char *value);
int CMSPop(int client,char *key_to_send,char **value_to_recv);
#ifdef __cplusplus 
}
#endif


#endif
