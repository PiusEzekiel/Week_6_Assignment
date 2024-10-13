#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Compile the Code: 
gcc social_network_system.c -o social_network

Run the Program:
./social_network
*/

#define MAX_USERS 100
#define MAX_NAME_LENGTH 50

// ANSI color codes
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"



// Function declaration for printLine
void printLine(char ch, int length);


// Structure to represent a graph node
typedef struct Node {
    char name[MAX_NAME_LENGTH];
    struct Node* next;
} Node;

// Structure to represent a graph
typedef struct Graph {
    Node* adjacencyList[MAX_USERS];
    int numUsers;
} Graph;

// Function to create a new graph
Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->numUsers = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        graph->adjacencyList[i] = NULL;
    }
    return graph;
}

// Function to add a user
void addUser(Graph* graph, char* name) {
    if (graph->numUsers < MAX_USERS) {
        Node* newUser = (Node*)malloc(sizeof(Node));
        strcpy(newUser->name, name);
        newUser->next = graph->adjacencyList[graph->numUsers];
        graph->adjacencyList[graph->numUsers] = newUser;
        graph->numUsers++;
    }
}

// Function to add a connection (friendship)
void addConnection(Graph* graph, char* user1, char* user2) {
    int user1Index = -1, user2Index = -1;
    
    // Find indices of user1 and user2
    for (int i = 0; i < graph->numUsers; i++) {
        if (strcmp(graph->adjacencyList[i]->name, user1) == 0) {
            user1Index = i;
        }
        if (strcmp(graph->adjacencyList[i]->name, user2) == 0) {
            user2Index = i;
        }
    }

    // Ensure both users exist before establishing a connection
    if (user1Index != -1 && user2Index != -1) {
        Node* newFriend = (Node*)malloc(sizeof(Node));
        strcpy(newFriend->name, user2);
        newFriend->next = graph->adjacencyList[user1Index]->next;
        graph->adjacencyList[user1Index]->next = newFriend;
        
        // Add the reverse connection
        newFriend = (Node*)malloc(sizeof(Node));
        strcpy(newFriend->name, user1);
        newFriend->next = graph->adjacencyList[user2Index]->next;
        graph->adjacencyList[user2Index]->next = newFriend;

        printf(GREEN "Connection established between %s and %s.\n" RESET, user1, user2);
    } else {
        printf(RED "Error: Both users must exist to establish a connection.\n" RESET);
    }
}

// DFS function to find recommendations
// Revised DFS function for finding friend recommendations
void dfs(Graph* graph, int index, int* visited, char recommendations[][MAX_NAME_LENGTH], int* recCount, char* user) {
    Node* current = graph->adjacencyList[index]->next;

    while (current != NULL) {
        // Check if the current friend is the user or already visited
        if (strcmp(current->name, user) != 0 && !visited[index]) {
            // Check if the recommendation is already present
            int found = 0;
            for (int j = 0; j < *recCount; j++) {
                if (strcmp(recommendations[j], current->name) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(recommendations[*recCount], current->name);
                (*recCount)++;
            }
        }
        current = current->next;
    }
}

// Function to get friend recommendations
void getRecommendations(Graph* graph, char* user) {
    int userIndex = -1;

    // Check if the user exists
    for (int i = 0; i < graph->numUsers; i++) {
        if (strcmp(graph->adjacencyList[i]->name, user) == 0) {
            userIndex = i;
            break;
        }
    }

    if (userIndex == -1) {
        printf(RED "Error: User %s not found.\n" RESET, user);
        return;
    }

    int visited[MAX_USERS] = {0};
    char recommendations[MAX_USERS][MAX_NAME_LENGTH];
    int recCount = 0;

    // Mark direct friends as visited and perform DFS
    Node* current = graph->adjacencyList[userIndex]->next;

    // Mark the user as visited
    visited[userIndex] = 1;

    while (current != NULL) {
        int friendIndex = -1;

        // Find the friend index
        for (int i = 0; i < graph->numUsers; i++) {
            if (strcmp(graph->adjacencyList[i]->name, current->name) == 0) {
                friendIndex = i;
                break;
            }
        }

        // Perform DFS from each friend
        if (friendIndex != -1) {
            dfs(graph, friendIndex, visited, recommendations, &recCount, user);
        }

        current = current->next;
    }

    // Print recommendations
    printf(YELLOW "Friend recommendations for %s:\n" RESET, user);
    if (recCount == 0) {
        printf(RED "None\n" RESET);
    } else {
        for (int i = 0; i < recCount; i++) {
            printf(GREEN "%s\n" RESET, recommendations[i]);
        }
    }
}
// Function to display the adjacency matrix
void displayAdjacencyMatrix(Graph* graph) {
    printf("\n" MAGENTA "Adjacency Matrix:\n" RESET);
    printf("        ");
    for (int i = 0; i < graph->numUsers; i++) {
        printf(YELLOW "%-10s" RESET, graph->adjacencyList[i]->name);
    }
    printf("\n");

    for (int i = 0; i < graph->numUsers; i++) {
        printf(CYAN "%-10s" RESET, graph->adjacencyList[i]->name);
        for (int j = 0; j < graph->numUsers; j++) {
            int found = 0;
            Node* current = graph->adjacencyList[i]->next;
            // Check for connection
            while (current != NULL) {
                if (strcmp(current->name, graph->adjacencyList[j]->name) == 0) {
                    found = 1;
                    break;
                }
                current = current->next;
            }

            // Print the connection status
            if (found) {
                printf(GREEN "%-10d" RESET, 1); // Connection exists
            } else {
                printf(RED "%-10d" RESET, 0);   // No connection
            }
        }
        printf("\n");
    }
}

// Function to display connections
void displayConnections(Graph* graph) {
    printf("\n" MAGENTA "Connections:\n" RESET);
    for (int i = 0; i < graph->numUsers; i++) {
        Node* current = graph->adjacencyList[i];
        printf(YELLOW "%s:" RESET, current->name);
        
        current = current->next; // Move to the first friend
        if (current == NULL) {
            printf(RED " No connections\n" RESET); // No connections
        } else {
            printf(" ");
            while (current != NULL) {
                printf(GREEN "%s" RESET, current->name); // Print friends in green
                current = current->next;
                if (current != NULL) {
                    printf(", "); // Separate friends with a comma
                }
            }
            printf("\n");
        }
    }
}

// Function to display the menu and get user input
void displayMenu() {
    printLine('-', 50);
    printf("\n" BOLD "Social Network System Menu\n" RESET);
    printLine('*', 50);
    printf("1. Display Adjacency Matrix\n");
    printf("2. Display Connections\n");
    printf("3. Get Friend Recommendations\n");
    printf("4. Add Connection\n");
    printf("5. Add New User\n");
    printf("6. Exit\n");
    printf(BLUE "Select an option: " RESET);
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
    Graph* graph = createGraph();

    // Adding initial users
    addUser(graph, "Alice");
    addUser(graph, "Bob");
    addUser(graph, "Charlie");
    addUser(graph, "David");
    addUser(graph, "Eve");

    // Establishing initial connections
    addConnection(graph, "Alice", "Bob");
    addConnection(graph, "Alice", "Charlie");
    addConnection(graph, "Bob", "David");
    addConnection(graph, "David", "Eve");

    int choice;
    char user1[MAX_NAME_LENGTH], user2[MAX_NAME_LENGTH], user[MAX_NAME_LENGTH];

    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayAdjacencyMatrix(graph);
                break;
            case 2:
                displayConnections(graph);
                break;
            case 3:
                printf("\n" BLUE "Enter the name of the person to get friend recommendations: " RESET);
                scanf("%s", user);
                getRecommendations(graph, user);
                break;
            case 4:
                printf("\n" BLUE "Enter the name of the first user: " RESET);
                scanf("%s", user1);
                printf(BLUE "Enter the name of the second user: " RESET);
                scanf("%s", user2);
                addConnection(graph, user1, user2);
                break;
            case 5:
                printf("\n" BLUE "Enter the name of the new user: " RESET);
                scanf("%s", user1);
                addUser(graph, user1);
                printf(GREEN "User %s added successfully.\n" RESET, user1);
                break;
            case 6:
                printf(RED "Exiting...\n" RESET);
                // Free allocated memory
                for (int i = 0; i < graph->numUsers; i++) {
                    Node* current = graph->adjacencyList[i];
                    while (current != NULL) {
                        Node* temp = current;
                        current = current->next;
                        free(temp);
                    }
                }
                free(graph);
                exit(0);
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
                break;
        }
    }

    return 0;
}