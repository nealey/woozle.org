#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const char *basepath = "/usr/share/IlohaMail/source";

int
main(int argc, char *argv[])
{
    char *pathinfo = getenv("PATH_INFO");
    char filename[512];

    if ((! pathinfo) || (0 == strcmp(pathinfo, "/index.html"))) {
        pathinfo = "/index.php";
    }
    snprintf(filename, sizeof filename, "%s%s", basepath, pathinfo);
    setenv("SCRIPT_FILENAME", filename, 1);
    execl("/usr/bin/php-cgi", filename, NULL);

    return 0;
}
