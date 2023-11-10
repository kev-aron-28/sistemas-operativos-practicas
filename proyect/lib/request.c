
#include <stdio.h>
#include <string.h>
#include "request.h"

char * getRequestPathFrom(REQUEST *request){
  return request->requestPath;
}

METHODS getMethodFrom(REQUEST *request) {
  return request->method;
}
