#ifndef RESPONSE_H
#define RESPONSE_H

enum statusCodes {
  BAD_REQUEST,
  OK,
  CREATED,
  NOT_FOUND,
  INTERNAL_ERROR,
  UNAUTHORIZED
};

typedef enum statusCodes STATUS;

typedef struct response {
  STATUS statusCode;
  int hasBody;
} RESPONSE;

#endif
