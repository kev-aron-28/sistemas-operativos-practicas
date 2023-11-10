#ifndef REQUEST_H
#define REQUEST_H

#define registerRoute "/register"

enum methods {
  GET,
  PUT,
  DELETE,
  POST
};

typedef enum methods METHODS;
typedef struct request {
  char requestPath[256];
  METHODS method;
} REQUEST;

char * getRequestPathFrom(REQUEST*);
METHODS getMethodFrom(REQUEST*);

void setRequestPath(char *);
void setMethod(METHODS);
#endif
