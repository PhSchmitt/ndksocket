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

#include <errno.h>

#define DEBUG_TAG "NDK_AndroidNDK1SampleActivity"

//initialize with negative value => socket inexistent
int sockfd = -1;

void error(const char *msg)
{
    __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "NDK:LC:ERR: [%s], Errno: %i", msg, errno);
}

void debug(const char *msg)
{
    __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "NDK:LC:DBG: [%s]", msg);
}

int createSocketIfNotExists ()
{
    if (sockfd >= 0)
    {
        debug("Socket already exists");
        return 1;
    }
    else
    {
        debug("Opening Socket");
        sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd < 0)
        {
            error("ERROR opening socket");
            return -1;
        }
    return 2;
    }
}

int setOrChangeSockOpt (int flagname, int socketOptionEnabled)
{
    int sockopterr = setsockopt(sockfd, SOL_SOCKET, flagname, (void *)&socketOptionEnabled, sizeof(socketOptionEnabled));
    if (sockopterr < 0)
    {
        error("ERROR setting sockopt");
        return -2;
    }
}


int Java_de_unikl_cs_disco_ndk1_AndroidNDK1SampleActivity_openConnection(JNIEnv * env, jobject this,
    jstring jurl, int portno)
{
    jboolean isCopy;
    const char * url = (*env)->GetStringUTFChars(env, jurl, &isCopy);
    struct hostent *server;
    struct sockaddr_in serv_addr;

    int socketStatus = createSocketIfNotExists();
    if ( socketStatus < 0)
    {
        return -1;
    }
    if (1 == socketStatus)
    {
        return 0;
    }

    server = gethostbyname(url);
    if (server == NULL)
    {
        error("ERROR Host not found");
        return -3;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
        return -4;
    }
    (*env)->ReleaseStringUTFChars(env, jurl, url);
    return 0;
}

int Java_de_unikl_cs_disco_ndk1_AndroidNDK1SampleActivity_sendNative(JNIEnv * env, jobject this,
    jstring jdata, jboolean jSetUrgentFlag)
{
    jboolean isCopy;
    const char * data = (*env)->GetStringUTFChars(env, jdata, &isCopy);
    const int flagname = SO_OOBINLINE;
    int socketOptionEnabled;

    if (jSetUrgentFlag)
    {
        socketOptionEnabled = 1;
    }
    else
    {
        socketOptionEnabled = 0;
    }

    if (setOrChangeSockOpt(flagname, socketOptionEnabled) < 0)
    {
       return -2;
    }

    if (write(sockfd,data,strlen(data)) < 0)
    {
        error("ERROR writing to socket");
    }

    (*env)->ReleaseStringUTFChars(env, jdata, data);
    return 0;
}

int Java_de_unikl_cs_disco_ndk1_AndroidNDK1SampleActivity_closeConnection(JNIEnv * env, jobject this)
{
    if (sockfd < 0)
    {
        error("No socket found");
        return -5;
    }
    close(sockfd);
    return 0;
}



