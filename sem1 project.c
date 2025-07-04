#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define constants
#define MAX_ROOMS 100
#define MAX_NAME_LENGTH 50
#define MAX_CONTACT_LENGTH 15

// Enum for room types - more readable than magic numbers
typedef enum
{
  SUITE = 1,
  DOUBLE = 2,
  SINGLE = 3
} RoomType;

// Pricing structure
const float ROOM_RATES[] = {0.0, 1000.0, 600.0, 400.0}; // Index matches RoomType values

// Date structure for check-in and check-out dates
typedef struct
{
  int day;
  int month;
  int year;
} Date;

// Structure to store room booking details - expanded with more fields
typedef struct
{
  int roomNumber;
  char guestName[MAX_NAME_LENGTH];
  char contact[MAX_CONTACT_LENGTH];
  RoomType roomType;
  int nights;
  float totalPrice;
  int isBooked;
  Date checkInDate;
  Date checkOutDate;
} Room;

// Function prototypes
void addBooking(Room rooms[], int *roomCount);
void deleteBooking(Room rooms[], int *roomCount);
void displayBookings(Room rooms[], int roomCount);
void searchBooking(Room rooms[], int roomCount);
void modifyBooking(Room rooms[], int roomCount);
void saveBookingsToFile(Room rooms[], int roomCount);
void loadBookingsFromFile(Room rooms[], int *roomCount);
int assignRoomNumber(Room rooms[], int roomCount, RoomType roomType);
void displayMenu();
int getValidInteger(const char *prompt);
int validateContact(const char *contact);
void calculateCheckOutDate(Date *checkIn, Date *checkOut, int nights);
const char *getRoomTypeName(RoomType type);
void getCurrentDate(Date *date);
int isDateValid(Date date);
void clearInputBuffer();

// Main function
int main()
{
  Room rooms[MAX_ROOMS] = {0};
  int roomCount = 0;
  int choice;

  // Load bookings after the initial menu display
  loadBookingsFromFile(rooms, &roomCount);

  while (1)
  {
    system("cls"); // Clear screen before showing menu
    displayMenu();
    choice = getValidInteger("Enter your choice: ");

    switch (choice)
    {
    case 1:
      addBooking(rooms, &roomCount);
      break;
    case 2:
      deleteBooking(rooms, &roomCount);
      break;
    case 3:
      displayBookings(rooms, roomCount);
      break;
    case 4:
      searchBooking(rooms, roomCount);
      break;
    case 5:
      modifyBooking(rooms, roomCount);
      break;
    case 6:
      saveBookingsToFile(rooms, roomCount);
      printf("\nThank you for using Rasa Ria Room Booking System!\n");
      return 0;
    default:
      printf("\n================================================\n");
      printf("Invalid choice. Please enter a number from 1 to 6.\n");
      printf("================================================\n");
    }
    printf("\nPress Enter to continue...");
    getchar();
  }

  return 0;
}

// Function to display the main menu
void displayMenu()
{
  printf("\n\n");
  printf("\t\t=============================================\n");
  printf("\t\t|                                           |\n");
  printf("\t\t|       ----------------------------        |\n");
  printf("\t\t|            RASA RIA ROOM BOOKING          |\n");
  printf("\t\t|       ----------------------------        |\n");
  printf("\t\t|                                           |\n");
  printf("\t\t=============================================\n\n");
  printf("===== RASA RIA HOTEL SERVICES =====\n");
  printf("1. Add New Booking\n");
  printf("2. Cancel Booking\n");
  printf("3. Display All Bookings\n");
  printf("4. Search for a Booking\n");
  printf("5. Modify Existing Booking\n");
  printf("6. Save & Exit\n");
  printf("==================================\n");
}

// Function to clear input buffer
void clearInputBuffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

// Function to get valid integer input
int getValidInteger(const char *prompt)
{
  int value;
  char line[256];

  while (1)
  {
    printf("%s", prompt);
    if (fgets(line, sizeof(line), stdin) == NULL)
    {
      continue;
    }

    if (sscanf(line, "%d", &value) == 1)
    {
      return value;
    }

    printf("Invalid input. Please enter a valid number.\n");
  }
}

// Function to validate contact number 
int validateContact(const char *contact)
{
  // Simple validation: must be between 8-12 digits
  int len = strlen(contact);
  if (len < 8 || len > 12)
  {
    return 0;
  }
  for (int i = 0; i < len; i++)
  {
    if (!((contact[i] >= '0' && contact[i] <= '9') || contact[i] == '-' || contact[i] == '+'))
    {
      return 0;
    }
  }
  return 1;
}

// Function to get current date 
void getCurrentDate(Date *date)
{
  printf("Enter today's date.\n");
  printf("Day (DD): ");
  scanf("%d", &date->day);
  printf("Month (MM): ");
  scanf("%d", &date->month);
  printf("Year (YYYY): ");
  scanf("%d", &date->year);
}

// Function to validate date
int isDateValid(Date date)
{
  // Basic validation
  if (date.month < 1 || date.month > 12)
    return 0;

  int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  // Adjust for leap year
  if (date.year % 400 == 0 || (date.year % 100 != 0 && date.year % 4 == 0))
  {
    daysInMonth[2] = 29;
  }

  return (date.day > 0 && date.day <= daysInMonth[date.month]);
}

// Function to calculate check-out date (simple addition, no time.h)
void calculateCheckOutDate(Date *checkIn, Date *checkOut, int nights)
{
  // Just add nights to the day (no month/year handling for simplicity)
  checkOut->day = checkIn->day + nights;
  checkOut->month = checkIn->month;
  checkOut->year = checkIn->year;
}

// Function to get room type name string
const char *getRoomTypeName(RoomType type)
{
  switch (type)
  {
  case SUITE:
    return "Suite";
  case DOUBLE:
    return "Double";
  case SINGLE:
    return "Single";
  default:
    return "Unknown";
  }
}

// Function to add a room booking
void addBooking(Room rooms[], int *roomCount)
{
  if (*roomCount >= MAX_ROOMS)
  {
    printf("\n=============================================\n");
    printf("No more rooms can be booked. Hotel is full!\n");
    printf("=============================================\n");
    return;
  }

  Room newBooking;
  int roomTypeInput, nightsInput;

  printf("\n=== ADDING NEW BOOKING ===\n");

  printf("Enter guest name      : ");
  clearInputBuffer(); // Clear any previous input
  fgets(newBooking.guestName, sizeof(newBooking.guestName), stdin);
  newBooking.guestName[strcspn(newBooking.guestName, "\n")] = '\0'; // Remove newline

  do
  {
    printf("Enter contact number(8-12 digits) : ");
    fgets(newBooking.contact, sizeof(newBooking.contact), stdin);
    newBooking.contact[strcspn(newBooking.contact, "\n")] = '\0'; // Remove newline
    if (!validateContact(newBooking.contact))
    {
      printf("Invalid contact number. Please enter 8-12 digits with optional +/- symbols.\n");
    }
  } while (!validateContact(newBooking.contact));

  printf("Room Types:\n");
  printf("1. Suite  - RM%.2f per night\n", ROOM_RATES[SUITE]);
  printf("2. Double - RM%.2f per night\n", ROOM_RATES[DOUBLE]);
  printf("3. Single - RM%.2f per night\n", ROOM_RATES[SINGLE]);

  printf("Enter room type (1 for Suite, 2 for Double, 3 for Single): ");
  if (scanf("%d", &roomTypeInput) != 1 || roomTypeInput < 1 || roomTypeInput > 3)
  {
    printf("===================================\n");
    printf("Invalid input. Please enter a valid room type (1-3).\n");
    printf("===================================\n");
    while (getchar() != '\n')
      ; // Clear the input buffer
    return;
  }
  newBooking.roomType = (RoomType)roomTypeInput;

  printf("Enter number of nights(1-30): ");
  if (scanf("%d", &nightsInput) != 1 || nightsInput <= 0 || nightsInput > 30)
  {
    printf("===================================\n");
    printf("Invalid input. Please enter a positive number of nights.\n");
    printf("===================================\n");
    while (getchar() != '\n')
      ; // Clear the input buffer
    return;
  }
  newBooking.nights = nightsInput;

  // Get check-in date
  printf("\nCheck-in Date:\n");
  Date today;
  getCurrentDate(&today);
  printf("Today is: %02d/%02d/%04d\n", today.day, today.month, today.year);

  // Allow booking starting from today up to 1 year in the future
  do
  {
    printf("Enter check-in day (DD): ");
    scanf("%d", &newBooking.checkInDate.day);
    printf("Enter check-in month (MM): ");
    scanf("%d", &newBooking.checkInDate.month);
    printf("Enter check-in year (YYYY): ");
    scanf("%d", &newBooking.checkInDate.year);

    // Basic validation: check if date is valid and not in the past
    if (!isDateValid(newBooking.checkInDate))
    {
      printf("Invalid date. Please enter a valid date.\n");
      continue;
    }

    // Compare check-in date with today's date
    if (newBooking.checkInDate.year < today.year ||
        (newBooking.checkInDate.year == today.year &&
         newBooking.checkInDate.month < today.month) ||
        (newBooking.checkInDate.year == today.year &&
         newBooking.checkInDate.month == today.month &&
         newBooking.checkInDate.day < today.day))
    {
      printf("Check-in date cannot be in the past. Please enter a valid date.\n");
      continue;
    }

    // Check if date is more than a year into the future
    if (newBooking.checkInDate.year > today.year + 1 ||
        (newBooking.checkInDate.year == today.year + 1 &&
         newBooking.checkInDate.month > today.month) ||
        (newBooking.checkInDate.year == today.year + 1 &&
         newBooking.checkInDate.month == today.month &&
         newBooking.checkInDate.day > today.day))
    {
      printf("Cannot book more than one year in advance. Please enter a valid date.\n");
      continue;
    }

    break;
  } while (1);

  // Calculate check-out date
  calculateCheckOutDate(&newBooking.checkInDate, &newBooking.checkOutDate, newBooking.nights);

  // Calculate total price based on room type and number of nights
  newBooking.totalPrice = ROOM_RATES[newBooking.roomType] * newBooking.nights;

  // Assign a room number
  newBooking.roomNumber = assignRoomNumber(rooms, *roomCount, newBooking.roomType);
  if (newBooking.roomNumber == -1)
  {
    printf("\n==============================================\n");
    printf("No available room of the selected type. Sorry!\n");
    printf("==============================================\n");
    clearInputBuffer();
    return;
  }

  newBooking.isBooked = 1;
  rooms[*roomCount] = newBooking;
  (*roomCount)++;

  clearInputBuffer();

  printf("\n===========================================================\n");
  printf("Booking added successfully!\n");
  printf("Room number assigned: %d\n", newBooking.roomNumber);
  printf("Check-in date: %02d/%02d/%04d\n",
         newBooking.checkInDate.day, newBooking.checkInDate.month, newBooking.checkInDate.year);
  printf("Check-out date: %02d/%02d/%04d\n",
         newBooking.checkOutDate.day, newBooking.checkOutDate.month, newBooking.checkOutDate.year);
  printf("Total price: $%.2f\n", newBooking.totalPrice);
  printf("===========================================================\n");
}

// Function to assign room number based on room type
int assignRoomNumber(Room rooms[], int roomCount, RoomType roomType)
{
  int start, end;

  // Set room number ranges based on room type
  switch (roomType)
  {
  case SUITE:
    start = 1;
    end = 20;
    break;
  case DOUBLE:
    start = 21;
    end = 60;
    break;
  case SINGLE:
    start = 61;
    end = 100;
    break;
  default:
    return -1; // Invalid room type
  }

  // Find first available room number in the appropriate range
  for (int i = start; i <= end; i++)
  {
    int available = 1;
    for (int j = 0; j < roomCount; j++)
    {
      if (rooms[j].roomNumber == i && rooms[j].isBooked)
      {
        available = 0;
        break;
      }
    }
    if (available)
    {
      return i;
    }
  }

  return -1; // No available room found
}

// Function to delete a room booking
void deleteBooking(Room rooms[], int *roomCount)
{
  if (*roomCount == 0)
  {
    printf("\n===================\n");
    printf("No bookings found.\n");
    printf("===================\n");
    return;
  }

  int roomNumber, index = -1;

  roomNumber = getValidInteger("\nEnter room number to cancel: ");

  for (int i = 0; i < *roomCount; i++)
  {
    if (rooms[i].roomNumber == roomNumber && rooms[i].isBooked)
    {
      index = i;
      break;
    }
  }

  if (index == -1)
  {
    printf("\n=====================================\n");
    printf("Room %d not found or not booked.\n", roomNumber);
    printf("=====================================\n");
    return;
  }

  // Display booking details and confirm deletion
  printf("\n=== BOOKING DETAILS ===\n");
  printf("Room Number: %d\n", rooms[index].roomNumber);
  printf("Guest Name : %s\n", rooms[index].guestName);
  printf("Room Type  : %s\n", getRoomTypeName(rooms[index].roomType));
  printf("Check-in   : %02d/%02d/%04d\n",
         rooms[index].checkInDate.day, rooms[index].checkInDate.month, rooms[index].checkInDate.year);
  printf("Check-out  : %02d/%02d/%04d\n",
         rooms[index].checkOutDate.day, rooms[index].checkOutDate.month, rooms[index].checkOutDate.year);

  char confirm;
  printf("\nAre you sure you want to cancel this booking? (y/n): ");
  clearInputBuffer();
  scanf("%c", &confirm);

  if (confirm == 'y' || confirm == 'Y')
  {
    for (int i = index; i < *roomCount - 1; i++)
    {
      rooms[i] = rooms[i + 1];
    }
    (*roomCount)--;

    printf("\n===============================\n");
    printf("Booking canceled successfully.\n");
    printf("===============================\n");
  }
  else
  {
    printf("\n===================\n");
    printf("Cancellation aborted.\n");
    printf("===================\n");
  }

  clearInputBuffer();
}

// Function to display all room bookings
void displayBookings(Room rooms[], int roomCount)
{
  if (roomCount == 0)
  {
    printf("\n===================\n");
    printf("No bookings found.\n");
    printf("===================\n");
    return;
  }

  printf("\n========================== BOOKING DETAILS ==========================\n");
  printf("%-5s %-20s %-15s %-10s %-12s %-12s %-10s\n",
         "Room", "Guest Name", "Contact", "Type", "Check-in", "Check-out", "Price");
  printf("--------------------------------------------------------------------\n");

  for (int i = 0; i < roomCount; i++)
  {
    if (rooms[i].isBooked)
    {
      printf("%-5d %-20s %-15s %-10s %02d/%02d/%04d  %02d/%02d/%04d  $%-9.2f\n",
             rooms[i].roomNumber,
             rooms[i].guestName,
             rooms[i].contact,
             getRoomTypeName(rooms[i].roomType),
             rooms[i].checkInDate.day, rooms[i].checkInDate.month, rooms[i].checkInDate.year,
             rooms[i].checkOutDate.day, rooms[i].checkOutDate.month, rooms[i].checkOutDate.year,
             rooms[i].totalPrice);
    }
  }

  printf("====================================================================\n");
}

// Function to search for a booking
void searchBooking(Room rooms[], int roomCount)
{
  if (roomCount == 0)
  {
    printf("\n===================\n");
    printf("No bookings found.\n");
    printf("===================\n");
    return;
  }

  printf("\n=== SEARCH BOOKING ===\n");
  printf("1. Search by Room Number\n");
  printf("2. Search by Guest Name\n");

  int choice = getValidInteger("Enter your choice: ");

  switch (choice)
  {
  case 1:
  {
    int roomNumber = getValidInteger("Enter room number: ");
    int found = 0;

    for (int i = 0; i < roomCount; i++)
    {
      if (rooms[i].roomNumber == roomNumber && rooms[i].isBooked)
      {
        found = 1;
        printf("\n=== BOOKING DETAILS ===\n");
        printf("Room Number: %d\n", rooms[i].roomNumber);
        printf("Guest Name : %s\n", rooms[i].guestName);
        printf("Contact    : %s\n", rooms[i].contact);
        printf("Room Type  : %s\n", getRoomTypeName(rooms[i].roomType));
        printf("Check-in   : %02d/%02d/%04d\n",
               rooms[i].checkInDate.day, rooms[i].checkInDate.month, rooms[i].checkInDate.year);
        printf("Check-out  : %02d/%02d/%04d\n",
               rooms[i].checkOutDate.day, rooms[i].checkOutDate.month, rooms[i].checkOutDate.year);
        printf("Nights     : %d\n", rooms[i].nights);
        printf("Total Price: $%.2f\n", rooms[i].totalPrice);
        printf("=======================\n");
        break;
      }
    }

    if (!found)
    {
      printf("\n===================================\n");
      printf("No booking found for Room %d.\n", roomNumber);
      printf("===================================\n");
    }
    break;
  }

  case 2:
  {
    char searchName[MAX_NAME_LENGTH];
    int found = 0;

    printf("Enter guest name: ");
    clearInputBuffer();
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0'; // Remove newline

    printf("\n=== SEARCH RESULTS ===\n");
    for (int i = 0; i < roomCount; i++)
    {
      // Case-sensitive partial name match (strstr)
      if (rooms[i].isBooked && strstr(rooms[i].guestName, searchName) != NULL)
      {
        found = 1;
        printf("\n--- Booking %d ---\n", found);
        printf("Room Number: %d\n", rooms[i].roomNumber);
        printf("Guest Name : %s\n", rooms[i].guestName);
        printf("Contact    : %s\n", rooms[i].contact);
        printf("Room Type  : %s\n", getRoomTypeName(rooms[i].roomType));
        printf("Check-in   : %02d/%02d/%04d\n",
               rooms[i].checkInDate.day, rooms[i].checkInDate.month, rooms[i].checkInDate.year);
        printf("Check-out  : %02d/%02d/%04d\n",
               rooms[i].checkOutDate.day, rooms[i].checkOutDate.month, rooms[i].checkOutDate.year);
        printf("Total Price: $%.2f\n", rooms[i].totalPrice);
      }
    }

    if (!found)
    {
      printf("\n==========================================\n");
      printf("No bookings found for guest: %s\n", searchName);
      printf("==========================================\n");
    }
    else
    {
      printf("\n=== End of Search Results ===\n");
    }
    break;
  }

  default:
    printf("\n==========================\n");
    printf("Invalid choice. Try again.\n");
    printf("==========================\n");
    break;
  }
}

// Function to modify an existing booking
void modifyBooking(Room rooms[], int roomCount)
{
  if (roomCount == 0)
  {
    printf("\n===================\n");
    printf("No bookings found.\n");
    printf("===================\n");
    return;
  }

  int roomNumber, index = -1;
  roomNumber = getValidInteger("Enter room number to modify: ");

  for (int i = 0; i < roomCount; i++)
  {
    if (rooms[i].roomNumber == roomNumber && rooms[i].isBooked)
    {
      index = i;
      break;
    }
  }

  if (index == -1)
  {
    printf("\n===================================\n");
    printf("Room %d not found or not booked.\n", roomNumber);
    printf("===================================\n");
    return;
  }

  // Display current booking details
  printf("\n=== CURRENT BOOKING DETAILS ===\n");
  printf("1. Guest Name : %s\n", rooms[index].guestName);
  printf("2. Contact    : %s\n", rooms[index].contact);
  printf("3. Room Type  : %s\n", getRoomTypeName(rooms[index].roomType));
  printf("4. Check-in   : %02d/%02d/%04d\n",
         rooms[index].checkInDate.day, rooms[index].checkInDate.month, rooms[index].checkInDate.year);
  printf("5. Nights     : %d\n", rooms[index].nights);
  printf("6. Return to Main Menu\n");

  int choice = getValidInteger("\nEnter what to modify (1-6): ");

  switch (choice)
  {
  case 1:
  {
    printf("Enter new guest name: ");
    clearInputBuffer();
    fgets(rooms[index].guestName, sizeof(rooms[index].guestName), stdin);
    rooms[index].guestName[strcspn(rooms[index].guestName, "\n")] = '\0'; // Remove newline
    printf("\nGuest name updated successfully.\n");
    break;
  }

  case 2:
  {
    char newContact[MAX_CONTACT_LENGTH];
    do
    {
      printf("Enter new contact number: ");
      clearInputBuffer();
      fgets(newContact, sizeof(newContact), stdin);
      newContact[strcspn(newContact, "\n")] = '\0'; // Remove newline

      if (!validateContact(newContact))
      {
        printf("Invalid contact number. Please enter 8-12 digits with optional +/- symbols.\n");
      }
      else
      {
        strcpy(rooms[index].contact, newContact);
        printf("\nContact updated successfully.\n");
        break;
      }
    } while (1);
    break;
  }

  case 3:
  {
    printf("Current room type: %s\n", getRoomTypeName(rooms[index].roomType));
    printf("Room Types:\n");
    printf("1. Suite  - $%.2f per night\n", ROOM_RATES[SUITE]);
    printf("2. Double - $%.2f per night\n", ROOM_RATES[DOUBLE]);
    printf("3. Single - $%.2f per night\n", ROOM_RATES[SINGLE]);

    int newType;
    do
    {
      newType = getValidInteger("Enter new room type (1-3): ");
      if (newType < 1 || newType > 3)
      {
        printf("Invalid room type. Please enter 1, 2, or 3.\n");
      }
    } while (newType < 1 || newType > 3);

    RoomType newRoomType = (RoomType)newType;

    // Check if there's an available room of the new type
    int newRoomNumber = assignRoomNumber(rooms, roomCount, newRoomType);

    if (newRoomNumber == -1)
    {
      printf("\nNo available rooms of the selected type. Modification canceled.\n");
      break;
    }

    // Update room type, room number, and recalculate price
    rooms[index].roomType = newRoomType;
    int oldRoomNumber = rooms[index].roomNumber;
    rooms[index].roomNumber = newRoomNumber;
    rooms[index].totalPrice = ROOM_RATES[newRoomType] * rooms[index].nights;

    printf("\nRoom type updated successfully.\n");
    printf("New room number: %d (was %d)\n", newRoomNumber, oldRoomNumber);
    printf("New total price: $%.2f\n", rooms[index].totalPrice);
    break;
  }

  case 4:
  {
    // Modify check-in date
    Date today;
    getCurrentDate(&today);
    printf("Today is: %02d/%02d/%04d\n", today.day, today.month, today.year);

    Date newCheckIn;
    do
    {
      printf("Enter new check-in day (DD): ");
      scanf("%d", &newCheckIn.day);
      printf("Enter new check-in month (MM): ");
      scanf("%d", &newCheckIn.month);
      printf("Enter new check-in year (YYYY): ");
      scanf("%d", &newCheckIn.year);

      // Basic validation: check if date is valid and not in the past
      if (!isDateValid(newCheckIn))
      {
        printf("Invalid date. Please enter a valid date.\n");
        continue;
      }

      // Compare check-in date with today's date
      if (newCheckIn.year < today.year ||
          (newCheckIn.year == today.year && newCheckIn.month < today.month) ||
          (newCheckIn.year == today.year && newCheckIn.month == today.month && newCheckIn.day < today.day))
      {
        printf("Check-in date cannot be in the past. Please enter a valid date.\n");
        continue;
      }

      // Check if date is more than a year into the future
      if (newCheckIn.year > today.year + 1 ||
          (newCheckIn.year == today.year + 1 && newCheckIn.month > today.month) ||
          (newCheckIn.year == today.year + 1 && newCheckIn.month == today.month && newCheckIn.day > today.day))
      {
        printf("Cannot book more than one year in advance. Please enter a valid date.\n");
        continue;
      }

      break;
    } while (1);

    rooms[index].checkInDate = newCheckIn;
    calculateCheckOutDate(&rooms[index].checkInDate, &rooms[index].checkOutDate, rooms[index].nights);

    printf("\nCheck-in date updated successfully.\n");
    printf("New check-in date: %02d/%02d/%04d\n",
           rooms[index].checkInDate.day, rooms[index].checkInDate.month, rooms[index].checkInDate.year);
    printf("New check-out date: %02d/%02d/%04d\n",
           rooms[index].checkOutDate.day, rooms[index].checkOutDate.month, rooms[index].checkOutDate.year);
    break;
  }

  case 5:
  {
    int newNights;
    do
    {
      newNights = getValidInteger("Enter new number of nights (1-30): ");
      if (newNights < 1 || newNights > 30)
      {
        printf("Invalid number of nights. Please enter a value between 1 and 30.\n");
      }
    } while (newNights < 1 || newNights > 30);

    rooms[index].nights = newNights;
    rooms[index].totalPrice = ROOM_RATES[rooms[index].roomType] * newNights;
    calculateCheckOutDate(&rooms[index].checkInDate, &rooms[index].checkOutDate, newNights);

    printf("\nBooking duration updated successfully.\n");
    printf("New number of nights: %d\n", newNights);
    printf("New check-out date: %02d/%02d/%04d\n",
           rooms[index].checkOutDate.day, rooms[index].checkOutDate.month, rooms[index].checkOutDate.year);
    printf("New total price: $%.2f\n", rooms[index].totalPrice);
    break;
  }

  case 6:
    printf("Returning to main menu.\n");
    break;

  default:
    printf("\n==========================\n");
    printf("Invalid choice. Try again.\n");
    printf("==========================\n");
    break;
  }

  clearInputBuffer();
}

// Function to save bookings to a file in text format
void saveBookingsToFile(Room rooms[], int roomCount)
{
  FILE *file = fopen("bookings.txt", "w");
  if (file == NULL)
  {
    printf("\n====================================\n");
    printf("Error: Unable to save bookings to file.\n");
    printf("====================================\n");
    return;
  }

  fprintf(file, "%d\n", roomCount);
  for (int i = 0; i < roomCount; i++)
  {
    fprintf(file, "%d\n%s\n%s\n%d\n%d\n%f\n%d\n%d %d %d\n%d %d %d\n",
            rooms[i].roomNumber,
            rooms[i].guestName,
            rooms[i].contact,
            rooms[i].roomType,
            rooms[i].nights,
            rooms[i].totalPrice,
            rooms[i].isBooked,
            rooms[i].checkInDate.day, rooms[i].checkInDate.month, rooms[i].checkInDate.year,
            rooms[i].checkOutDate.day, rooms[i].checkOutDate.month, rooms[i].checkOutDate.year);
  }

  fclose(file);
  printf("\n============================\n");
  printf("Bookings saved successfully.\n");
  printf("============================\n");
}

// Function to load bookings from a file in text format
void loadBookingsFromFile(Room rooms[], int *roomCount)
{
  FILE *file = fopen("bookings.txt", "r");
  if (file == NULL)
  {
    printf("No previous bookings found.\n");
    return;
  }

  if (fscanf(file, "%d\n", roomCount) != 1)
  {
    printf("Error reading booking count from file.\n");
    fclose(file);
    return;
  }

  for (int i = 0; i < *roomCount; i++)
  {
    if (fscanf(file, "%d\n", &rooms[i].roomNumber) != 1)
      break;
    if (fgets(rooms[i].guestName, sizeof(rooms[i].guestName), file) == NULL)
      break;
    rooms[i].guestName[strcspn(rooms[i].guestName, "\n")] = '\0';
    if (fgets(rooms[i].contact, sizeof(rooms[i].contact), file) == NULL)
      break;
    rooms[i].contact[strcspn(rooms[i].contact, "\n")] = '\0';
    int roomType;
    if (fscanf(file, "%d\n", &roomType) != 1)
      break;
    rooms[i].roomType = (RoomType)roomType;
    if (fscanf(file, "%d\n", &rooms[i].nights) != 1)
      break;
    if (fscanf(file, "%f\n", &rooms[i].totalPrice) != 1)
      break;
    if (fscanf(file, "%d\n", &rooms[i].isBooked) != 1)
      break;
    if (fscanf(file, "%d %d %d\n",
               &rooms[i].checkInDate.day, &rooms[i].checkInDate.month, &rooms[i].checkInDate.year) != 3)
      break;
    if (fscanf(file, "%d %d %d\n",
               &rooms[i].checkOutDate.day, &rooms[i].checkOutDate.month, &rooms[i].checkOutDate.year) != 3)
      break;
  }

  fclose(file);
  printf("Bookings loaded successfully.\n");
}