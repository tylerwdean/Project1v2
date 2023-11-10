#pragma once
#include <string>
#include "User.h"

class Customer : public User {
public:

    Customer(int userID);

    void mainMenu();
    //lists all their orders and the cost of them 
    void checkOrderHistory();
    //shows the rewards balance
    void checkPointsBalance();
    //shows and lets choose to update contact information
    void updateInformation();
    //search for products by name
    void searchForProducts();
    //adds the product to the cart
    void addToCart(int productID, int quantity);
    //views whats in the cart and subtotal
    void viewCart();

    void changePassword();
};