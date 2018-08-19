# vita-curl_webdavtest
test functionality of libcurl to work with webdav (own/nextcloud)

# Adding credentials
create a file called "creds.h" into your ```src/``` directory with the following contents:
```c
#define DAV_DOMAIN "domain.name"
#define DAV_USER "your_user"
#define DAV_PASSWORD "y0urP4ssW0rD"
```
