#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <limits>
#include <queue>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>


using namespace std;
// Structure to represent a location with coordinates and an address
struct Location {
    double x, y;        // x and y coordinates
    string address;     // Address of the location
};

// Structure to represent a driver with their details
struct Driver {
    string name;         // Name of the driver
    double maxCapacity;  // Maximum load capacity
    double currentCapacity; // Current load
    Location location;   // Current location of the driver
};

// Structure to represent a delivery with its details
struct Delivery {
    Location location;   // Delivery location
    double weight;       // Weight of the delivery
    string status;       // Status of the delivery (e.g., "Pending")
};

// Utility function to calculate the Euclidean distance between two locations
double calculateDistance(Location a, Location b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Function to insert a new city into the system
void insertCity(vector<Location>& cities) {
    Location newCity;

    cout << "Enter the coordinates of the city (x y): ";
    cin >> newCity.x >> newCity.y; // Input city coordinates

    cout << "Enter the name of the city: ";
    cin.ignore();

    getline(cin, newCity.address); // Input city address

    cities.push_back(newCity); // Add city to the list
    cout << "City added successfully!\n";
}

// Function to insert a new driver into the system
void insertDriver(vector<Driver>& drivers) {
    Driver newDriver;

    cout << "Enter the driver's name: ";
    cin.ignore();

    getline(cin, newDriver.name); // Input driver's name
    cout << "Enter the driver's maximum capacity: ";
    cin >> newDriver.maxCapacity; // Input max capacity

    cout << "Enter the driver's current capacity: ";
    cin >> newDriver.currentCapacity; // Input current capacity

    cout << "Enter the driver's location (x y): ";
    cin >> newDriver.location.x >> newDriver.location.y; // Input location coordinates

    cout << "Enter the address of the location: ";
    cin.ignore();

    getline(cin, newDriver.location.address); // Input location address
    drivers.push_back(newDriver); // Add driver to the list

    cout << "Driver added successfully!\n";
}

// Function to insert a new delivery into the system
void insertDelivery(vector<Delivery>& deliveries)
 {
    Delivery newDelivery;

    cout << "Enter the delivery location coordinates (x y): ";
    cin >> newDelivery.location.x >> newDelivery.location.y; // Input delivery location

    cout << "Enter the delivery location address: ";
    cin.ignore();

    getline(cin, newDelivery.location.address); // Input location address
    cout << "Enter the weight of the delivery: ";
    cin >> newDelivery.weight; // Input weight

    newDelivery.status = "Pending"; // Set initial status
    deliveries.push_back(newDelivery); // Add delivery to the list

    cout << "Delivery added successfully!\n";
}

// Function to display distances of all cities from the warehouse
void case1(const vector<Location>& cities)
 {
    cout << "Available cities and their distances from Warehouse: \n";
    Location warehouse = cities[0]; // Assume the first city is the warehouse

    for (int i = 1; i < cities.size(); ++i) {
        double distance = calculateDistance(warehouse, cities[i]); // Calculate distance
        cout << cities[i].address << ": " << distance << " km" << endl;
    }
}

// Function to calculate the shortest distance and time between two cities using Prim's Algorithm
void case2(const vector<Location>& cities)
 {
    cout << "Available cities: \n";
    for (int i = 0; i < cities.size(); ++i) {
        cout << i + 1 << ". " << cities[i].address << endl;
    }

    cout << "Enter the city numbers to calculate distance and time (1-indexed): ";
    int city1, city2;
    cin >> city1 >> city2;
    city1--; city2--; // Convert to 0-indexed

    int n = cities.size();
    vector<vector<double>> adjMatrix(n, vector<double>(n, numeric_limits<double>::infinity()));

    // Fill the adjacency matrix with distances between cities
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dist = calculateDistance(cities[i], cities[j]);
            adjMatrix[i][j] = dist;
            adjMatrix[j][i] = dist;
        }
    }

    // Prim's algorithm to find the shortest path between city1 and city2
    vector<double> minDist(n, numeric_limits<double>::infinity()); // Minimum distances
    vector<bool> visited(n, false); // Visited nodes
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq; // Min-heap

    minDist[city1] = 0;
    pq.push({0, city1});

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue; // Skip already visited nodes
        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && adjMatrix[u][v] < numeric_limits<double>::infinity()) {
                if (minDist[v] > minDist[u] + adjMatrix[u][v]) {
                    minDist[v] = minDist[u] + adjMatrix[u][v];
                    pq.push({minDist[v], v});
                }
            }
        }
    }

    double distance = minDist[city2]; // Shortest distance
    cout << "Shortest distance between " << cities[city1].address << " and " << cities[city2].address << ": " << distance << " km" << endl;

    // Calculate time assuming a fixed speed of 50 km/h
    double time = distance / 50.0;
    cout << "Time to travel: " << time << " hours" << endl;
}

// Function to find the closest driver to a delivery location
void searchClosestDriver(const vector<Driver>& drivers, const vector<Location>& cities)
 {
    cout << "Available cities: \n";
    for (int i = 0; i < cities.size(); ++i) {
        cout << i + 1 << ". " << cities[i].address << endl;
    }

    cout << "Enter the city number to find the closest driver: ";
    int cityIndex;
    cin >> cityIndex;
    cityIndex--; // Convert to 0-indexed

    if (cityIndex < 0 || cityIndex >= cities.size()) {
        cout << "Invalid city number.\n";
        return;
    }

    Location cityLocation = cities[cityIndex]; // Get selected city's location
    double minDistance = numeric_limits<double>::max(); // Minimum distance
    Driver closestDriver; // Closest driver

    // Find the closest driver to the selected city
    for (const auto& driver : drivers)
        {
        double distance = calculateDistance(driver.location, cityLocation);
        if (distance < minDistance)
        {
            minDistance = distance;
            closestDriver = driver;
        }
    }

    cout << "The closest driver to the city " << cities[cityIndex].address << " is " << closestDriver.name << endl;
}

// Function to optimize the delivery route using a greedy approach (TSP Approximation)
void optimizeDeliveryRoute(const vector<Location>& cities, const Location& warehouse)
 {
    cout << "Enter the number of cities to optimize the route: ";
    int numCities;
    cin >> numCities;

    vector<int> cityIndexes(numCities);
    cout << "Enter city numbers (1-indexed) to visit (e.g. 1 for Warehouse, 2 for Jaipur, etc.): ";
    for (int i = 0; i < numCities; ++i)
        {
        cin >> cityIndexes[i];
        cityIndexes[i]--; // Convert to 0-indexed
    }

    vector<bool> visited(cities.size(), false); // Visited status
    vector<Location> optimizedRoute; // Optimized route
    Location currentLocation = warehouse; // Start from

        visited[0] = true; // Mark warehouse as visited
    optimizedRoute.push_back(currentLocation); // Add warehouse to the route

    while (optimizedRoute.size() <= numCities)
        {
        double minDistance = numeric_limits<double>::max();
        int nextCityIndex = -1;

        // Find the closest unvisited city
        for (int i = 0; i < numCities; ++i)
            {
            if (!visited[cityIndexes[i]])
             {
                double distance = calculateDistance(currentLocation, cities[cityIndexes[i]]);
                if (distance < minDistance)
                 {
                    minDistance = distance;
                    nextCityIndex = cityIndexes[i];
                }
            }
        }

        // If no more cities are unvisited, break the loop
        if (nextCityIndex == -1) break;

        // Move to the next closest city
        visited[nextCityIndex] = true;
        currentLocation = cities[nextCityIndex];
        optimizedRoute.push_back(currentLocation);
    }

    // Add the warehouse back to complete the route
    optimizedRoute.push_back(warehouse);

    // Print the optimized route
    cout << "Optimized delivery route: \n";
    for (const auto& loc : optimizedRoute)
        {
        cout << loc.address << " (" << loc.x << ", " << loc.y << ")\n";
    }

    // Calculate total distance
    double totalDistance = 0.0;
    for (size_t i = 1; i < optimizedRoute.size(); ++i)
        {
        totalDistance += calculateDistance(optimizedRoute[i - 1], optimizedRoute[i]);
    }

    cout << "Total distance covered: " << totalDistance << " km\n";
}

// Main function to manage the system
void business_case1()
{
    // Predefined cities
    vector<Location> cities = {
        {0, 0, "Warehouse"},
        {26.9124, 75.7873, "Jaipur"},
        {24.5854, 73.7125, "Udaipur"},
        {26.2957, 73.0163, "Jodhpur"},
        {26.9157, 70.9075, "Jaisalmer"},
        {28.0229, 73.3120, "Bikaner"},
        {26.4927, 74.5516, "Pushkar"},
        {26.4453, 74.6399, "Ajmer"},
        {24.8797, 74.6261, "Chittorgarh"},
        {25.2138, 75.8689, "Kota"},
        {27.3963, 76.6129, "Alwar"},
        {27.6100, 75.2130, "Sikar"},
        {25.7988, 73.0011, "Pali"},
        {25.7585, 71.4187, "Barmer"},
        {29.9160, 72.2707, "Sri Ganganagar"},
        {26.0955, 75.7899, "Tonk"}
    };

    // Predefined drivers
    vector<Driver> drivers = {
        {"Alice", 50.0, 30.0, {0, 0, "Warehouse"}},
        {"Bob", 40.0, 20.0, {26.9124, 75.7873, "Jaipur"}},
        {"Charlie", 60.0, 40.0, {24.5854, 73.7125, "Udaipur"}},
        {"David", 50.0, 30.0, {26.2957, 73.0163, "Jodhpur"}},
        {"Eve", 45.0, 25.0, {26.9157, 70.9075, "Jaisalmer"}},
        {"Frank", 60.0, 35.0, {28.0229, 73.3120, "Bikaner"}},
        {"Grace", 50.0, 28.0, {26.4927, 74.5516, "Pushkar"}},
        {"Hank", 70.0, 50.0, {26.4453, 74.6399, "Ajmer"}},
        {"Ivy", 55.0, 40.0, {24.8797, 74.6261, "Chittorgarh"}},
        {"Jack", 65.0, 45.0, {25.2138, 75.8689, "Kota"}},
        {"John", 55.0, 35.0, {27.3963, 76.6129, "Alwar"}},
        {"Liam", 50.0, 30.0, {27.6100, 75.2130, "Sikar"}},
        {"Mia", 45.0, 30.0, {25.7988, 73.0011, "Pali"}},
        {"Nina", 50.0, 30.0, {25.7585, 71.4187, "Barmer"}},
        {"Oliver", 60.0, 40.0, {29.9160, 72.2707, "Sri Ganganagar"}},
        {"Paul", 65.0, 45.0, {26.0955, 75.7899, "Tonk"}}
    };

    // Predefined delivery
    vector<Delivery> deliveries = {
        {{26.9124, 75.7873, "Jaipur"}, 10.0, "Pending"}
    };


    int choice;
    do {
        cout << "\n--- Delivery Management System ---\n";
        cout << "1. Display distances of all cities from the warehouse\n";
        cout << "2. Calculate shortest distance and time between two cities\n";
        cout << "3. Insert a new city\n";
        cout << "4. Insert a new driver\n";
        cout << "5. Insert a new delivery\n";
        cout << "6. Search for the closest driver to a delivery location\n";
        cout << "7. Optimize delivery route\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
         {
            case 1:
                case1(cities);
                break;
            case 2:
                case2(cities);
                break;
            case 3:
                insertCity(cities);
                break;
            case 4:
                insertDriver(drivers);
                break;
            case 5:
                insertDelivery(deliveries);
                break;
            case 6:
                searchClosestDriver(drivers, cities);
                break;
            case 7:
                optimizeDeliveryRoute(cities, cities[0]); // Pass the warehouse as the starting point
                break;
            case 8:
                return;
                //cout << "Exiting the system. Goodbye!\n";
               // break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);

}

// Structure to hold gadget data
struct Gadget
{
    string name;
    int salesCount;
    double price;
};

// Quick Sort partition function
int partition(vector<Gadget>& gadgets, int low, int high)
 {
    int pivot = gadgets[high].salesCount; // Pivot is the salesCount of the last element
    int i = low - 1;

    for (int j = low; j < high; ++j)
        {
        if (gadgets[j].salesCount <= pivot)
         {
            ++i;
            swap(gadgets[i], gadgets[j]);
        }
    }
    swap(gadgets[i + 1], gadgets[high]);
    return i + 1;
}

// Quick Sort function
void quickSort(vector<Gadget>& gadgets, int low, int high)
 {
    if (low < high) {
        int pi = partition(gadgets, low, high);
        quickSort(gadgets, low, pi - 1);
        quickSort(gadgets, pi + 1, high);
    }
}

// Function to display all gadgets
void displayGadgets(const vector<Gadget>& gadgets)
 {
    if (gadgets.empty())
        {
        cout << "No gadgets available.\n";
        return;
    }

    cout << "\nGadgets List:\n";
    cout << "Name\t\tSalesCount\tPrice\n";
    for (const auto& gadget : gadgets)
        {
        cout << gadget.name << "\t\t" << gadget.salesCount
             << "\t\t" << fixed << setprecision(2) << gadget.price << "\n";
    }
}

// Function to increment or decrement prices
void adjustPrices(vector<Gadget>& gadgets, bool increment)
 {
    // Sort gadgets by sales count before applying the operation
    quickSort(gadgets, 0, gadgets.size() - 1);
    displayGadgets(gadgets); // Show sorted list to users

    string name;
    cout << "Enter gadget name to adjust price (type 'done' to finish): ";
    while (cin >> name && name != "done")
        {
        bool found = false;
        for (auto& gadget : gadgets)
         {
            if (gadget.name == name)
            {
                double adjustment = gadget.price * (gadget.salesCount / 100.0);
                if (increment)
                 {
                    gadget.price += adjustment;
                    cout << "Price incremented for " << gadget.name << "\n";
                } else
                {
                    gadget.price -= adjustment;
                    if (gadget.price < 0) gadget.price = 0;
                    cout << "Price decremented for " << gadget.name << "\n";
                }
                found = true;
                break;
            }
        }
        if (!found) cout << "Gadget " << name << " not found!\n";
        cout << "Enter another gadget name (or 'done' to finish): ";
    }
}

// Other Functions (Add, Search, Remove, Apply Discount) remain the same
void addGadget(vector<Gadget>& gadgets)
 {
    Gadget gadget;
    cout << "Enter gadget name: ";
    cin.ignore();
    getline(cin, gadget.name);
    cout << "Enter sales count: ";
    cin >> gadget.salesCount;
    cout << "Enter price: ";
    cin >> gadget.price;
    gadgets.push_back(gadget);
    cout << "Gadget added successfully.\n";
}

void searchGadget(const vector<Gadget>& gadgets, const string& name)
 {
    for (const auto& gadget : gadgets)
        {
        if (gadget.name == name) {
            cout << "Gadget found: " << gadget.name
                 << ", Sales Count: " << gadget.salesCount
                 << ", Price: " << fixed << setprecision(2) << gadget.price << "\n";
            return;
        }
    }
    cout << "Gadget not found!\n";
}

void removeLowSales(vector<Gadget>& gadgets, int threshold)
{
    size_t originalSize = gadgets.size();
    gadgets.erase(
        remove_if(gadgets.begin(), gadgets.end(), [threshold](const Gadget& g)
                   {
            return g.salesCount < threshold;
        }),
        gadgets.end()
    );
    if (gadgets.size() < originalSize)
        cout << "Gadgets with sales count below " << threshold << " have been removed.\n";
    else
        cout << "No gadgets met the removal criteria.\n";
}

void applyDiscount(vector<Gadget>& gadgets, double discountPercentage)
{
    for (auto& gadget : gadgets)
        {
        gadget.price -= gadget.price * (discountPercentage / 100.0);
        if (gadget.price < 0) gadget.price = 0;
    }
    cout << "Discount of " << discountPercentage << "% applied to all gadgets.\n";
}

void business_case2()
{
    vector<Gadget> gadgets =
     {
        {"Smartphone", 50, 299.99},
         {"Tablet", 30, 199.99},
          {"Laptop", 20, 999.99},
        {"Smartwatch", 70, 149.99},
         {"Headphones", 80, 89.99},
          {"Camera", 15, 499.99},
        {"Speaker", 60, 79.99},
         {"Monitor", 25, 199.99},
         {"Keyboard", 40, 49.99},
        {"Mouse", 45, 29.99},
         {"Drone", 10, 599.99},
         {"Projector", 5, 449.99},
        {"PowerBank", 90, 39.99},
         {"Charger", 100, 19.99},
        {"Router", 35, 129.99}
    };

    int choice;

    while (true) {
        cout << "\n--- Gadget Management System ---\n";
        cout << "1. Display All Gadgets\n";
        cout << "2. Search Gadget by Name\n";
        cout << "3. Increment Prices for Selected Gadgets\n";
        cout << "4. Decrement Prices for Selected Gadgets\n";
        cout << "5. Add New Gadget\n";
        cout << "6. Remove Gadgets with Low Sales Count\n";
        cout << "7. Apply Discount to All Gadgets\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                displayGadgets(gadgets);
                break;
            case 2:
                 {
                string name;
                cout << "Enter gadget name to search: ";
                cin.ignore();
                getline(cin, name);
                searchGadget(gadgets, name);
                break;
            }
            case 3:
                adjustPrices(gadgets, true);
                break;
            case 4:
                adjustPrices(gadgets, false);
                break;
            case 5:
                addGadget(gadgets);
                break;
            case 6:
                 {
                int threshold;
                cout << "Enter sales count threshold: ";
                cin >> threshold;
                removeLowSales(gadgets, threshold);
                break;
            }
            case 7:
                 {
                double discountPercentage;
                cout << "Enter discount percentage: ";
                cin >> discountPercentage;
                applyDiscount(gadgets, discountPercentage);
                break;
            }
            case 8:
                cout << "Exiting program. Goodbye!\n";
                return;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }

}

// Binary Search Tree Node
struct TreeNode
 {
    int data;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int value) : data(value), left(nullptr), right(nullptr) {}
};

// Function to insert a value into the BST
TreeNode* insertBST(TreeNode* root, int value)
 {
    if (!root) return new TreeNode(value);
    if (value < root->data)
        root->left = insertBST(root->left, value);
    else
        root->right = insertBST(root->right, value);
    return root;
}

// Inorder Traversal of BST (sorted output)
void inorderBST(TreeNode* root)
 {
    if (!root) return;
    inorderBST(root->left);
    cout << root->data << " ";
    inorderBST(root->right);
}

// Quick Sort
void quickSort(vector<int>& arr, int low, int high)
{
    if (low < high)
        {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++)
            {
            if (arr[j] < pivot)
             {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int pi = i + 1;

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Merge Sort
void merge(vector<int>& arr, int left, int mid, int right)
 {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
        {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int left, int right)
 {
    if (left < right)
        {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Main function
void business_case3()
{
    cout << "Welcome to E-Commerce Order Management System!\n";

    // Binary Search Tree for storing customer order IDs
    TreeNode* root = nullptr;
    int nOrders;
    cout << "Enter the number of customer orders: ";
    cin >> nOrders;

    cout << "Enter the order IDs (to store in BST): ";
    for (int i = 0; i < nOrders; i++)
        {
        int orderID;
        cin >> orderID;
        root = insertBST(root, orderID);
    }

    cout << "\nOrder IDs (sorted using BST inorder traversal): ";
    inorderBST(root);
    cout << "\n";

    // Quick Sort for sorting delivery times
    int nTimes;
    cout << "\nEnter the number of delivery times to sort: ";
    cin >> nTimes;

    vector<int> deliveryTimes(nTimes);
    cout << "Enter the delivery times (in minutes): ";
    for (int i = 0; i < nTimes; i++)
        {
        cin >> deliveryTimes[i];
    }

    quickSort(deliveryTimes, 0, nTimes - 1);
    cout << "\nSorted Delivery Times (using Quick Sort): ";
    for (int time : deliveryTimes)
        {
        cout << time << " ";
    }
    cout << "\n";

    // Merge Sort for sorting order amounts
    int nAmounts;
    cout << "\nEnter the number of order amounts to sort: ";
    cin >> nAmounts;

    vector<int> orderAmounts(nAmounts);
    cout << "Enter the order amounts: ";
    for (int i = 0; i < nAmounts; i++)
        {
        cin >> orderAmounts[i];
    }

    mergeSort(orderAmounts, 0, nAmounts - 1);
    cout << "\nSorted Order Amounts (using Merge Sort): ";
    for (int amount : orderAmounts)
        {
        cout << amount << " ";
    }
    cout << "\n";

}


// Function for Bubble Sort
void bubbleSort(vector<int>& arr)
 {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        {
        for (int j = 0; j < n - i - 1; j++)
         {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Function for Selection Sort
void selectionSort(vector<int>& arr)
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
         {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }
        swap(arr[i], arr[minIndex]);
    }
}

// Function for Binary Search
int binarySearch(const vector<int>& arr, int key)
 {
    int low = 0, high = arr.size() - 1;
    while (low <= high)
        {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key)
            {
            return mid;
        } else if (arr[mid] < key)
         {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1; // Key not found
}

void business_case4()
{
    cout << "Welcome to Inventory Management System!\n";

    // Input for Bubble Sort (Turnover rates)
    int nTurnover;
    cout << "Enter the number of products (for turnover rate sorting): ";
    cin >> nTurnover;

    vector<int> turnoverRates(nTurnover);
    cout << "Enter the turnover rates of products: ";
    for (int i = 0; i < nTurnover; i++)
        {
        cin >> turnoverRates[i];
    }

    bubbleSort(turnoverRates);
    cout << "Turnover rates sorted (using Bubble Sort): ";
    for (int rate : turnoverRates)
        {
        cout << rate << " ";
    }
    cout << "\n";

    // Input for Selection Sort (Product values)
    int nValues;
    cout << "Enter the number of products (for value sorting): ";
    cin >> nValues;

    vector<int> productValues(nValues);
    cout << "Enter the product values: ";
    for (int i = 0; i < nValues; i++)
        {
        cin >> productValues[i];
    }

    selectionSort(productValues);
    cout << "Product values sorted (using Selection Sort): ";
    for (int value : productValues)
        {
        cout << value << " ";
    }
    cout << "\n";

    // Input for Binary Search (Search for product stock levels)
    int nStock;
    cout << "Enter the number of products in stock (for searching): ";
    cin >> nStock;

    vector<int> stockLevels(nStock);
    cout << "Enter the stock levels of products: ";
    for (int i = 0; i < nStock; i++)
        {
        cin >> stockLevels[i];
    }

    selectionSort(stockLevels); // Ensure the stock levels are sorted
    cout << "Stock levels sorted (using Selection Sort): ";
    for (int stock : stockLevels)
        {
        cout << stock << " ";
    }
    cout << "\n";

    int searchKey;
    cout << "Enter the stock level to search (for reordering check): ";
    cin >> searchKey;

    int searchResult = binarySearch(stockLevels, searchKey);
    if (searchResult != -1)
        {
        cout << "Stock level " << searchKey << " found at position " << searchResult << ".\n";
    } else
     {
        cout << "Stock level " << searchKey << " not found (consider reordering).\n";
    }


}
// Graph to represent fraud detection
unordered_map<int, vector<int>> adj;

// Add a transaction (edge) between two users (nodes)
void addTransaction(int userA, int userB)
{
    adj[userA].push_back(userB);
}

// Remove a transaction (edge) between two users
void removeTransaction(int userA, int userB)
{
    auto& neighbors = adj[userA];
    neighbors.erase(remove(neighbors.begin(), neighbors.end(), userB), neighbors.end());
}

// Check if a transaction exists between two users
bool transactionExists(int userA, int userB)
 {
    auto& neighbors = adj[userA];
    return find(neighbors.begin(), neighbors.end(), userB) != neighbors.end();
}

// Display all transactions
void displayTransactions()
 {
    cout << "\nAll Transactions:\n";
    if (adj.empty())
        {
        cout << "No transactions available.\n";
    } else
    {
        for (const auto& pair : adj)
        {
            int userA = pair.first;
            for (int userB : pair.second)
             {
                cout << "User " << userA << " -> User " << userB << endl;
            }
        }
    }
}

// Detect cycle (possible money laundering activity)
bool detectCycleUtil(int node, unordered_set<int>& visited, unordered_set<int>& recStack)
{
    visited.insert(node);
    recStack.insert(node);

    for (int neighbor : adj[node])
        {
        if (recStack.find(neighbor) != recStack.end())
         {
            return true;  // Cycle detected
        }
        if (visited.find(neighbor) == visited.end() && detectCycleUtil(neighbor, visited, recStack))
         {
            return true;
        }
    }

    recStack.erase(node);
    return false;
}

bool detectCycle()
{
    unordered_set<int> visited, recStack;
    for (const auto& pair : adj)
        {
        int node = pair.first;
        if (visited.find(node) == visited.end())
        {
            if (detectCycleUtil(node, visited, recStack))
             {
                return true;  // Fraud detected due to cycle
            }
        }
    }
    return false;  // No cycle found
}

// Find central nodes with high centrality (suspects for fraud intermediaries)
void findCentralNodes()
{
    unordered_map<int, int> inDegree;

    // Calculate in-degree (number of incoming edges/transactions)
    for (const auto& pair : adj)
        {
        for (int neighbor : pair.second)
         {
            inDegree[neighbor]++;
        }
    }

    cout << "\nUsers with high centrality (more connections to/from them):\n";
    bool found = false;
    for (const auto& pair : inDegree)
        {
        if (pair.second > 2)
        {  // Arbitrary threshold for centrality (more than 2 connections)
            cout << "User " << pair.first << " has " << pair.second << " connections.\n";
            found = true;
        }
    }

    if (!found)
        {
        cout << "No users with high centrality found.\n";
    }
}

// Remove a user (node) from the graph
void removeUser(int user)
 {
    adj.erase(user);  // Remove all outgoing edges

    // Remove all incoming edges
    for (auto& pair : adj)
        {
        auto& neighbors = pair.second;
        neighbors.erase(remove(neighbors.begin(), neighbors.end(), user), neighbors.end());
    }

    cout << "User " << user << " and their transactions have been removed.\n";
}

// Show all users in the graph
void displayUsers()
{
    cout << "\nAll Users in the System:\n";
    for (const auto& pair : adj)
        {
        cout << "User " << pair.first << endl;
    }
}

// Get fraud statistics (number of users, transactions, etc.)
void displayStats()
{
    int totalTransactions = 0;
    for (const auto& pair : adj)
        {
        totalTransactions += pair.second.size();
    }

    cout << "\nSystem Statistics:\n";
    cout << "Total Users: " << adj.size() << endl;
    cout << "Total Transactions: " << totalTransactions << endl;
}

// Clear all data from the graph
void clearSystem()
 {
    adj.clear();
    cout << "\nAll data has been cleared from the system.\n";
}

void business_case5()
{
    // Adding predefined transactions
    addTransaction(1, 2);
    addTransaction(2, 3);
    addTransaction(3, 4);
    addTransaction(4, 5);
    addTransaction(5, 6);
    addTransaction(6, 1); // This creates a cycle (1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 1)

    int choice;

    while (true)
        {
        cout << "\nFraud Detection System - E-commerce\n";
        cout << "1. Add Transaction\n";
        cout << "2. Remove Transaction\n";
        cout << "3. Display All Transactions\n";
        cout << "4. Detect Cycles (Money Laundering)\n";
        cout << "5. Find Central Fraud Suspects\n";
        cout << "6. Remove User\n";
        cout << "7. Display All Users\n";
        cout << "8. Display System Statistics\n";
        cout << "9. Clear All Data\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
         {
            case 1:
                 {
                int userA, userB;
                cout << "Enter the users involved in the transaction (userA -> userB): ";
                cin >> userA >> userB;
                addTransaction(userA, userB);
                cout << "Transaction added between User " << userA << " and User " << userB << endl;
                break;
            }

            case 2:
                {
                int userA, userB;
                cout << "Displaying all transactions:\n";
                displayTransactions();  // Show all transactions first

                cout << "Enter the users involved in the transaction to remove (userA -> userB): ";
                cin >> userA >> userB;

                // Check if the transaction exists before removing
                if (transactionExists(userA, userB))
                    {
                    removeTransaction(userA, userB);  // Remove the specified transaction
                    cout << "Transaction removed between User " << userA << " and User " << userB << endl;
                } else
                {
                    cout << "Transaction between User " << userA << " and User " << userB << " does not exist.\n";
                }

                // Display updated transactions
                cout << "\nUpdated Transactions:\n";
                displayTransactions();
                break;
            }

            case 3:
                displayTransactions();
                break;

            case 4:
                if (detectCycle())
                    {
                    cout << "Fraud detected! Possible money laundering detected due to cycle.\n";
                } else
                {
                    cout << "No cycles detected. No suspicious activity.\n";
                }
                break;

            case 5:
                findCentralNodes();
                break;

            case 6:
                {
                int user;
                cout << "Displaying all transactions:\n";
                displayTransactions();  // Show all transactions first

                cout << "Enter the user ID to remove: ";
                cin >> user;

                // Remove the user and their transactions
                removeUser(user);

                // Display updated transactions after user removal
                cout << "\nUpdated Transactions:\n";
                displayTransactions();
                break;
            }

            case 7:
                displayUsers();
                break;

            case 8:
                displayStats();
                break;

            case 9:
                clearSystem();
                break;
            case 10:
                cout << "Exiting the system. Goodbye!\n";
                return;

            default:
                cout << "Invalid choice, please try again.\n";
        }
    }

}
void stringMatch(vector<string>& productNames, string keyword)
 {
    bool found = false;
    for (const auto& name : productNames)
        {
        if (name.find(keyword) != string::npos)
         { // Partial match
            cout << "Product found: " << name << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "No product found with the keyword \"" << keyword << "\".\n";
    }
}

// Function for Linear Search (search by category)
void linearSearch(vector<string>& categories, vector<string>& productNames, string category)
 {
    bool found = false;
    cout << "Products in category \"" << category << "\":\n";
    for (size_t i = 0; i < categories.size(); i++)
        {
        if (categories[i] == category)
         {
            cout << "- " << productNames[i] << endl;
            found = true;
        }
    }
    if (!found)
     {
        cout << "No products found in the category \"" << category << "\".\n";
    }
}

// Function for Binary Search (search by price)
int binarySearch(vector<int>& prices, int target)
 {
    int low = 0, high = prices.size() - 1;
    while (low <= high)
        {
        int mid = low + (high - low) / 2;
        if (prices[mid] == target)
        {
            return mid; // Found
        } else if (prices[mid] < target)
        {
            low = mid + 1;
        } else
        {
            high = mid - 1;
        }
    }
    return -1; // Not found
}

void business_case6()
{
    cout << "Welcome to the Product Catalog Management System!\n";

    // Input for Product Data
    int n;
    cout << "Enter the number of products: ";
    cin >> n;

    vector<string> productNames(n);
    vector<int> productPrices(n);
    vector<string> productCategories(n);

    cout << "Enter product details (name, price, category):\n";
    for (int i = 0; i < n; i++) {
        cin.ignore(); // Clear input buffer
        cout << "Product " << i + 1 << " Name: ";
        getline(cin, productNames[i]);
        cout << "Product " << i + 1 << " Price: ";
        cin >> productPrices[i];
        cin.ignore();
        cout << "Product " << i + 1 << " Category: ";
        getline(cin, productCategories[i]);
    }

    // String Matching (Search by Name/Description)
    string keyword;
    cout << "\nEnter a keyword to search for a product (by name/description): ";
    cin.ignore();
    getline(cin, keyword);
    stringMatch(productNames, keyword);

    // Linear Search (Search by Category)
    string category;
    cout << "\nEnter a category to list all products: ";
    getline(cin, category);
    linearSearch(productCategories, productNames, category);

    // Binary Search (Search by Price)
    sort(productPrices.begin(), productPrices.end()); // Sort prices for Binary Search
    cout << "\nSorted Prices: ";
    for (int price : productPrices)
        {
        cout << price << " ";
    }
    cout << "\n";

    int targetPrice;
    cout << "Enter a price to search for a product: ";
    cin >> targetPrice;
    int priceIndex = binarySearch(productPrices, targetPrice);
    if (priceIndex != -1)
        {
        cout << "Product with price " << targetPrice << " found in the catalog.\n";
    } else
    {
        cout << "No product found with the price " << targetPrice << ".\n";
    }

}

int main()
 {
    int choice;

    // Display the menu
   while(true)
    {
    cout << "Select an option:\n";
    cout << "1) order delivery optimization\n";
    cout << "2) Dynamic Pricing Optimization\n";
    cout << "3) Customer Segmentation\n";
    cout << "4) Efficient Inventory Management\n";
    cout << "5) Fraud Detection\n";
    cout << "6) Product Catalog Management\n";
    cout << "Enter your choice: ";
    cin >> choice;

    // Process the choice using switch-case

    switch (choice)
    {
        case 1:
            business_case1();
            break;

        case 2:
            business_case2();

            break;

        case 3:
            business_case3();
            break;

        case 4:
            business_case4();
            break;

        case 5:
            business_case5();
            break;

        case 6:
            business_case6();
            break;

        case 7:
            exit(0);

        default:
            cout << "Invalid choice.Please select a valid option (1-4).\n";
            break;
    }

    }

}
