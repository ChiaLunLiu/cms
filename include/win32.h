#ifdef WIN32

#include <winsock2.h>
#include <CMType.h>

#define FAIL    0
#define SUCCESS 1
#define bool int
#define true 1
#define false 0

#define uint16_t unsigned short
#define int64_t long int
#define uint32_t unsigned int
#define int32_t int


#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)
#define CONFIG_FILE_NAME "CM.conf"

#ifndef snprintf
#define snprintf sprintf_s
#endif

#ifndef srandom
#define srandom srand
#endif

#ifndef random
#define random rand
#endif

#ifndef close
#define close closesocket
#endif

#ifndef getpid
#define getpid _getpid
#endif

// struct timeval is defined in winsock.h
#ifdef __cplusplus
extern "C" {
#endif
int gettimeofday(struct timeval *tv/*in*/, struct timezone *tz/*in*/);
char *GetConfigFilePath();
int walkConfFile(const char *file, const CM_PCHAR buffer, int len);
int ftw(const char *path, int(*func)(), int depth);
void writeParamToFile();
int _getpid( void );
int CMSInitFinished();
int initParamEventReg();
int initFileEventReg();
void sync();
#ifdef __cplusplus
}
#endif
#endif 
