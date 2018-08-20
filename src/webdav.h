#ifndef __WEBDAV_H__
#define __WEBDAV_H__

using namespace std;

void webdavDownloadFile(string url, string file);
void webdavUploadFile(string url, string file);
void webdavCreateDir(string url);
int webdavMove(string url_old, string url_new);
int webdavTest(string url);
string userpwd();
#endif
