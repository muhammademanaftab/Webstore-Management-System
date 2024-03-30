#include <iostream>
#include <fstream> // For file input/output
#include <cstring> // For string operations

// Define the Product and Shopping structs
struct Product {
    char name[50];
    int price;
    int quantity;
};

struct Shopping {
    Product* products;
    int productsCount;
};

// Function to print possible operations
void printPossibleOperations() {
    std::cout << "Possible Operations:" << std::endl;
    std::cout << "1. Load Webstore From a Txt File" << std::endl;
    std::cout << "2. Write Webstore To a Txt File" << std::endl;
    std::cout << "3. Insert product to Cart" << std::endl;
    std::cout << "4. Print Total Price of Cart" << std::endl;
    std::cout << "5. Buy Products of Cart" << std::endl;
    std::cout << "6. Exit..." << std::endl;
}

// Function to check if a product is a member of the shopping list
int isMember(const char* name, const Shopping* myShopping) {
    for (int i = 0; i < myShopping->productsCount; ++i) {
        if (strcmp(name, myShopping->products[i].name) == 0) {
            return 1; // Product found
        }
    }
    return 0; // Product not found
}

// Function to insert an item into the shopping list
void insertItem(Shopping* myShopping, const Product& myProduct, int* totalCapacity) {
    // Check if the product is already inserted
    if (isMember(myProduct.name, myShopping)) {
        for (int i = 0; i < myShopping->productsCount; ++i) {
            if (strcmp(myProduct.name, myShopping->products[i].name) == 0) {
                myShopping->products[i].quantity += myProduct.quantity;
                return;
            }
        }
    }

    // Make space if necessary
    if (myShopping->productsCount == *totalCapacity) {
        *totalCapacity *= 2;
        Product* temp = new Product[*totalCapacity];
        std::copy(myShopping->products, myShopping->products + myShopping->productsCount, temp);
        delete[] myShopping->products;
        myShopping->products = temp;
    }

    // Insert the product
    strcpy(myShopping->products[myShopping->productsCount].name, myProduct.name);
    myShopping->products[myShopping->productsCount].price = myProduct.price;
    myShopping->products[myShopping->productsCount].quantity = myProduct.quantity;

    // Increase the total count of products in myShopping
    myShopping->productsCount++;
}

// Function to load the webstore from a file
void loadWebstore(Shopping* store, int* totalCapacityOfStore) {
    char nameOfInputFile[50];
    std::cout << "Enter the name of the file with .txt : ";
    std::cin >> nameOfInputFile;

    std::ifstream file(nameOfInputFile);
    
    if(file.is_open()) {
        Product myProduct;
        while(file >> myProduct.name >> myProduct.price >> myProduct.quantity) {
            insertItem(store, myProduct, totalCapacityOfStore);
        }
        
        std::cout << "Successfully loaded webshop store from the file " << nameOfInputFile << std::endl;
    } else {
        std::cout << "There was an error while opening the file " << nameOfInputFile << std::endl;
    }
    
    file.close();
}

// Function to write the webstore to a file
void writeWebStore(const Shopping* store) {
    char nameOfOutputFile[50];
    std::cout << "Enter the name of output file: including .txt: ";
    std::cin >> nameOfOutputFile;

    std::ofstream file(nameOfOutputFile);

    if (file.is_open()) {
        for (int i = 0; i < store->productsCount; i++) {
            file << store->products[i].name << " " << store->products[i].price << " " << store->products[i].quantity << std::endl;
        }
        std::cout << "Successfully written the content of the store to " << nameOfOutputFile << std::endl;
    } else {
        std::cout << "Failed to open " << nameOfOutputFile << std::endl;
    }

    file.close();
}

// Function to insert a product into the cart
void insertToCart(Shopping* store, Shopping* cart, int* totalCapacityOfCart) {
    Product myProduct;
    std::cout << "Enter the name and quantity of the product to be bought respectively: ";
    std::cin >> myProduct.name >> myProduct.quantity;

    // Check if the product is available in the store
    if (!isMember(myProduct.name, store)) {
        std::cout << "Sorry, the product is not available in the store." << std::endl;
        return;
    }

    // Get the price of the product
    for (int i = 0; i < store->productsCount; ++i) {
        if (strcmp(myProduct.name, store->products[i].name) == 0) {
            myProduct.price = store->products[i].price;
            break;
        }
    }

    // Insert the product into the cart
    insertItem(cart, myProduct, totalCapacityOfCart);
    std::cout << myProduct.quantity << " " << myProduct.name << "s successfully added to cart." << std::endl;
}

// Function to print the total price of the items in the cart
void printPrice(const Shopping* cart) {
    int price = 0;
    for (int i = 0; i < cart->productsCount; ++i) {
        price += (cart->products[i].price * cart->products[i].quantity);
    }

    std::cout << "Total price: " << price << std::endl;
}

// Function to buy products from the cart
void buyProducts(Shopping* store, Shopping* cart, int* totalCapacityOfCart) {
    for (int i = 0; i < cart->productsCount; ++i) {
        for (int j = 0; j < store->productsCount; ++j) {
            int result = strcmp(cart->products[i].name, store->products[j].name);
            if (result == 0) {
                if (cart->products[i].quantity <= store->products[j].quantity) {
                    std::cout << cart->products[i].quantity << " " << cart->products[i].name << "s bought successfully." << std::endl;
                    store->products[j].quantity -= cart->products[i].quantity;
                } else {
                    int diff = cart->products[i].quantity - store->products[j].quantity;
                    std::cout << diff << " " << store->products[j].name << "s are missing." << std::endl;
                }
            }
        }
    }
    // Reset the cart
    *totalCapacityOfCart = 1;
    cart->productsCount = 0;
    delete[] cart->products; // Free existing memory, if any
    cart->products = new Product[*totalCapacityOfCart];
}

int main() {
    int choice = 0;
    int totalCapacityOfStore = 1;
    int totalCapacityOfCart = 1;
    Shopping* store = new Shopping;
    store->productsCount = 0;
    store->products = new Product[totalCapacityOfStore];

    Shopping* cart = new Shopping;
    cart->productsCount = 0;
    cart->products = new Product[totalCapacityOfCart];

    do {
        printPossibleOperations();

        std::cout << "Enter the choice number (1-6): ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                loadWebstore(store, &totalCapacityOfStore);
                break;
            case 2:
                writeWebStore(store);
                break;
            case 3:
                insertToCart(store, cart, &totalCapacityOfCart);
                break;
            case 4:
                printPrice(cart);
                break;
            case 5:
                buyProducts(store, cart, &totalCapacityOfCart);
                break;
        }
    } while (choice != 6);

    std::cout << "Thanks for coming! " << std::endl;

    delete[] store->products;
    delete store;
    delete[] cart->products;
    delete cart;

    return 0;
}
