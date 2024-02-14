#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

class Product {
public:
    int ID;
    std::string Name;
    float Price;
    std::string Category;

    Product(int id, std::string name, float price, std::string category);
};

// Used to identify sorting method
enum class SortBy {
    ID,
    Name,
    Price
};

// Constructor
Product::Product(int id, std::string name, float price, std::string category) {
    ID = id;
    Name = name;
    Price = price;
    Category = category;
}

// Partition the vector for quicksort. 
int Partition(std::vector<Product> &products, int start, int end, SortBy sortBy) {
    float pivot;
    // Set the pivot to the last element
    // I've seen most implementations during my research use a random element, but they say any pivot is fine
    switch (sortBy) {
        case SortBy::ID:
            pivot = products[end].ID;
            break;
        case SortBy::Name:
            pivot = products[end].Name[0];
            break;
        case SortBy::Price:
            pivot = products[end].Price;
            break;
    }
    int i = start - 1;

    // Move elements smaller than the pivot to the left
    for (int j = start; j <= end - 1; j++) {
        float value;
        switch (sortBy) {
            case SortBy::ID:
                value = products[j].ID;
                break;
            case SortBy::Name:
                value = products[j].Name[0];
                break;
            case SortBy::Price:
                value = products[j].Price;
                break;
        }
        if (value < pivot) {
            i++;
            std::swap(products[i], products[j]);
        }
    }
    std::swap(products[i + 1], products[end]);
    return i + 1;
}

void QuickSort(std::vector<Product> &products, int start, int end, SortBy sortBy) {
    if (start < end) {
        int pivot = Partition(products, start, end, sortBy);
        // Split the vector based on pivot and recursively sort
        QuickSort(products, start, pivot - 1, sortBy);
        QuickSort(products, pivot + 1, end, sortBy);
    }
}

Product *BinarySearch(std::vector<Product> &products, SortBy sortBy, const std::string &value) {
    auto start = std::chrono::steady_clock::now();
    int left = 0;
    int right = products.size() - 1;
    
    while (left <= right) {
        // Find the middle element
        int middle = left + (right - left) / 2;
        // Depending on the sorting method, compare the middle element with the value
        switch (sortBy) {
            case SortBy::ID: {
                int id = std::stoi(value);
                // If the middle element = value, return pointer
                if (products[middle].ID == id) {
                    auto end = std::chrono::steady_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                    std::cout << "Search Time: " << duration.count() << " microseconds\n";
                    return &products[middle];
                }
                // If the middle element < value, move left pointer to the middle + 1
                if (products[middle].ID < id) {
                    left = middle + 1;
                } else {
                    // If the middle element > value, move right pointer to the middle - 1
                    right = middle - 1;
                }
                break;
            }
            case SortBy::Name: {
                if (products[middle].Name == value) {
                    auto end = std::chrono::steady_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                    std::cout << "Search Time: " << duration.count() << " microseconds\n";
                    return &products[middle];
                }
                if (products[middle].Name < value) {
                    left = middle + 1;
                } else {
                    right = middle - 1;
                }
                break;
            }
            case SortBy::Price: {
                float price = std::stof(value);
                if (products[middle].Price == price) {
                    return &products[middle];
                }
                if (products[middle].Price < price) {
                    left = middle + 1;
                } else {
                    right = middle - 1;
                }
                break;
            }
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Search Time: " << duration.count() << " microseconds\n";
    return nullptr;
}

void Help() {
    std::cout << "Commands: \n";
    std::cout << "help - Display help \n";
    std::cout << "insert - Add a new product \n";
    std::cout << "update - Modify existing product details \n";
    std::cout << "delete - Remove a product \n";
    std::cout << "search - Find product \n";
    std::cout << "print - Display all products \n";
    std::cout << "exit - Exit the program \n";
}

void Update(std::vector<Product> &products) {
    int id;

    std::cout << "Enter the ID: ";
    std::cin >> id;
    // The 1000 is kinda stupid, but it works
    std::cin.ignore(1000, '\n');

    auto start = std::chrono::steady_clock::now();

    //Sort the products by ID (for binary search)
    QuickSort(products, 0, products.size() - 1, SortBy::ID);

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Sort Time: " << duration.count() << " microseconds\n";
    
    //Search for the product
    std::string value = std::to_string(id);
    Product *product;
    
    product = BinarySearch(products, SortBy::ID, value);
    if (product == nullptr) {
        std::cout << "Product not found. \n";
        return;
    }

    

    std::string input;
    std::cout << "What to change? (ID, Name, Price, Category) *lowercase only*: ";
    std::cin >> input;

    if (input == "id") {
        std::cout << "Enter the new ID: ";
        std::cin >> id;
        start = std::chrono::steady_clock::now();
        product->ID = id;
        end = std::chrono::steady_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "Update Time: " << duration.count() << " microseconds\n";
    } else if (input == "name") {
        std::cout << "Enter the new name: ";
        std::cin.ignore(1000, '\n');
        std::getline(std::cin, input);
        product->Name = input;
    } else if (input == "price") {
        std::cout << "Enter the new price: ";
        std::cin.ignore(1000, '\n');
        std::getline(std::cin, input);
        product->Price = std::stof(input);
    } else if (input == "category") {
        std::cout << "Enter the new category: ";
        std::cin.ignore(1000, '\n');
        std::getline(std::cin, input);
        product->Category = input;
    } else {
        std::cout << "Invalid input. \n";
    }
}

void Insert(std::vector<Product> &products) {
    int id;
    float price;
    std::string name, category;

    std::cout << "Enter the ID: ";
    std::cin >> id;
    std::cout << "Enter the name: ";
    std::cin.ignore(1000, '\n');
    std::getline(std::cin, name);
    std::cout << "Enter the price: ";
    std::cin >> price;
    std::cout << "Enter the category: ";
    std::cin.ignore(1000, '\n');
    std::getline(std::cin, category);
    auto start = std::chrono::steady_clock::now();
    // Create a new product and add it to the vector
    products.push_back(Product(id, name, price, category));
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

void Delete(std::vector<Product> &products) {
    int id;
    std::cout << "Enter the ID of the product you want to delete: ";
    std::cin >> id;
    std::cin.ignore(1000, '\n');

    auto start = std::chrono::steady_clock::now();

    //Sort the products by ID (for binary search)
    QuickSort(products, 0, products.size() - 1, SortBy::ID);

    //Search for the product
    std::string value = std::to_string(id);
    Product *product;

    product = BinarySearch(products, SortBy::ID, value);
    if (product == nullptr) {
        std::cout << "Product not found. \n";
        return;
    }

    // Binary search does not return the index, so I'll just make a linear search to find the index :(
    // I kinda don't wanna remake another binary search function that returns the index since my brain
    // is already fried from writing everything else T-T
    for (size_t i = 0; i < products.size(); i++) {
        if (products[i].ID == id) {
            products.erase(products.begin() + i);
            std::cout << "Product deleted successfully. \n";
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << "Time: " << duration.count() << " microseconds\n";
            return;
        }
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time: " << duration.count() << " microseconds\n";
}

void Search(std::vector<Product> &products) {
    std::string input;
    std::cout << "Search by? (ID, Name) *lowercase only*: ";
    std::cin >> input;
    if (input == "id") {
        int id;
        std::cout << "Enter the ID: ";
        std::cin >> id;
        std::string value = std::to_string(id);
        // Make sure to sort the products by ID before searching
        QuickSort(products, 0, products.size() - 1, SortBy::ID);
        Product *product;
        product = BinarySearch(products, SortBy::ID, value);
        if (product == nullptr) {
            std::cout << "Product not found. \n";
        } else {
            std::cout << "Product found: \n";
            std::cout << "ID: " << product->ID << " Name: " << product->Name << " Price: " << product->Price
                      << " Category: " << product->Category << "\n";
        }
    } else if (input == "name") {
        std::string name;
        std::cout << "Enter the name: ";
        std::cin.ignore(1000, '\n');
        std::getline(std::cin, name);
        // Make sure to sort the products by Name before searching
        QuickSort(products, 0, products.size() - 1, SortBy::Name);
        Product *product;
        product = BinarySearch(products, SortBy::Name, name);
        if (product == nullptr) {
            std::cout << "Product not found. \n";
        } else {
            std::cout << "ID: " << product->ID << " Name: " << product->Name << " Price: " << product->Price
                      << " Category: " << product->Category << "\n";
        }
    } else {
        std::cout << "Invalid input. \n";
    }
}

void Print(std::vector<Product> &products) {
    for (int i = 0; i < products.size(); i++) {
        std::cout << "ID: " << products[i].ID << " Name: " << products[i].Name << " Price: " << products[i].Price
                  << " Category: " << products[i].Category << "\n";
    }
    std::cout << products.size() << " products. \n";
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream file("data/product_data.txt");
    if (file.fail()) {
        std::cerr << "Error opening file. The file was expected in a folder named \"data\"\n";
    }
    std::string line;
    //Using a vector instead of an array for dynamic size and therefore more efficient
    std::vector<Product> products; 

    //Read the file and create a new product on each line
    while (std::getline(file, line)) {
        size_t pos = 0;
        std::string token;
        std::vector<std::string> tokens;

        while ((pos = line.find(", ")) != std::string::npos) {
            token = line.substr(0, pos);
            tokens.push_back(token);
            // +2 to remove ", " (comma + space)
            line.erase(0, pos + 2); 
        }
        // last token after the last comma
        tokens.push_back(line); 

        //Create a new product and add it to the vector
        products.push_back(Product(std::stoi(tokens[0]), tokens[1], std::stof(tokens[2]), tokens[3]));
    }

    std::cout << products.size() << " products loaded. \n";

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time: " << duration.count() << " microseconds\n";
    std::cout << "Sorting products by ID... \n";
    QuickSort(products, 0, products.size() - 1, SortBy::ID);
    std::cout << "Sorting Complete! \n";

    bool running = true;
    do {
        //Ask user for input
        std::string input;
        std::cout << "\nEnter a command (type help for commands): \n";
        std::getline(std::cin, input);
        if (input == "help") {
            Help();
        } else if (input == "insert") {
            Insert(products);
        } else if (input == "update") {
            Update(products);
        } else if (input == "delete") {
            Delete(products);
        } else if (input == "search") {
            Search(products);
        } else if (input == "print") {
            Print(products);
        } else if (input == "exit") {
            running = false;
        } else {
            std::cout << "Invalid command. \n";
        }
        std::cout << std::endl;
    } while (running);
    return 0;
}