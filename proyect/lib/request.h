#ifndef REQUEST_H
#define REQUEST_H

// Auth
#define registerRoute "/register"
#define loginRoute "/login"

// Auction
#define createAuction "/auction/create"
#define getAllAuctionsRoute "/auction/all"
#define bidToAuction "/auction/:id"
#define getMyAuctions "/auction/myAuctions"
#define endAuction "/auction/:id/end"
#define wonAuction "/auction/won"

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
#endif
