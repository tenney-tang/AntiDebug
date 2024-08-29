//#include "AntiDebug.h"
#include "Log.h"
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <pthread.h>
pthread_t t_id;
#define CHECK_TIME 1
void be_attached_check()
{
	const int bufsize = 1024;
    char filename[bufsize];
    char line[bufsize];
    int pid = getpid();
    sprintf(filename, "/proc/%d/status", pid);
    while (1) {
        FILE* fd = fopen(filename, "r");
        if (fd != nullptr){
          while (fgets(line, bufsize, fd)){
            if (strncmp(line, "TracerPid", 9) == 0){
              int statue = atoi(&line[10]);
              if (statue != 0){
                fclose(fd);
                kill(0, SIGKILL);
              }
              break;
            }
          }
          fclose(fd);
        } else{
          LOG_PRINT_E("open %s fail...", filename);
        }
        sleep(CHECK_TIME);
    }

}

void* anti_ptrace(void* arg) {
    try{
        be_attached_check();
    }catch (std::exception& e) {
        LOG_PRINT_E("Exception occur......");
    }
    // 实现anti_ptrace函数的具体逻辑
    return NULL;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_android_antidebug_AntiDebug_checkDebug(JNIEnv *env, jclass clazz) {
    if (pthread_create(&t_id, NULL, anti_ptrace, NULL) != 0) {
        LOG_PRINT_D("checkDebug tjl>>");
        exit(EXIT_FAILURE); // 退出程序
    }
}