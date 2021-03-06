#define VITASDK

#include <psp2/io/stat.h> 
#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/display.h>

#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>

#include <psp2/io/fcntl.h>

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <string>

#include "debugScreen.h"
#include "creds.h"

#include <curl/curl.h>
#include "webdav.h"

using namespace std;

void netInit() {
//  psvDebugScreenPrintf("Loading module SCE_SYSMODULE_NET\n");
    sceSysmoduleLoadModule(SCE_SYSMODULE_NET);

//  psvDebugScreenPrintf("Running sceNetInit\n");
    SceNetInitParam netInitParam;
    int size = 4*1024*1024;
    netInitParam.memory = malloc(size);
    netInitParam.size = size;
    netInitParam.flags = 0;
    sceNetInit(&netInitParam);

//  psvDebugScreenPrintf("Running sceNetCtlInit\n");
    sceNetCtlInit();
}

void netTerm() {
//  psvDebugScreenPrintf("Running sceNetCtlTerm\n");
    sceNetCtlTerm();

//  psvDebugScreenPrintf("Running sceNetTerm\n");
    sceNetTerm();

//  psvDebugScreenPrintf("Unloading module SCE_SYSMODULE_NET\n");
    sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);
}

void httpInit() {
//  psvDebugScreenPrintf("Loading module SCE_SYSMODULE_HTTP\n");
    sceSysmoduleLoadModule(SCE_SYSMODULE_HTTP);

//  psvDebugScreenPrintf("Running sceHttpInit\n");
    sceHttpInit(4*1024*1024);
}

void httpTerm() {
//  psvDebugScreenPrintf("Running sceHttpTerm\n");
    sceHttpTerm();

//  psvDebugScreenPrintf("Unloading module SCE_SYSMODULE_HTTP\n");
    sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTP);
}


int main(int argc, char *argv[]) {
    psvDebugScreenInit();
   psvDebugScreenPrintf("webdav sample v.1.0 by phils3r\n\n");
    psvDebugScreenPrintf("Webdav domain: \"%s\"\n", DAV_DOMAIN);
    psvDebugScreenPrintf("Webdav username: \"%s\"\n", DAV_USER);

    netInit();
    httpInit();

    struct SceIoStat * dirStat = (SceIoStat*)malloc(sizeof(SceIoStat));
    if(sceIoGetstat("ux0:data/curltest" , dirStat) < 0){
        sceIoMkdir("ux0:data/curltest" , 0777);
}
//  psvDebugScreenPrintf("Download memory card id.dat file");
    string domain = DAV_DOMAIN;

    psvDebugScreenPrintf("Download test...\n");
    webdavDownloadFile("https://cloud.phils3r.de/remote.php/dav/files/philipp/reisen.txt",
    "ux0:data/curltest/reisen.txt");
    psvDebugScreenPrintf("Upload test...\n");
    webdavUploadFile("https://cloud.phils3r.de/remote.php/dav/files/philipp/test.txt",
    "ux0:data/curltest/test.txt");
    psvDebugScreenPrintf("Create directory test...\n");
    webdavCreateDir("https://cloud.phils3r.de/remote.php/dav/files/philipp/testdir");
    webdavCreateDir("https://cloud.phils3r.de/remote.php/dav/files/philipp/testdir_tmp");
    psvDebugScreenPrintf("file move test...\n");

        psvDebugScreenPrintf("\nReturn code of move test: %d\n", 
        webdavMove("https://cloud.phils3r.de/remote.php/dav/files/philipp/testdir_tmp",
        "https://cloud.phils3r.de/remote.php/dav/files/philipp/testdir_tmp_new"));

    psvDebugScreenPrintf("File test success...\n");
        psvDebugScreenPrintf("\nReturn code of successfull test: %d\n", 
        webdavTest("https://cloud.phils3r.de/remote.php/dav/files/philipp/testdir"));
    if (webdavTest("https://cloud.phils3r.de/remote.php/dav/files/philipp/testdir") == 404) {
        psvDebugScreenPrintf("File \"testdir\" nicht da\n");
    }
    psvDebugScreenPrintf("File test failed...\n");
    if (webdavTest("https://cloud.phils3r.de/remote.php/dav/files/philipp/test123_file") == 404) {
        psvDebugScreenPrintf("File \"test123_file\" nicht da\n");
    }

    httpTerm();
    netTerm();

    psvDebugScreenPrintf("This app will close in 20 seconds!\n");
    sceKernelDelayThread(20*1000*1000);

    sceKernelExitProcess(0);
    return 0;
}
