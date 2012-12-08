#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const char *baseurl = "https://woozle.org/mail.cgi/";
const char *basepath = "/opt/roundcubemail";

int
main(int argc, char *argv[])
{
    char *pathinfo = getenv("PATH_INFO");
    char *remaddr = getenv("REMOTE_ADDR");
    size_t pathlen = pathinfo?strlen(pathinfo):0;
    char filename[512];
    char *ext;

    if ((! pathinfo) || (! remaddr) || (! getenv("HTTPS"))) {
        printf("Location: %s\n", baseurl);
        return 0;
    } else if (0 == strcmp(pathinfo, "/index.html")) {
        snprintf(filename, sizeof filename, "%s/index.php", basepath);
    } else if (pathinfo[pathlen-1] == '/') {
        snprintf(filename, sizeof filename, "%s%sindex.php", basepath, pathinfo);
    } else {
        snprintf(filename, sizeof filename, "%s%s", basepath, pathinfo);
    }

    ext = strrchr(filename, '.');
    if (! ext) {
        ext = "";
    }

    if (0 == strcmp(ext, ".php")) {
        setenv("SCRIPT_FILENAME", filename, 1);
        setenv("REDIRECT_STATUS", "fuck me", 1);
        execl("/usr/bin/php-cgi", filename, NULL);
    } else if (strstr(filename, "/config/") ||
               strstr(filename, "/logs/") ||
               strstr(filename, "/temp/")) {
        printf("Content-type: text/plain\n\n[MESSAGE REDACTED]\n");
    } else {
        FILE *f = fopen(filename, "r");
        char *ct = "application/octet-stream";

        if (0 == strcmp(ext, ".css")) {
            ct = "text/css";
        } else if (0 == strcmp(ext, ".html")) {
            ct = "text/html";
        } else if (0 == strcmp(ext, ".js")) {
            ct = "application/javascript";
        } else if (0 == strcmp(ext, ".png")) {
            ct = "image/png";
        } else if (0 == strcmp(ext, ".jpg")) {
            ct = "image/jpeg";
        } else if (0 == strcmp(ext, ".gif")) {
            ct = "image/gif";
        }

        printf("Content-type: %s\n\n", ct);

        while (! feof(f)) {
            char buf[4096];
            size_t len;

            
            len = fread(buf, 1, sizeof buf, f);
            if (len) {
                fwrite(buf, 1, len, stdout);
            }
        }
    }

    return 0;
}
