# Music Playlist Manager

## Overview

The **Music Playlist Manager** is a command-line application written in C utilizing the SDL2 and SDL_mixer libraries for audio playback. It allows users to manage a playlist of songs, add new songs, display the current playlist, and play the songs in the playlist.

## Features

- Add songs to the playlist
- Display the current playlist
- Play songs from the playlist
- Stop playback
- Exit the application

## Requirements

To run this application, you need to have the following libraries installed:

- **SDL2**
- **SDL_mixer**

## Installation

1. **Install Required Libraries**:

   For Debian-based systems (like Ubuntu), use the following commands:

   ```bash
   sudo apt-get update
   sudo apt-get install libsdl2-dev libsdl2-mixer-dev
   ```

2. **Compile the Program**:

   Use the following command to compile the program:

   ```bash
   gcc music_playlist_manager.c -o music_playlist_manager -lSDL2 -lSDL2_mixer -lpthread
   ```

3. **Run the Program**:

   Execute the program using:

   ```bash
   ./music_playlist_manager
   ```

## Usage Instructions

1. **Add a Song**: Enter the song file path and its duration when prompted.
2. **Display Playlist**: View the current list of songs with their lengths.
3. **Play Songs**: Start playing the songs in the playlist. You can stop playback anytime.
4. **Stop Playing**: Signal the application to halt playback.
5. **Exit**: Properly exit the application and free up resources.


# Simple Database System

## Overview

The **Simple Database System** is a command-line application written in C that uses a Binary Search Tree (BST) to manage a collection of products. This program allows users to insert, search, update, and display product information, making it a useful tool for managing inventories.

## Features

- Insert new products with unique IDs and names
- Search for products by name
- Update product details (ID, name, price, quantity)
- Display all products in a tabular format
- Sort products based on ID, name, price, or quantity
- Track out-of-stock products

## Requirements

To compile and run this program, you need a C compiler such as `gcc`.

## Installation

1. **Compile the Code**:

   Use the following command to compile the program:

   ```bash
   gcc bst_for_database.c -o database
   ```

2. **Run the Program**:

   Execute the compiled program using:

   ```bash
   ./database
   ```

## Usage Instructions

1. **Insert New Product**: Enter the product ID, name, price, and quantity. The system checks for uniqueness of the ID and name.
2. **Search for Product**: Input the product name to find its details.
3. **Update Product Details**: Choose a product to update its information.
4. **Display All Products**: View a complete list of products in a formatted table.
5. **Sort Products**: Select a sorting criterion (ID, name, price, or quantity) to display products in order.
6. **Track Out-of-Stock Products**: Identify products that are currently out of stock.
7. **Exit**: Exit the application safely.



# Social Network System

## Overview

The **Social Network System** is a command-line application written in C that simulates a simple social network using a graph data structure. Users can add friends, display connections, and receive friend recommendations based on their existing connections.

## Features

- Add new users to the network
- Establish connections (friendships) between users
- Display an adjacency matrix representing user connections
- Display all connections of each user
- Get friend recommendations based on mutual connections

## Requirements

To compile and run this program, you need a C compiler such as `gcc`.

## Installation

1. **Compile the Code**:

   Use the following command to compile the program:

   ```bash
   gcc social_network_system.c -o social_network
   ```

2. **Run the Program**:

   Execute the compiled program using:

   ```bash
   ./social_network
   ```

## Usage Instructions

1. **Display Adjacency Matrix**: Shows the connections between all users in a matrix format.
2. **Display Connections**: Lists all friends for each user.
3. **Get Friend Recommendations**: Enter a user's name to see suggested friends based on mutual connections.
4. **Add Connection**: Create a friendship between two users.
5. **Add New User**: Introduce a new user to the network.
6. **Exit**: Safely exit the application and free allocated memory.


# Emergency Room Patient Management System

## Overview

The **Emergency Room Patient Management System** is a command-line application written in C designed to manage patient information in an emergency room setting. It uses a max-heap data structure to prioritize patients based on their severity levels, allowing healthcare providers to treat the most critical cases first.

## Features

- Add new patients with severity levels
- Treat the next patient (highest severity)
- Update severity levels of patients
- Display all patients in the queue
- Interactive menu for easy navigation

## Requirements

To compile and run this program, you need a C compiler such as `gcc`.

## Installation

1. **Compile the Code**:

   Use the following command to compile the program:

   ```bash
   gcc patient_management_system.c -o patient_management
   ```

2. **Run the Program**:

   Execute the compiled program using:

   ```bash
   ./patient_management
   ```

## Usage Instructions

1. **Add Patient**: Enter the patient's name and severity (0-10).
2. **Treat Next Patient**: Automatically treats the patient with the highest severity.
3. **Update Patient Severity**: Modify the severity of an existing patient.
4. **Display Patients**: View all patients in the queue, with the most severe patient highlighted.
5. **Exit**: Safely exit the application.


