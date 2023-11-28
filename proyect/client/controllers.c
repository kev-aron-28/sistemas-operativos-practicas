#include <stdio.h>
#include <string.h>
#include "../lib/lib.h"
#include "controllers.h"
#include "../lib/entities.h"
#include "../lib/response.h"

User registerUserController()
{
  User user;
  printf("NOMBRE: ");
  scanf("%s", user.name);

  printf("EMAIL: ");
  scanf("%s", user.email);

  printf("CONTRASENA: ");
  scanf("%s", user.password);

  return user;
}

Login loginUserController() {
  Login login;

  printf("EMAIL: ");
  scanf("%s", login.email);

  printf("CONTRASENA: ");
  scanf("%s", login.password);

  return login;
}

Auction createAuctionController(User owner) {
  Auction auction;
  getchar();
  fflush(stdin);
  printf("NOMBRE DEL PRODUCTO: ");
  getchar();
  fflush(stdin);
  scanf("%[^\n]", auction.title);
  getchar();
  fflush(stdin);

  printf("PRECIO INICIAL: ");
  scanf("%f", &auction.actualBid);

  auction.isActive = 1;
  strcpy(auction.ownerId, owner.id);
  strcpy(auction.sellerName, owner.name);

  return auction;
}

Bid bidToAuctionController (User bidder) {
  Bid bid;
  printf("ID SUBASTA: ");
  scanf("%s", bid.auctionId);
  printf("\n");
  printf("PUJA: ");
  scanf("%f", &bid.bid);

  strcpy(bid.bidderId, bidder.id);
  return bid;
}

EndAuctionById endAuctionByIdController (User owner) {
  EndAuctionById auctionToEnd;
  printf("ID SUBASTA: ");
  scanf("%s", auctionToEnd.auctionId);
  strcpy(auctionToEnd.userId, owner.id);
  return auctionToEnd;

}
