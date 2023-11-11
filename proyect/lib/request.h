#ifndef REQUEST_H
#define REQUEST_H

// Auth
#define registerRoute "/register"
#define loginRoute "/login"

// Auction
#define createAuction "/auction/create"

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
