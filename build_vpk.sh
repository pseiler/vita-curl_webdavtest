#!/bin/bash
NAME="vita-curl_webdavtest"
PUSH=0
PORT=1337

if [ "${1}" = "-p" ] && [ ! -z "${2}" ];
then
    PUSH=1;
    MY_IP="${2}";
elif [ "${1}" = "-p" ] && [ -z "${2}" ];
then
    echo "Please add the IP of your PS Vita as the second parameter";
    exit 1
fi;

test -d build && rm -rf build;
mkdir build && cd build;
cmake ..;
make
MAKE_RETURN=$?
if [ ${PUSH} -eq 1 ];
then
    if [ ${MAKE_RETURN} -eq 0 ];
    then
        curl -T "${NAME}.vpk" "ftp://${MY_IP}:${PORT}/ux0:/vpk/";
    else
        echo "\"make\" call failed. No upload.";
    fi;
fi;
cd -

exit 0;
