#include <stdio.h>
#include "response.h"

STATUS getStatus (RESPONSE * response) {
  return response->statusCode;
}

int hasBody(RESPONSE * response){
  return response->hasBody;
}

void setStatus(RESPONSE * response, STATUS status) {
  response->statusCode = status;
}

void setBody (RESPONSE *response, int flag) {
  response->hasBody = flag;
}
