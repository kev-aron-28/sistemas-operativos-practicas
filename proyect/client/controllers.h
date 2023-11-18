#ifndef CONTROLLERS_H
#define CONTROLLERS_H
#include "../lib/entities.h"

User registerUserController();
Login loginUserController();
Auction createAuctionController(User);
Bid bidToAuctionController(User);
#endif
