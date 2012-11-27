#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <values.h>
#include <time.h>
#include <glob.h>

const char *BASE_DIR = "/tmp/clicko";
const char *BASE_URL = "http://woozle.org/l.cgi";


#define POST_MAX 1024

/*
 * CGI
 */
static int is_cgi  = 0;
static char **argv = NULL;

static int
read_char_argv()
{
  static int   arg = 0;
  static char *p;

  if (NULL == argv) {
    return EOF;
  }

  if (0 == arg) {
    arg = 1;
    p = argv[1];
  }

  if (! p) {
    return EOF;
  } else if (! *p) {
    arg += 1;
    p = argv[arg];
    return '&';
  }

  return *(p++);
}

static int
read_char_stdin()
{
  static int inlen = -1;

  if (-1 == inlen) {
    char *p = getenv("CONTENT_LENGTH");
    if (p) {
      inlen = atoi(p);
      if (inlen > POST_MAX) {
        inlen = POST_MAX;
      }
      if (inlen < 0) {
        inlen = 0;
      }
    } else {
      inlen = 0;
    }
  }

  if (inlen) {
    inlen -= 1;
    return getchar();
  }
  return EOF;
}

static int
read_char_query_string()
{
  static char *p = (char *)-1;

  if ((char *)-1 == p) {
    p = getenv("QUERY_STRING");
  }

  if (! p) {
    return EOF;
  } else if (! *p) {
    return EOF;
  } else {
    return *(p++);
  }
}

static int (* read_char)() = read_char_argv;

int
cgi_init(char *global_argv[])
{
  char *rm = getenv("REQUEST_METHOD");

  if (! rm) {
    read_char = read_char_argv;
    argv = global_argv;
  } else if (0 == strcmp(rm, "POST")) {
    read_char = read_char_stdin;
    is_cgi = 1;
  } else if (0 == strcmp(rm, "GET")) {
    read_char = read_char_query_string;
    is_cgi = 1;
  } else {
    printf(("405 Method not allowed\r\n"
            "Allow: GET, POST\r\n"
            "Content-type: text/plain\r\n"
            "\r\n"
            "%s is not allowed.\n"),
           rm);
    return -1;
  }

  return 0;
}

static char
tonum(int c)
{
  if ((c >= '0') && (c <= '9')) {
    return c - '0';
  }
  if ((c >= 'a') && (c <= 'f')) {
    return 10 + c - 'a';
  }
  if ((c >= 'A') && (c <= 'F')) {
    return 10 + c - 'A';
  }
  return 0;
}

static char
read_hex()
{
  int a = read_char();
  int b = read_char();

  return tonum(a)*16 + tonum(b);
}

/* Read a key or a value.  Since & and = aren't supposed to appear
   outside of boundaries, we can use the same function for both.
*/
size_t
cgi_item(char *str, size_t maxlen)
{
  int    c;
  size_t pos = 0;

  while (1) {
    c = read_char();
    switch (c) {
      case EOF:
      case '=':
      case '&':
        str[pos] = '\0';
        return pos;
      case '%':
        c = read_hex();
        break;
      case '+':
        c = ' ';
        break;
    }
    if (pos < maxlen - 1) {
      str[pos] = c;
      pos += 1;
    }
  }
}

void
cgi_head(char *title)
{
  if (is_cgi) {
    printf("Content-type: text/html\r\n\r\n");
  }
  printf(("<!DOCTYPE html>\n"
          "<html>\n"
          "  <head><title>%s</title></head>\n"
          "  <body>\n"
          "    <h1>%s</h1>\n"),
         title, title);
}

void
cgi_foot()
{
  printf("\n"
         "  </body>\n"
         "</html>\n");
}

void
cgi_result(int code, char *desc, char *fmt, ...)
{
  va_list ap;

  if (is_cgi) {
    printf("%d %s\r\n", code, desc);
  }
  cgi_head(desc);
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
  cgi_foot();
  exit(0);
}

void
cgi_page(char *title, char *fmt, ...)
{
  va_list  ap;

  cgi_head(title);
  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);
  cgi_foot();
  exit(0);
}

void
cgi_error(char *text)
{
  cgi_result(500, "Internal error", "<p>%s</p>", text);
}


static FILE *
open_file(char *shorty, char *mode)
{
    char fn[256];
    FILE *f;

    snprintf(fn, sizeof fn, "%s/%s.url", BASE_DIR, shorty);

    f = fopen(fn, mode);
    if (! f) {
        cgi_error("Unable to open database");
    }

    return f;
}

void
shorten_url()
{
    char u[4096];
    size_t ulen;
    char shorty[16];

    ulen = cgi_item(u, sizeof u);
    if (ulen == 0) {
        cgi_error("No URL specified");
    }

    snprintf(shorty, sizeof shorty, "%08x.%04x", (unsigned int)time(NULL), getpid());

    /* Put the URL into a file */
    {
        FILE *f = open_file(shorty, "w");
        fprintf(f, "%s", u);
        fclose(f);
    }

    /* Report back */
    printf("Content-type: text/plain\r\n\r\n");
    printf("%s/%s\n", BASE_URL, shorty);
}


void
redirect(char *pi)
{
    int i;

    if (! pi) {
        cgi_error("No short URL provided");
    }

    if (pi[0] != '/') {
        cgi_error("Invalid PATH_INFO");
    }
    pi += 1;

    for (i = 0; pi[i]; i += 1) {
        if ((! isalnum(pi[i])) &&
            (pi[i] != '.')) {
            cgi_error("Bad short URL");
        }
    }

    /* Open file */
    {
        char u[4096];
        FILE *f = open_file(pi, "r");

        fgets(u, sizeof u, f);
        fclose(f);

        printf("Location: %s\r\n", u);
    }
}


void
list_links()
{
    char g[256];
    glob_t globbuf;
    int i;

    snprintf(g, sizeof g, "%s/*.url", BASE_DIR);

    glob(g, 0, NULL, &globbuf);

    cgi_head("Clicko history");
    printf("<ul>\n");
    for (i = globbuf.gl_pathc - 1; i >= 0; i -= 1) {
        FILE *f = fopen(globbuf.gl_pathv[i], "r");
        char url[4096];

        fgets(url, sizeof url, f);
        fclose(f);

        printf("<li><a href=\"%s\">%s</a></li>\n", url, url);
    }
    printf("</ul>\n");
    cgi_foot();
}

int
main(int argc, char *argv[])
{
    if (-1 == cgi_init(argv)) {
        fprintf(stderr, "Unable to initialize CGI.\n");
        return -1;
    }

    while (1) {
        char key[12];
        size_t klen;

        klen = cgi_item(key, sizeof key);
        if (klen == 0) {
            break;
        }
        switch (key[0]) {
            case 'u':
                shorten_url();
                return 0;
        }
    }

    {
        char *pi = getenv("PATH_INFO");

        if (pi) {
            redirect(pi);
        }
    }

    list_links();

    return 0;
}
