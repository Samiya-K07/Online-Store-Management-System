#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <limits>
using namespace std;

// =============================
//    CITY GRAPH CONFIGURATION
// =============================
#define NUM_CITIES 3
#define INF 99999

string cityNamesList[NUM_CITIES] =
    {
        "Karachi",
        "Lahore",
        "Islamabad"};

int cityGraph[NUM_CITIES][NUM_CITIES] =
    {
        {0, 10, 15}, // Karachi -> Lahore, Islamabad
        {10, 0, 12}, // Lahore -> Karachi, Islamabad
        {15, 12, 0}  // Islamabad -> Karachi, Lahore
};

// =============================
//    CITY NETWORK
// =============================
class CityNetwork
{
public:
    string cityNames[NUM_CITIES];
    int cityGraph[NUM_CITIES][NUM_CITIES];

    CityNetwork(int graph[NUM_CITIES][NUM_CITIES], string names[])
    {
        for (int i = 0; i < NUM_CITIES; i++)
        {
            cityNames[i] = names[i];
            for (int j = 0; j < NUM_CITIES; j++)
            {
                cityGraph[i][j] = graph[i][j];
            }
        }
    }

    int minDistance(int dist[], bool visited[])
    {
        int min = INF, minIndex = -1;
        for (int i = 0; i < NUM_CITIES; i++)
        {
            if (!visited[i] && dist[i] <= min)
            {
                min = dist[i];
                minIndex = i;
            }
        }
        return minIndex;
    }

    int computeShortestDistance(int src, int dest)
    {
        int dist[NUM_CITIES];
        bool visited[NUM_CITIES];

        for (int i = 0; i < NUM_CITIES; i++)
        {
            dist[i] = INF;
            visited[i] = false;
        }

        dist[src] = 0;

        for (int count = 0; count < NUM_CITIES - 1; count++)
        {
            int u = minDistance(dist, visited);
            if (u == -1)
            {
                break;
            }
            visited[u] = true;
            for (int v = 0; v < NUM_CITIES; v++)
            {
                if (!visited[v] &&
                    cityGraph[u][v] != 0 &&
                    dist[u] != INF &&
                    dist[u] + cityGraph[u][v] < dist[v])
                {
                    dist[v] = dist[u] + cityGraph[u][v];
                }
            }
        }
        return dist[dest];
    }
};

// =============================
//    ORDER CLASS (Data Payload)
// =============================
class Order
{
public:
    string customerName;
    string itemName;
    int itemCount;
    double totalCost;
    int orderID;

    Order() : customerName(""), itemName(""), itemCount(0), totalCost(0.0), orderID(0) {} // Default constructor with safe values

    Order(string name, string item, int count, double cost, int id)
    {
        customerName = name;
        itemName = item;
        itemCount = count;
        totalCost = cost;
        orderID = id;
    }
};

// =============================
//    DELIVERY NODE (Linked List for active deliveries)
// =============================
class DeliveryNode
{
public:
    Order orderInfo;
    string deliveryAddress;
    double deliveryFee;
    int distanceToLocation;
    DeliveryNode *nextNode;

    DeliveryNode(string name, string item, int count, double cost, string address, double fee, int dist, int id)
    {
        orderInfo = Order(name, item, count, cost, id);
        deliveryAddress = address;
        deliveryFee = fee;
        distanceToLocation = dist;
        nextNode = NULL;
    }
};

// =============================
//    COMPLETED ORDER LIST
// =============================
class CompletedOrderNode
{
public:
    Order orderInfo;
    CompletedOrderNode *next;

    CompletedOrderNode(Order order)
    {
        orderInfo = order;
        next = NULL;
    }
};

// =============================
//    ELECTRONICS STORE CLASS
// =============================
class ElectronicsStore
{
public:
    string storeName;
    string *itemList;
    int *itemPrices;
    string storeAddress;
    DeliveryNode *deliveryHead;
    CompletedOrderNode *completedHead; 

    ElectronicsStore()
    {
        deliveryHead = NULL;
        completedHead = NULL; 
        itemList = NULL;
        itemPrices = NULL;
    }

    void addDeliveryOrder(string name, string item, int count, double cost, string address, double fee, int dist, int id, bool silent = false)
    {
        DeliveryNode *newNode = new DeliveryNode(name, item, count, cost, address, fee, dist, id);

        if (deliveryHead == NULL)
        {
            deliveryHead = newNode;
        }
        
        else
        {
            DeliveryNode *temp = deliveryHead;
            while (temp->nextNode != NULL)
            {
                temp = temp->nextNode;
            }
            
            temp->nextNode = newNode;
        }

        if (!silent)
        {
            cout << endl << "****** Delivery Order Placed ******" << endl;
            cout << "Order ID: " << id << endl;
            cout << "Customer: " << name << endl;
            cout << "Bill: Rs." << cost << endl;
            cout << "Delivery Fee: Rs." << fee << endl;
        }
    }

    void displayDeliveryOrders()
    {
        if (deliveryHead == NULL)
        {
            cout << "No delivery orders in the queue." << endl;
            return;
        }

        DeliveryNode *temp = deliveryHead;
        cout << endl << "===== ACTIVE DELIVERY ORDERS =====" << endl;

        while (temp != NULL)
        {
            cout << "------------------------------------------" << endl;
            cout << "ID: " << temp->orderInfo.orderID << " | Customer: " << temp->orderInfo.customerName << endl;
            cout << "Item: " << temp->orderInfo.itemName << endl;
            cout << "Address: " << temp->deliveryAddress << endl;
            cout << "------------------------------------------" << endl;
            temp = temp->nextNode;
        }
    }

    Order processNextDelivery()
    {
        if (deliveryHead == NULL)
        {
            return Order(); 
        }

        DeliveryNode *deliveredOrder = deliveryHead;
        Order completedOrderData = deliveredOrder->orderInfo; 

        cout << endl << "****** Delivery Completed ******" << endl;
        cout << "Order ID: " << completedOrderData.orderID << endl;
        cout << "Customer: " << completedOrderData.customerName << endl;
        cout << "--------------------------------" << endl;

        deliveryHead = deliveredOrder->nextNode;

        delete deliveredOrder;

        return completedOrderData; 
    }

    void archiveOrder(Order completedOrder)
    {
        CompletedOrderNode *newNode = new CompletedOrderNode(completedOrder);

        if (completedHead == NULL)
        {
            completedHead = newNode;
        }
        
        else
        {
            CompletedOrderNode *temp = completedHead;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            
            temp->next = newNode;
        }
    }

    void displayCompletedOrders()
    {
        if (completedHead == NULL)
        {
            cout << "The completed order list is empty." << endl;
            return;
        }
        
        cout << endl << "===== COMPLETED ORDER HISTORY (Chronological Order) =====" << endl;
        CompletedOrderNode *temp = completedHead;
        
        while (temp != NULL)
        {
            cout << "------------------------------------------" << endl;
            cout << "ID: " << temp->orderInfo.orderID << " | Customer: " << temp->orderInfo.customerName << endl;
            cout << "Item: " << temp->orderInfo.itemName << " (x" << temp->orderInfo.itemCount << ")" << endl;
            cout << "Total Bill: Rs." << temp->orderInfo.totalCost << endl;
            cout << "------------------------------------------" << endl;
            temp = temp->next;
        }
    }

    ~ElectronicsStore()
    {
        DeliveryNode *temp = deliveryHead;
        while (temp != NULL)
        {
            DeliveryNode *del = temp;
            temp = temp->nextNode;
            delete del;
        }

        CompletedOrderNode *compTemp = completedHead;
        while (compTemp != NULL)
        {
            CompletedOrderNode *del = compTemp;
            compTemp = compTemp->next;
            delete del;
        }

        delete[] itemList;
        delete[] itemPrices;
    }
};

// =============================
//    AVL TREE NODE (For Pickup/All Orders)
// =============================
class OrderNode
{
public:
    string buyerName;
    string itemName;
    int itemCount;
    double totalCost;
    int orderNumber;

    OrderNode *leftBranch;
    OrderNode *rightBranch;

    OrderNode(string bName, string iName, int count, double cost, int oNum)
    {
        buyerName = bName;
        itemName = iName;
        itemCount = count;
        totalCost = cost;
        orderNumber = oNum;
        leftBranch = NULL;
        rightBranch = NULL;
    }
};

// =============================
//    AVL UTILITY
// =============================
int getHeight(OrderNode *ref)
{
    if (!ref)
    {
        return -1;
    }

    int leftH = getHeight(ref->leftBranch);
    int rightH = getHeight(ref->rightBranch);
    return max(leftH, rightH) + 1;
}

int getBalance(OrderNode *ref)
{
    if (!ref)
    {
        return 0;
    }

    return getHeight(ref->leftBranch) - getHeight(ref->rightBranch);
}

// =============================
//    AVL ROTATIONS
// =============================
OrderNode *rotateLL(OrderNode *ref)
{
    OrderNode *newRoot = ref->leftBranch;
    ref->leftBranch = newRoot->rightBranch;
    newRoot->rightBranch = ref;
    return newRoot;
}

OrderNode *rotateRR(OrderNode *ref)
{
    OrderNode *newRoot = ref->rightBranch;
    ref->rightBranch = newRoot->leftBranch;
    newRoot->leftBranch = ref;
    return newRoot;
}

OrderNode *rotateLR(OrderNode *ref)
{
    ref->leftBranch = rotateRR(ref->leftBranch);
    return rotateLL(ref);
}

OrderNode *rotateRL(OrderNode *ref)
{
    ref->rightBranch = rotateLL(ref->rightBranch);
    return rotateRR(ref);
}

// =============================
//    AVL INSERTION
// =============================
OrderNode *addOrder(OrderNode *ref, string buyer, string item, int count, double cost, int orderNo)
{
    if (!ref)
    {
        return new OrderNode(buyer, item, count, cost, orderNo);
    }

    if (orderNo < ref->orderNumber)
    {
        ref->leftBranch = addOrder(ref->leftBranch, buyer, item, count, cost, orderNo);
    }
    
    else if (orderNo > ref->orderNumber)
    {
        ref->rightBranch = addOrder(ref->rightBranch, buyer, item, count, cost, orderNo);
    }
    
    else 
    {
        return ref;
    }

    int bf = getBalance(ref);

    if (bf == 2)
    {
        return (orderNo < ref->leftBranch->orderNumber) ? rotateLL(ref) : rotateLR(ref);
    }

    if (bf == -2)
    {
        return (orderNo > ref->rightBranch->orderNumber) ? rotateRR(ref) : rotateRL(ref);
    }

    return ref;
}

// =============================
//    AVL SEARCH
// =============================
OrderNode *findOrder(OrderNode *ref, int orderNo)
{
    if (!ref)
    {
        return NULL;
    }
    
    if (orderNo == ref->orderNumber)
    {
        return ref;
    }
    
    if (orderNo < ref->orderNumber)
    {
        return findOrder(ref->leftBranch, orderNo);
    }

    return findOrder(ref->rightBranch, orderNo);
}

// =============================
//    AVL LINEAR SEARCH (For Benchmarking)
// =============================
OrderNode *linearSearchAVL(OrderNode *ref, int orderNo)
{
    if (!ref)
        return NULL;

    if (ref->orderNumber == orderNo)
        return ref;

    OrderNode *res = linearSearchAVL(ref->leftBranch, orderNo);
    if (res != NULL)
        return res;

    return linearSearchAVL(ref->rightBranch, orderNo);
}

// =============================
//    AVL DELETE
// =============================
OrderNode *findMin(OrderNode *ref)
{
    while (ref->leftBranch)
    {
        ref = ref->leftBranch;
    }
    return ref;
}

OrderNode *removeOrder(OrderNode *ref, int orderNo)
{
    if (!ref)
        return NULL;

    if (orderNo < ref->orderNumber)
    {
        ref->leftBranch = removeOrder(ref->leftBranch, orderNo);
    }
    
    else if (orderNo > ref->orderNumber)
    {
        ref->rightBranch = removeOrder(ref->rightBranch, orderNo);
    }
    
    else
    {
        if (!ref->leftBranch || !ref->rightBranch)
        {
            OrderNode *temp = (ref->leftBranch) ? ref->leftBranch : ref->rightBranch;
            delete ref;
            return temp;
        }
        OrderNode *succ = findMin(ref->rightBranch);
        ref->buyerName = succ->buyerName;
        ref->itemName = succ->itemName;
        ref->itemCount = succ->itemCount;
        ref->totalCost = succ->totalCost;
        ref->orderNumber = succ->orderNumber;
        ref->rightBranch = removeOrder(ref->rightBranch, succ->orderNumber);
    }

    if (!ref)
        return ref;

    int bf = getBalance(ref);
    
    if (bf > 1 && getBalance(ref->leftBranch) >= 0)
    {
        return rotateLL(ref);
    }
    
    if (bf > 1 && getBalance(ref->leftBranch) < 0)
    {
        return rotateLR(ref);
    }
    
    if (bf < -1 && getBalance(ref->rightBranch) <= 0)
    {
        return rotateRR(ref);
    }
    
    if (bf < -1 && getBalance(ref->rightBranch) > 0)
    {
        return rotateRL(ref);
    }

    return ref;
}

// =============================
//    DISPLAY
// =============================
void printOrder(OrderNode *ref)
{
    cout << endl << "------------------------------------------" << endl;
    cout << "Order No: " << ref->orderNumber << endl;
    cout << "Customer: " << ref->buyerName << endl;
    cout << "Item: " << ref->itemName << endl;
    cout << "Bill: Rs." << ref->totalCost << endl;
    cout << "------------------------------------------" << endl;
}

void showAllOrders(OrderNode *ref)
{
    if (!ref)
        return;
    
    showAllOrders(ref->leftBranch);
    printOrder(ref);
    showAllOrders(ref->rightBranch);
}

// =============================
//    GLOBAL DATA
// =============================

ElectronicsStore *storePtr = nullptr;
OrderNode *orderTreeRoot = nullptr;
CityNetwork *network = nullptr;
int globalNextOrderId = 1001; // Global ID tracker

// =============================
//    HELPER FUNCTIONS
// =============================

int selectCity(string prompt)
{
    int choice;
    cout << endl << prompt << endl;
    
    for (int i = 0; i < NUM_CITIES; i++)
    {
        cout << i + 1 << ". " << cityNamesList[i] << endl;
    }
    cout << "Enter choice (1-" << NUM_CITIES << "): ";
    cin >> choice;
    return choice - 1;
}

void initializeStore()
{
    storePtr = new ElectronicsStore();
    storePtr->storeName = "TechLogix Electronics";
    storePtr->storeAddress = "Saddar, Karachi";

    storePtr->itemList = new string[11];
    storePtr->itemPrices = new int[11];

    storePtr->itemList[1] = "Dell XPS 15 Laptop";
    storePtr->itemPrices[1] = 285000;
    storePtr->itemList[2] = "MacBook Pro 16";
    storePtr->itemPrices[2] = 450000;
    storePtr->itemList[3] = "Samsung S24 Ultra";
    storePtr->itemPrices[3] = 350000;
    storePtr->itemList[4] = "iPhone 15 Pro Max";
    storePtr->itemPrices[4] = 420000;
    storePtr->itemList[5] = "iPad Pro 12.9";
    storePtr->itemPrices[5] = 180000;
    storePtr->itemList[6] = "Samsung QLED 65 TV";
    storePtr->itemPrices[6] = 275000;
    storePtr->itemList[7] = "LG OLED 55 TV";
    storePtr->itemPrices[7] = 320000;
    storePtr->itemList[8] = "Sony WH-1000XM5";
    storePtr->itemPrices[8] = 65000;
    storePtr->itemList[9] = "Apple Watch S9";
    storePtr->itemPrices[9] = 125000;
    storePtr->itemList[10] = "HP Gaming Laptop";
    storePtr->itemPrices[10] = 195000;

    network = new CityNetwork(cityGraph, cityNamesList);
}

void cleanupSystem()
{
    delete storePtr;
    delete network;
}

void displayProducts()
{
    cout << endl << "============================================" << endl;
    cout << " NO. | PRODUCT NAME              | PRICE (PKR)" << endl;
    cout << "============================================" << endl;
    for (int i = 1; i <= 10; i++)
    {
        cout << "  " << i << "  | " << storePtr->itemList[i] << "\t | " << storePtr->itemPrices[i] << endl;
    }
}

int calculateDeliveryFee(int distance)
{
    int baseFee = 150;
    return (distance > 10) ? baseFee + (distance - 10) * 25 : baseFee + distance * 15;
}

void displayMenu()
{
    cout << "1. View Products" << endl;
    cout << "2. Place Pickup Order" << endl;
    cout << "3. Place Delivery Order" << endl;
    cout << "4. Perform Delivery (Process Queue)" << endl;
    cout << "5. Collect Pickup Order" << endl;
    cout << "6. View All Active Delivery Orders" << endl;
    cout << "7. View All Pending Pickup Orders" << endl;
    cout << "8. View Completed Orders History" << endl; 
    cout << "9. Load Data & Test Speed" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

// =============================
//    BENCHMARKING FUNCTIONS
// =============================
void loadBenchmarks(string filename)
{
    ifstream file(filename.c_str());
    if (!file.is_open())
    {
        cout << "Could not open " << filename << ". Please create a dummy file with 500 records." << endl;
        return;
    }

    string line;
    int count = 0;
    cout << endl << "Loading 500 records into AVL Tree and Linked List..." << endl;

    clock_t start = clock();

    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr, name, item, qtyStr, costStr, distStr, addr;

        if (getline(ss, idStr, ',') &&
            getline(ss, name, ',') &&
            getline(ss, item, ',') &&
            getline(ss, qtyStr, ',') &&
            getline(ss, costStr, ',') &&
            getline(ss, distStr, ',') &&
            getline(ss, addr, ','))
        {
            int id = stoi(idStr);
            int qty = stoi(qtyStr);
            double cost = stod(costStr);
            int dist = stoi(distStr);

            orderTreeRoot = addOrder(orderTreeRoot, name, item, qty, cost, id);

            storePtr->addDeliveryOrder(name, item, qty, cost, addr, 0, dist, id, true);

            count++;
            globalNextOrderId = id + 1;
        }
    }

    clock_t end = clock();
    double duration = double(end - start) / CLOCKS_PER_SEC * 1000;
    cout << "Loaded " << count << " records in " << duration << " ms." << endl;
    file.close();
}

void runPerformanceTest()
{
    if (!orderTreeRoot)
    {
        cout << "Please Load Data first (Option 9)." << endl;
        return;
    }

    int targetID = globalNextOrderId - 1; // Pick a valid ID (last inserted)
    cout << endl << "===== BENCHMARK RESULTS (N=500) =====" << endl;
    cout << "Comparing Search Algorithms on the SAME Data Structure (AVL Tree)" << endl;
    cout << "Searching for ID: " << targetID << endl;
    cout << "Running 100,000 iterations for accuracy." << endl;

    // 1. AVL Standard Search (Logarithmic)
    clock_t startAVL = clock();
    for (int i = 0; i < 100000; i++)
    {
        findOrder(orderTreeRoot, targetID);
    }
    clock_t endAVL = clock();
    double timeAVL = double(endAVL - startAVL) / CLOCKS_PER_SEC * 1000;

    // 2. Linear Search on AVL (Linear)
    clock_t startLinear = clock();
    for (int i = 0; i < 100000; i++)
    {
        linearSearchAVL(orderTreeRoot, targetID);
    }
    clock_t endLinear = clock();
    double timeLinear = double(endLinear - startLinear) / CLOCKS_PER_SEC * 1000;

    cout << endl << "-----------------------------------------------------" << endl;
    cout << "ALGORITHM\t\t| TIME (ms)\t| COMPLEXITY" << endl;
    cout << "-----------------------------------------------------" << endl;
    cout << "AVL Tree Search\t\t| " << timeAVL << " ms\t| O(log n)" << endl;
    cout << "Linear Search (Brute)\t| " << timeLinear << " ms\t| O(n)" << endl;
    cout << "-----------------------------------------------------" << endl;

    if (timeLinear > timeAVL)
        cout << "VERDICT: AVL Search is " << (timeLinear / timeAVL) << "x faster than Linear Search." << endl;
    
    else
        cout << "VERDICT: Dataset too small to show significant difference." << endl;
}

// =============================
//    MAIN FUNCTION (Control Flow)
// =============================

int main()
{
    initializeStore();
    int choice = -1;

    do
    {
        cout << endl << endl << "************************************************" << endl;
        cout << "          " << storePtr->storeName << endl;
        cout << "************************************************" << endl;

        displayMenu();
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        string customerName, fullAddress;
        int quantity, itemNumber, distance, orderNumber;
        double totalBill, deliveryFee;
        Order archivedOrder;

        switch (choice)
        {
        case 1: 
        {
            displayProducts();
            break;
        }

        case 2: 
        {
            cout << endl << "--- PLACE PICKUP ORDER ---" << endl;
            cout << "Name: ";
            cin.ignore();
            getline(cin, customerName);
            cout << "Product (1-10): ";
            cin >> itemNumber;
            cout << "Quantity: ";
            cin >> quantity;

            orderNumber = globalNextOrderId++;
            totalBill = quantity * storePtr->itemPrices[itemNumber];

            // Insert into AVL Tree
            orderTreeRoot = addOrder(orderTreeRoot, customerName, storePtr->itemList[itemNumber], quantity, totalBill, orderNumber);
            cout << "Order Placed! ID: #" << orderNumber << endl;
            break;
        }

        case 3: // Place Delivery Order
        {
            cout << endl << "---- PLACE DELIVERY ORDER ----" << endl;
            int srcCity = 0; // Assuming store is located in Karachi (Index 0)
            int destCity = selectCity("Select Delivery City:");

            cout << "Name: ";
            cin.ignore();
            getline(cin, customerName);
            cout << "Product (1-10): ";
            cin >> itemNumber;
            cout << "Quantity: ";
            cin >> quantity;

            distance = network->computeShortestDistance(srcCity, destCity);
            deliveryFee = calculateDeliveryFee(distance);
            totalBill = (quantity * storePtr->itemPrices[itemNumber]) + deliveryFee;
            fullAddress = cityNamesList[destCity];
            orderNumber = globalNextOrderId++;

            storePtr->addDeliveryOrder(customerName, storePtr->itemList[itemNumber], quantity, totalBill, fullAddress, deliveryFee, distance, orderNumber);

            cout << "Distance: " << distance << "km | Fee: " << deliveryFee << endl;
            break;
        }

        case 4: 
        {
            cout << endl << "--- PERFORM NEXT DELIVERY ---" << endl;

            archivedOrder = storePtr->processNextDelivery();

            if (archivedOrder.orderID != 0)
            {
                storePtr->archiveOrder(archivedOrder);
            }
            else
            {
                cout << "Delivery queue is empty. No orders to deliver." << endl;
            }
            break;
        }

        case 5: 
        {
            cout << "Enter Order ID to Collect: ";
            cin >> orderNumber;

            OrderNode *foundOrder = findOrder(orderTreeRoot, orderNumber);

            if (!foundOrder)
            {
                cout << "Order not found in pending pickup orders!" << endl;
            }
            
            else
            {
                Order collectedOrder(foundOrder->buyerName, foundOrder->itemName, foundOrder->itemCount, foundOrder->totalCost, foundOrder->orderNumber);
                storePtr->archiveOrder(collectedOrder);

                orderTreeRoot = removeOrder(orderTreeRoot, orderNumber);
                cout << endl << "Order #" << orderNumber << " has been collected!" << endl;
            }
            break;
        }

        case 6: 
        {
            storePtr->displayDeliveryOrders();
            break;
        }

        case 7: // AVL In-Order Traversal
        {
            if (!orderTreeRoot)
            {
                cout << "No pending pickup orders." << endl;
            }
            
            else
            {
                cout << endl << "===== PENDING PICKUP ORDERS =====" << endl;
                showAllOrders(orderTreeRoot);
            }
            
            break;
        }

        case 8: 
        {
            storePtr->displayCompletedOrders();
            break;
        }

        case 9: 
        {
            loadBenchmarks("orders_data.txt");
            runPerformanceTest();
            break;
        }

        case 0: 
        {
            cout << "Exiting... Thank you for using " << storePtr->storeName << "!~" << endl;
            break;
        }

        default:
        {
            cout << "Invalid Option!! Please try again." << endl;
        }
        }
    } while (choice != 0);

    cleanupSystem();
    return 0;
}
