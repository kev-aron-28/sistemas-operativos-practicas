#include "../lib/entities.h"
#include "../lib/response.h"
#ifndef CONTROLLERS_H
#define CONTROLLERS_H

STATUS registerUserController();
STATUS loginUserController();
STATUS createAuctionController();
STATUS getAllAuctionsController();
STATUS bidToAuctionController();

#endif
