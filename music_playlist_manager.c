#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <pthread.h> // For threading

// ANSI escape codes for styling
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"


/*
Usage Instructions
Install Required Libraries: SDL2 and SDL_mixer

sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-mixer-dev


Compile the Program:
gcc music_playlist_manager.c -o music_playlist_manager -lSDL2 -lSDL2_mixer -lpthread

Run the Program:
./music_playlist_manager


*/



// Structure for a Song node
struct Song {
    char name[100];        // Name of the song file
    int length;           // Length of the song in seconds
    struct Song* next;     // Pointer to the next song in the list
};

// Global variable for controlling playback
volatile int isPlaying = 0; // Flag to indicate if playback is ongoing

// Function to create a new song node
struct Song* createSong(const char* name, int length) {
    struct Song* newSong = (struct Song*)malloc(sizeof(struct Song));
    if (newSong == NULL) {
        printf(RED "Memory allocation failed!\n" RESET);
        exit(1);
    }
    strcpy(newSong->name, name);
    newSong->length = length; // Assign the length
    newSong->next = NULL;
    return newSong;
}

// Function to add a song to the playlist
void addSong(struct Song** head) {
    char songName[100];
    int songLength;

    printf(BLUE "Enter song file path (e.g \"songs/song1.mp3\" to add song1.mp3 located in songs folder): " RESET);
    scanf(" %[^\n]", songName);  // Read string with spaces
    printf(BLUE "Enter song length (in seconds): " RESET);
    scanf("%d", &songLength);     // Read song length

    struct Song* newSong = createSong(songName, songLength);
    if (*head == NULL) {
        *head = newSong;
    } else {
        struct Song* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newSong;
        
    }
        printf(GREEN "%s added to the playlist.\n" RESET, songName);
    
}


// Function to display the playlist
void displayPlaylist(struct Song* head) {
    if (head == NULL) {
        printf(YELLOW "The playlist is empty.\n" RESET);
        return;
    }
    struct Song* temp = head;
    printf(BOLD CYAN "***Playlist***\n" RESET);
    while (temp != NULL) {
        printf(GREEN "Song: %s, Length: %d seconds\n" RESET, temp->name, temp->length);
        temp = temp->next;
    }
}

// Function to play songs from the playlist
void* playSongs(void* arg) {
    struct Song* head = (struct Song*)arg;
    if (head == NULL) {
        printf(RED "The playlist is empty. Cannot play songs.\n" RESET);
        return NULL;
    }

    struct Song* temp = head;
    // printf(BOLD "Playing songs" RESET);

    while (temp != NULL) {
        // Load and play the song
        Mix_Music* music = Mix_LoadMUS(temp->name);
        if (music == NULL) {
            printf(RED "Failed to load music file: %s. SDL_mixer Error: %s\n" RESET, temp->name, Mix_GetError());
        } else {
            isPlaying = 1; // Set the flag indicating playback
            printf(BOLD CYAN "\nNow playing: %s\n" RESET, temp->name); // Display current song
            Mix_PlayMusic(music, 1); // Play the music once
            
            // Wait until the music finishes or is interrupted
            while (Mix_PlayingMusic()) {
                SDL_Delay(100); // Wait until the music finishes
                if (!isPlaying) { // Check if the user wants to stop
                    Mix_HaltMusic(); // Stop the music
                    break;
                }
            }
            Mix_FreeMusic(music);
        }

        temp = temp->next;
    }

    isPlaying = 0; // Clear the flag
    printf(BOLD BLUE "Finished playing all songs.\n" RESET);
    return NULL;
}

// Function to free the playlist memory
void freePlaylist(struct Song* head) {
    struct Song* current = head;
    while (current != NULL) {
        struct Song* next = current->next;
        free(current);
        current = next;
    }
}

// Function to print a decorative line
void printLine(char ch, int length) {
    for (int i = 0; i < length; i++) {
        printf("%c", ch);
    }
    printf("\n");
}

// Main function
int main() {
    struct Song* playlist = NULL; // Head of the playlist
    int choice;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf(RED "SDL could not initialize! SDL Error: %s\n" RESET, SDL_GetError());
        return 1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf(RED "SDL_mixer could not initialize! SDL_mixer Error: %s\n" RESET, Mix_GetError());
        return 1;
    }

    pthread_t playbackThread; // Thread for playing songs

    do {
        printLine('-', 30); // Decorative line
        printf(BOLD "\nMusic Playlist Manager\n" RESET);
        printLine('*', 30); // Decorative line
        printf("1. Add a song\n");
        printf("2. Display playlist\n");
        printf("3. Play songs\n");
        printf("4. Stop playing\n");
        printf("5. Exit\n");
        printLine('-', 30); // Decorative line

        // Check for user choice after all messages
        printf(BLUE "Enter your choice: " RESET);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addSong(&playlist);
                break;
            case 2:
                displayPlaylist(playlist);
                break;
            case 3:
                if (pthread_create(&playbackThread, NULL, playSongs, (void*)playlist) != 0) {
                    printf(RED "Failed to create thread for playback.\n" RESET);
                }
                break;
            case 4:
                isPlaying = 0; // Signal to stop playback
                printf(BOLD RED "Playing Stopped.\n" RESET);
                break;
            case 5:
                printf(RED "Exiting...\n" RESET);
                isPlaying = 0; // Ensure playback stops on exit
                pthread_join(playbackThread, NULL); // Wait for playback thread to finish
                break;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
        }

        // short delay to ensure all messages are printed before the next prompt
        SDL_Delay(500); // Adjust the delay as necessary
    } while (choice != 5);

    // Free memory
    freePlaylist(playlist);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}