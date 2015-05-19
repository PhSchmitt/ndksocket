#include <jni.h>
#include <string.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define DEBUG_TAG "NDK_AndroidNDK1SampleActivity"

void Java_de_unikl_cs_disco_ndk1_AndroidNDK1SampleActivity_helloLog(JNIEnv * env, jobject this, jstring logThis)
{
    jboolean isCopy;
    const char * szLogThis = (*env)->GetStringUTFChars(env, logThis, &isCopy);

    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK:LC: [%s]", szLogThis);

    (*env)->ReleaseStringUTFChars(env, logThis, szLogThis);
}

void error(const char *msg)
{
    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK:LC: [%s]", msg);
   // exit(0);
}


int Java_de_unikl_cs_disco_ndk1_AndroidNDK1SampleActivity_sendUrgent(JNIEnv * env, jobject this,
        jstring jurl, int portno, jstring jdata, jboolean jSetUrgentFlag)
{
    jboolean isCopy;
    const char * url = (*env)->GetStringUTFChars(env, jurl, &isCopy);
    const char * data = (*env)->GetStringUTFChars(env, jdata, &isCopy);

    int sockfd , n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (jSetUrgentFlag)
    {
    //TODO set Flag
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else
    {
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(url);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    n = write(sockfd,data,strlen(data));
    if (n < 0)
         error("ERROR writing to socket");
    error("line 74");
    bzero(buffer,256);
    error("line 75");
    n = read(sockfd,buffer,255);
    error("line 77");
    if (n < 0)
         error("ERROR reading from socket");
    error("line 80");
    printf("%s\n",buffer);
    error("line 88");
    close(sockfd);

    (*env)->ReleaseStringUTFChars(env, jurl, url);
    (*env)->ReleaseStringUTFChars(env, jdata, data);
    return 1;
}




