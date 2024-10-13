#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ANSI color codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define BLUE        "\033[34m"
#define CYAN        "\033[36m"

/*
Compile the code using:
gcc database.c -o database

Run the program:
./database
*/


// Function declaration for printLine
void printLine(char ch, int length);

// Structure for a product
typedef struct Product {
    int id;            // Product ID
    char name[100];    // Product name
    float price;       // Product price
    int quantity;      // Product quantity
    struct Product* left;   // Left child
    struct Product* right;  // Right child
} Product;

// Function to create a new product node
Product* createProduct(int id, const char* name, float price, int quantity) {
    Product* newProduct = (Product*)malloc(sizeof(Product));
    newProduct->id = id;
    strcpy(newProduct->name, name);
    newProduct->price = price;
    newProduct->quantity = quantity;
    newProduct->left = NULL;
    newProduct->right = NULL;
    return newProduct;
}

// Function to check for unique ID
int isUniqueID(Product* root, int id) {
    if (root == NULL) return 1; // ID is unique

    if (root->id == id) return 0; // ID exists
    return (id < root->id) ? isUniqueID(root->left, id) : isUniqueID(root->right, id);
}

// Function to check for unique name
int isUniqueName(Product* root, const char* name) {
    if (root == NULL) return 1; // If tree is empty, name is unique

    // Compare the current node's name with the input name
    if (strcmp(root->name, name) == 0) return 0; // Name already exists

    // Recursively check left and right subtrees
    return (strcmp(name, root->name) < 0) ? isUniqueName(root->left, name) : isUniqueName(root->right, name);
}

// Function to insert a new product into the BST
Product* insertProduct(Product* root, int id, const char* name, float price, int quantity) {
    if (!isUniqueID(root, id)) {
        printf(RED "Error: A product with ID %d already exists.\n" RESET, id);
        return root; // Return without inserting
    }

    if (root == NULL) {
        return createProduct(id, name, price, quantity);
    }

    // Insert based on name for alphabetical order
    if (strcmp(name, root->name) < 0) {
        root->left = insertProduct(root->left, id, name, price, quantity);
    } else {
        root->right = insertProduct(root->right, id, name, price, quantity);
    }

    return root;
}

// Function to search for a product by name
Product* searchProduct(Product* root, const char* name) {
    if (root == NULL || strcmp(root->name, name) == 0) {
        return root;
    }

    if (strcmp(name, root->name) < 0) {
        return searchProduct(root->left, name);
    } else {
        return searchProduct(root->right, name);
    }
}

// Function to validate if a string is a valid integer
int isValidInt(const char* str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

// Function to validate if a string is a valid float
int isValidFloat(const char* str) {
    int dotFound = 0;
    while (*str) {
        if (*str == '.') {
            if (dotFound) return 0; // More than one dot
            dotFound = 1;
        } else if (!isdigit(*str)) {
            return 0; // Non-digit character found
        }
        str++;
    }
    return 1;
}

// Function to update product details
void updateProduct(Product* root, const char* name) {
    Product* product = searchProduct(root, name);
    if (product != NULL) {
        char input[100];

        printf("\n" BLUE "Updating product: %s (ID: %d)\n" RESET, product->name, product->id);
        
        // Update loop
        while (1) {
            printf("\n" CYAN "What would you like to update? (Enter 'done' to finish)\n" RESET);
            printf("1. Product ID\n");
            printf("2. Product Name\n");
            printf("3. Product Price\n");
            printf("4. Product Quantity\n");
            printf("Enter your choice: ");
            scanf(" %[^\n]", input);

            if (strcmp(input, "done") == 0) {
                break; // Exit the update loop
            }

            if (strcmp(input, "1") == 0) {
                printf("Enter New Product ID: ");
                scanf(" %[^\n]", input);
                if (isValidInt(input)) {
                    product->id = atoi(input);
                    printf(GREEN "Product ID updated to: %d\n" RESET, product->id);
                } else {
                    printf(RED "Invalid ID. Please enter a valid number.\n" RESET);
                }
            } else if (strcmp(input, "2") == 0) {
                printf("Enter New Product Name: ");
                scanf(" %[^\n]", product->name);
                printf(GREEN "Product Name updated to: %s\n" RESET, product->name);
            } else if (strcmp(input, "3") == 0) {
                printf("Enter New Product Price: ");
                scanf(" %[^\n]", input);
                if (isValidFloat(input)) {
                    product->price = atof(input);
                    printf(GREEN "Product Price updated to: %.2f\n" RESET, product->price);
                } else {
                    printf(RED "Invalid price. Please enter a valid number.\n" RESET);
                }
            } else if (strcmp(input, "4") == 0) {
                printf("Enter New Product Quantity: ");
                scanf(" %[^\n]", input);
                if (isValidInt(input)) {
                    product->quantity = atoi(input);
                    printf(GREEN "Product Quantity updated to: %d\n" RESET, product->quantity);
                } else {
                    printf(RED "Invalid quantity. Please enter a valid number.\n" RESET);
                }
            } else {
                printf(RED "Invalid choice. Please try again.\n" RESET);
            }
        }

        printf(BLUE "Product updated successfully: %s (ID: %d)\n" RESET, product->name, product->id);
    } else {
        printf(RED "Product not found: %s\n" RESET, name);
    }
}

// Function to collect products into an array (helper for sorting)
void collectProducts(Product* node, Product** products, int* count) {
    if (node == NULL) return;
    collectProducts(node->left, products, count);
    products[(*count)++] = node;
    collectProducts(node->right, products, count);
}

// Function to display products in a tabular format (in-order traversal)
void displayProducts(Product* root) {
    if (root == NULL) {
        return; // If there are no products, exit the function
    }

    // Traverse left subtree
    displayProducts(root->left);

    // Print current node
    printf("| %-8d | %-20s | %-10.2f | %-10d |\n", root->id, root->name, root->price, root->quantity);

    // Traverse right subtree
    displayProducts(root->right);
}

// Function to sort and display products based on a given criteria
void displaySortedProducts(Product* root, int choice) {
    // Create an array to hold products for sorting
    Product* products[100]; // Adjust size as needed
    int count = 0;

    // Collect products into the array
    collectProducts(root, products, &count);

    // Simple bubble sort based on the user's choice
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int swap = 0;
            if (choice == 1 && products[j]->id > products[j + 1]->id) swap = 1;     // Sort by ID
            else if (choice == 2 && strcmp(products[j]->name, products[j + 1]->name) > 0) swap = 1; // Sort by Name
            else if (choice == 3 && products[j]->price > products[j + 1]->price) swap = 1;  // Sort by Price
            else if (choice == 4 && products[j]->quantity > products[j + 1]->quantity) swap = 1; // Sort by Quantity
            
            if (swap) {
                Product* temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }
    // Print sorting type
    const char* sortBy = (choice == 1) ? "ID" : (choice == 2) ? "Name" : (choice == 3) ? "Price" : "Quantity";
    printf(GREEN "\nSorted by %s" RESET, sortBy);

    // Print sorted products
    printf("\n| %-8s | %-20s | %-10s | %-10s |\n", "Item ID", "Item Name", "Price", "Quantity");
    printLine('-', 65);
    for (int i = 0; i < count; i++) {
        printf("| %-8d | %-20s | %-10.2f | %-10d |\n", products[i]->id, products[i]->name, products[i]->price, products[i]->quantity);
    }
    printLine('-', 65);
}

// Forward declaration of the helper function
void traverseAndCheck(Product* node, int* outOfStockFound);

// Helper function to traverse the tree and check for out-of-stock products
void traverseAndCheck(Product* node, int* outOfStockFound) {
    if (node != NULL) {
        traverseAndCheck(node->left, outOfStockFound);
        if (node->quantity == 0) {
            printf(RED "Out of stock: %s (ID: %d)\n" RESET, node->name, node->id);
            *outOfStockFound = 1; // Set the flag if out of stock
        }
        traverseAndCheck(node->right, outOfStockFound);
    }
}

// Function to track out-of-stock products
void trackOutOfStock(Product* root) {
    // Check if there are no products
    if (root == NULL) {
        printf(RED "No products available.\n" RESET);
        return; // Exit the function if there are no products
    }

    int outOfStockFound = 0; // Flag to track if any out-of-stock products are found

    // Start traversal
    traverseAndCheck(root, &outOfStockFound);

    // Check if any out-of-stock products were found
    if (!outOfStockFound) {
        printf(GREEN "All products are in stock.\n" RESET);
    }
}

// Function to print decorative lines
void printLine(char ch, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", ch);
    }
    printf("\n");
}

// Main function
int main() {
    Product* root = NULL;
    int choice, id, quantity;
    char name[100];
    float price;

    do {
        printLine('-', 50);
        printf(BOLD "Simple Database System\n" RESET);
        printLine('*', 50);
        printf(BLUE "1. Insert New Product\n" RESET);
        printf(BLUE "2. Search for Product by Name\n" RESET);
        printf(BLUE "3. Update Product Details\n" RESET);
        printf(BLUE "4. Display All Products\n" RESET);
        printf(BLUE "5. Sort Products\n" RESET);
        printf(BLUE "6. Track Out-of-Stock Products\n" RESET);
        printf(BLUE "7. Exit\n" RESET);
        printLine('-', 50);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
    printf("Enter Product ID: ");
    scanf(" %[^\n]", name); // Use name variable for input validation
    if (!isValidInt(name)) {
        printf(RED "Invalid ID. Please enter a valid number.\n" RESET);
        break;
    }
    id = atoi(name);

    // Check for uniqueness of ID
    if (!isUniqueID(root, id)) {
        printf(RED "Error: A product with ID %d already exists.\n" RESET, id);
        break; // Exit if ID is not unique
    }

    // Use a different variable for name input
    char productName[100]; // Separate variable for product name
    printf("Enter Product Name: ");
    scanf(" %[^\n]", productName);

    // Check for uniqueness of name
    if (!isUniqueName(root, productName)) {
        printf(RED "Error: A product with the name '%s' already exists.\n" RESET, productName);
        break; // Exit if name is not unique
    }

    // Validate and get price
    printf("Enter Product Price: ");
    scanf(" %[^\n]", name); // Reuse name for price input
    if (!isValidFloat(name)) {
        printf(RED "Invalid price. Please enter a valid number.\n" RESET);
        break;
    }
    price = atof(name);

    // Validate and get quantity
    printf("Enter Product Quantity: ");
    scanf(" %[^\n]", name); // Reuse name for quantity input
    if (!isValidInt(name)) {
        printf(RED "Invalid quantity. Please enter a valid number.\n" RESET);
        break;
    }
    quantity = atoi(name);

    // Insert the product using the correct name variable
    root = insertProduct(root, id, productName, price, quantity);
    printf(GREEN "Product added: %s (ID: %d)\n" RESET, productName, id);
    break;
            case 2:
                printf("Enter Product Name to Search: ");
                scanf(" %[^\n]", name);
                Product* foundProduct = searchProduct(root, name);
                if (foundProduct != NULL) {
                    printf(GREEN "Product found: ID: %d, Name: %s, Price: %.2f, Quantity: %d\n" RESET,
                           foundProduct->id, foundProduct->name, foundProduct->price, foundProduct->quantity);
                } else {
                    printf(RED "Product not found: %s\n" RESET, name);
                }
                break;
            case 3:
                printf("Enter Product Name to Update: ");
                scanf(" %[^\n]", name);
                updateProduct(root, name);
                break;
            case 4:
                if (root == NULL) {
                    printf(RED "No products available to display.\n" RESET);
                } else {
                    printf("\n| %-8s | %-20s | %-10s | %-10s |\n", "Item ID", "Item Name", "Price", "Quantity");
                    printLine('-', 65);
                    displayProducts(root);
                    printLine('-', 65);
                }
                break;
            case 5:
                printf("Choose sorting option:\n");
                printf("1. Sort by ID\n");
                printf("2. Sort by Name\n");
                printf("3. Sort by Price\n");
                printf("4. Sort by Quantity\n");
                printf("Enter your choice: ");
                int sortChoice;
                scanf("%d", &sortChoice);
                displaySortedProducts(root, sortChoice);
                break;
            case 6:
                printf("Out-of-Stock Products:\n");
                trackOutOfStock(root);
                break;
            case 7:
                printf(RED "Exiting...\n" RESET);
                break;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    } while (choice != 7);

    return 0;
}