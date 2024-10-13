#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Compile the Code: 
gcc patient_management_system.c -o patient_management

Run the Program:
./patient_management
*/


#define MAX_PATIENTS 100

// ANSI color codes for styling
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define MAGENTA "\033[35m"

// Patient structure
typedef struct {
    char name[50];
    int severity; // Severity scale from 0 (min) to 10 (max)
} Patient;

// Max-Heap structure
typedef struct {
    Patient patients[MAX_PATIENTS];
    int size;
} MaxHeap;

// Function prototypes
void initHeap(MaxHeap* heap);
void insert(MaxHeap* heap, const char* name, int severity);
Patient extractMax(MaxHeap* heap);
void heapify(MaxHeap* heap, int index);
void updateSeverity(MaxHeap* heap, const char* name, int newSeverity);
void displayPatients(MaxHeap* heap);
void swap(Patient* a, Patient* b);
void printLine(char ch, int length);
void interactiveMenu(MaxHeap* heap);
int getSeverityInput();

int main() {
    MaxHeap heap;
    initHeap(&heap);
    interactiveMenu(&heap);
    return 0;
}

// Initialize the heap
void initHeap(MaxHeap* heap) {
    heap->size = 0;
}

// Insert a patient into the max-heap
void insert(MaxHeap* heap, const char* name, int severity) {
    if (heap->size >= MAX_PATIENTS) {
        printf(RED "Heap is full!\n" RESET);
        return;
    }
    Patient newPatient;
    strcpy(newPatient.name, name);
    newPatient.severity = severity;

    heap->patients[heap->size] = newPatient;
    heap->size++;

    // Heapify upwards
    int index = heap->size - 1;
    while (index != 0 && heap->patients[(index - 1) / 2].severity < heap->patients[index].severity) {
        swap(&heap->patients[index], &heap->patients[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

// Extract the patient with the highest severity
Patient extractMax(MaxHeap* heap) {
    if (heap->size == 0) {
        printf(RED "Heap is empty!\n" RESET);
        exit(EXIT_FAILURE); // or handle it appropriately
    }
    Patient maxPatient = heap->patients[0];
    heap->patients[0] = heap->patients[heap->size - 1];
    heap->size--;
    heapify(heap, 0);
    return maxPatient;
}

// Heapify operation to maintain max-heap property
void heapify(MaxHeap* heap, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->patients[left].severity > heap->patients[largest].severity) {
        largest = left;
    }
    if (right < heap->size && heap->patients[right].severity > heap->patients[largest].severity) {
        largest = right;
    }
    if (largest != index) {
        swap(&heap->patients[index], &heap->patients[largest]);
        heapify(heap, largest);
    }
}

// Update severity of a patient and re-heapify
void updateSeverity(MaxHeap* heap, const char* name, int newSeverity) {
    for (int i = 0; i < heap->size; i++) {
        if (strcmp(heap->patients[i].name, name) == 0) {
            heap->patients[i].severity = newSeverity;
            // Re-heapify from the updated position
            int parentIndex = (i - 1) / 2;
            if (parentIndex >= 0 && heap->patients[parentIndex].severity < heap->patients[i].severity) {
                // Heapify upwards
                while (i != 0 && heap->patients[parentIndex].severity < heap->patients[i].severity) {
                    swap(&heap->patients[i], &heap->patients[parentIndex]);
                    i = parentIndex;
                    parentIndex = (i - 1) / 2;
                }
            } else {
                // Heapify downwards
                heapify(heap, i);
            }
            return;
        }
    }
    printf(RED "Patient %s not found.\n" RESET, name);
}

// Display current patients with the most severe patient highlighted
void displayPatients(MaxHeap* heap) {
    if (heap->size == 0) {
        printf(RED "No patients in the queue.\n" RESET);
        return;
    }

    printf(YELLOW "\nCurrent Patients in the Queue:\n" RESET);
    printf(CYAN "+---------------------------+----------+\n" RESET);
    printf(CYAN "| Patient Name              | Severity |\n" RESET);
    printf(CYAN "+---------------------------+----------+\n" RESET);
    
    // Temporary heap for sorting
    MaxHeap tempHeap;
    initHeap(&tempHeap);

    // Copy patients to a temporary max heap
    for (int i = 0; i < heap->size; i++) {
        insert(&tempHeap, heap->patients[i].name, heap->patients[i].severity);
    }

    // Extract the most severe patient first
    if (tempHeap.size > 0) {
        Patient topPatient = extractMax(&tempHeap);
        printf(RED "| %-25s | %-8d |\n" RESET, topPatient.name, topPatient.severity);
    }

    // Extract remaining patients from the temp heap
    while (tempHeap.size > 0) {
        Patient p = extractMax(&tempHeap);
        printf(CYAN "| %-25s | %-8d |\n" RESET, p.name, p.severity);
    }

    printf(CYAN "+---------------------------+----------+\n" RESET);
}

// Swap two patients
void swap(Patient* a, Patient* b) {
    Patient temp = *a;
    *a = *b;
    *b = temp;
}

// Print decorative lines
void printLine(char ch, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", ch);
    }
    printf("\n");
}

// Interactive menu for user input
void interactiveMenu(MaxHeap* heap) {
    int choice;
    char name[50];
    int severity;

    while (1) {
        printLine('-', 50);
        printf(BOLD "\nEmergency Room Patient Management System\n" RESET);
        printLine('*', 50);
        printf("1. Add Patient\n");
        printf("2. Treat Next Patient\n");
        printf("3. Update Patient Severity\n");
        printf("4. Display Patients\n");
        printf("5. Exit\n");
        printf(BLUE "Select an option: " RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf(BLUE "Enter patient name: " RESET);
                scanf("%s", name);
                severity = getSeverityInput();
                insert(heap, name, severity);
                displayPatients(heap); // Show patients after adding
                break;
            case 2:
                if (heap->size > 0) {
                    Patient treated = extractMax(heap);
                    printf(GREEN "Treated patient: %s with severity %d\n" RESET, treated.name, treated.severity);
                } 
                // else {
                //     printf(RED "No patients to treat.\n" RESET);
                // }
                displayPatients(heap); // Show patients after treating
                break;
            case 3:
                if (heap->size > 0) {
                    printf(BLUE "Enter patient name to update severity: " RESET);
                    scanf("%s", name);
                    severity = getSeverityInput();
                    updateSeverity(heap, name, severity);
                } 
                // else {
                //     printf(RED "No patients to update.\n" RESET);
                // }
                displayPatients(heap); // Show patients after updating
                break;
            case 4:
                displayPatients(heap);
                break;
            case 5:
                printf(RED "Exiting...\n" RESET);
                exit(0);
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
                break;
        }
    }
}

// Function to get valid severity input
int getSeverityInput() {
    int severity;
    while (1) {
        printf(BLUE "Enter severity (0-10): " RESET);
        scanf("%d", &severity);
        if (severity >= 0 && severity <= 10) {
            return severity;
        } else {
            printf(RED "Invalid input. Please enter a severity between 0 and 10.\n" RESET);
        }
    }
}