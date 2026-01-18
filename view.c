#include <stdio.h>  // Header file for standard input/output functions (printf, fprintf, fopen, fread, fseek, etc.)
#include <string.h> // Header file for string manipulation functions (strcmp, strstr, strlen, etc.)
#include <stdlib.h> // Header file for memory allocation functions (malloc, free, etc.)
#include "type.h"   // User-defined header file for custom type definitions (Status, e_success, e_failure)
#include "view.h"   // User-defined header file for ViewInfo structure and function declarations

/*
 * Function: open_files
 * Description: Opens the source MP3 file in read mode and validates the file pointer
 * Parameters: viInfo - pointer to ViewInfo structure containing file information
 * Return: Status (e_success/e_failure)
 */
Status open_files(ViewInfo *viInfo)
{

  viInfo->fptr_src_song = fopen(viInfo->src_song_fname, "r"); // Open source MP3 file (ex: sample.mp3) in read mode

  if (viInfo->fptr_src_song == NULL) // Error handling: Check if file pointer is NULL (file opening failed)
  {
    perror("fopen"); // Print system error message for file opening failure

    fprintf(stderr, "\033[1;91mERROR: Unable to open file %s\033[0m\n", viInfo->src_song_fname); // Print custom error message with red color formatting indicating unable to open file
    return e_failure;                                                                            // Return failure status
  }
  return e_success; // Return success if file opened successfully
}

/*
 * Function: read_and_validate_for_view
 * Description: Validates command-line arguments for viewing operation, checks for .mp3 extension and valid filename
 * Parameters: argv[] - command-line argument array, viInfo - pointer to ViewInfo structure
 * Return: Status (e_success/e_failure)
 */
Status read_and_validate_for_view(char *argv[], ViewInfo *viInfo)
{
  if (argv[2][0] != '.') // Validate that source filename doesn't start with '.' (hidden file or invalid format)
  {
    if (strstr(argv[2], ".mp3")) // Check if ".mp3" extension is present in the source filename
    {
      // Step 2: Store source filename in ViewInfo structure
      viInfo->src_song_fname = argv[2]; // Copy source filename (ex: sample.mp3) to viInfo structure
    }
    else
    {
      printf("\033[1;91mERROR: \033[1;97mInvalid source file without .mp3 extension\n"); // Print error message if file doesn't have .mp3 extension
      return e_failure;                                                                  // Return failure if .mp3 extension not found
    }
  }
  else
  {
    printf("\033[1;91mERROR: \033[1;97mInvalid source file without filename\n"); // Print error message if filename starts with '.' (invalid filename)
    return e_failure;                                                            // Return failure if filename starts with '.'
  }

  return e_success; // Return success if all validation conditions are met
}

/*
 * Function: version_reader
 * Description: Reads and validates ID3 tag header (first 3 bytes should be "ID3")
 * Parameters: viInfo - pointer to ViewInfo structure
 * Return: Status (e_success/e_failure)
 */
Status version_reader(ViewInfo *viInfo)
{
  viInfo->version = (char *)malloc(3 * sizeof(char)); // Allocate memory for 3 bytes to store ID3 tag identifier

  fread(viInfo->version, 1, 3, viInfo->fptr_src_song); // Read first 3 bytes from MP3 file to check for "ID3" header

  if (strcmp(viInfo->version, "ID3") != 0) // Validate if the file contains "ID3" tag header
  {
    printf("\033[1;91mERROR: \033[1;97mInvalid source file without filename without ID3\n"); // Print error message if ID3 tag not found (invalid MP3 file format)
    return e_failure;                                                                        // Return failure if ID3 tag not present
  }

  free(viInfo->version); // Free allocated memory for version string

  fseek(viInfo->fptr_src_song, 7, SEEK_CUR); // Skip next 7 bytes in the file (ID3 version, revision, flags, and size information)

  return e_success; // Return success if ID3 tag validated successfully
}

/*
 * Function: Big_to_Little_Endian
 * Description: Converts 4-byte big-endian data to little-endian format and calculates size
 * Parameters: ch - pointer to 4-byte character array, size - pointer to store calculated size
 * Return: void
 */
void Big_to_Little_Endian(char *ch, int *size)
{
  int start = 0, end = 4 - 1; // Initialize start and end indices for byte reversal

  // Reverse the byte order (swap bytes from big-endian to little-endian)
  while (start < end)
  {
    char temp = ch[start]; // Store start byte in temporary variable
    ch[start] = ch[end];   // Move end byte to start position
    ch[end] = temp;        // Move temp (original start) to end position
    start++;               // Move start pointer forward
    end--;                 // Move end pointer backward
  }

  // Calculate the total size by summing all 4 bytes
  for (int i = 0; i < 4; i++)
  {
    *size += (int)ch[i]; // Add each byte value to size
  }
}

/*
 * Function: read_tag
 * Description: Reads tag content of specified size from MP3 file and stores in buffer
 * Parameters: size - number of bytes to read, store_content - buffer to store tag data, viInfo - pointer to ViewInfo structure
 * Return: Status (e_success/e_failure)
 */
Status read_tag(int size, char *store_content, ViewInfo *viInfo)
{
  fseek(viInfo->fptr_src_song, 3, SEEK_CUR); // Skip 3 bytes (encoding and other metadata bytes)

  fread(store_content, 1, size - 1, viInfo->fptr_src_song); // Read tag content (size-1 bytes) from file into store_content buffer

  // Find the end of the string (locate null terminator position)
  int i = 0;
  while (store_content[i])
  {
    i++; // Increment index until null character found
  }

  store_content[i] = '\0'; // Ensure null termination at the end of string

  return e_success; // Return success if tag content read successfully
}

/*
 * Function: TAG_reader
 * Description: Reads a complete ID3v2 frame including tag identifier, size, and content
 * Parameters: viInfo - pointer to ViewInfo structure
 * Return: Status (e_success/e_failure)
 */
Status TAG_reader(ViewInfo *viInfo)
{
  fread(viInfo->TAG, 1, 4, viInfo->fptr_src_song); // Read 4-byte tag identifier (ex: TIT2, TPE1, TALB, TYER, TCON, COMM)
  viInfo->TAG[4] = '\0';                           // Null-terminate the TAG string

  char ch1[4]; // Array to store 4-byte size information

  fread(ch1, 1, 4, viInfo->fptr_src_song); // Read 4 bytes representing the size of tag content

  viInfo->tag_size = 0; // Initialize tag size to 0

  Big_to_Little_Endian(ch1, &viInfo->tag_size); // Convert big-endian size bytes to little-endian and calculate actual size

  viInfo->tag = malloc((viInfo->tag_size) * sizeof(char)); // Allocate memory for tag content based on calculated size

  read_tag(viInfo->tag_size, viInfo->tag, viInfo); // Read the actual tag content into allocated memory

  return e_success; // Return success after reading tag
}

/*
 * Function: read_and_print_for_tag
 * Description: Reads ID3v2 tags and prints formatted output for TITLE, ARTIST, ALBUM, YEAR, GENRE, and COMMENT
 * Parameters: viInfo - pointer to ViewInfo structure
 * Return: Status (e_success/e_failure)
 */
Status read_and_print_for_tag(ViewInfo *viInfo)
{
  viInfo->TAG = (char *)malloc(5 * sizeof(char)); // Allocate memory for 5 bytes to store TAG identifier (4 bytes + null terminator)

  for (int i = 0; i < 6; i++) // Loop through first 6 tags (TIT2, TPE1, TALB, TYER, TCON, COMM)
  {
    TAG_reader(viInfo);                   // Read current tag identifier and content
    if (strcmp(viInfo->TAG, "TIT2") == 0) // Check if current tag is TITLE (TIT2)
    {
      printf("▐ \033[1;93m\033[1;7m \033[1;92m %-4s\033[0m\033[1;97m     %-5s \033[1;3m%-102s\033[0m▌\n", "TITLE ", ":", viInfo->tag); // Print TITLE
      printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
    }
    else if (strcmp(viInfo->TAG, "TPE1") == 0) // Check if current tag is ARTIST (TPE1)
    {
      printf("▐ \033[1;93m\033[1;7m \033[1;92m %-4s\033[0m\033[1;97m    %-5s \033[1;3m%-102s\033[0m▌\n", "ARTIST ", ":", viInfo->tag); // Print ARTIST
      printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
    }
    else if (strcmp(viInfo->TAG, "TALB") == 0) // Check if current tag is ALBUM (TALB)
    {
      printf("▐ \033[1;93m\033[1;7m \033[1;92m %-4s\033[0m\033[1;97m     %-5s \033[1;3m%-102s\033[0m▌\n", "ALBUM ", ":", viInfo->tag); // Print ALBUM
      printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
    }
    else if (strcmp(viInfo->TAG, "TYER") == 0) // Check if current tag is YEAR (TYER)
    {
      printf("▐ \033[1;93m\033[1;7m \033[1;92m %-4s\033[0m\033[1;97m      %-5s \033[1;3m%-102s\033[0m▌\n", "YEAR ", ":", viInfo->tag); // Print YEAR
      printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
    }
    else if (strcmp(viInfo->TAG, "TCON") == 0) // Check if current tag is GENRE (TCON)
    {
      printf("▐ \033[1;93m\033[1;7m \033[1;92m %-4s\033[0m\033[1;97m     %-5s \033[1;3m%-102s\033[0m▌\n", "GENRE ", ":", viInfo->tag); // Print GENRE
      printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
    }
    else if (strcmp(viInfo->TAG, "COMM") == 0) // Check if current tag is COMMENT (COMM)
    {
      printf("▐ \033[1;93m\033[1;7m \033[1;92m %-4s\033[0m\033[1;97m   %-5s \033[1;3m%-102s\033[0m▌\n\033[1;97m", "COMMENT ", ":", viInfo->tag); // Print COMMENT
    }
  }
  free(viInfo->TAG); // Free allocated memory for TAG identifier
}

/*
 * Function: view_tags
 * Description: Main orchestration function to view MP3 tags - opens file, prints header, reads tags, and prints footer
 * Parameters: viInfo - pointer to ViewInfo structure
 * Return: Status (e_success/e_failure)
 */
Status view_tags(ViewInfo *viInfo)
{
  // Open source MP3 file and validate file pointer
  if (open_files(viInfo) == e_failure)
  {
    return e_failure; // Return failure if file opening fails
  }
  if (version_reader(viInfo) == e_failure) // Validate ID3 version and skip header bytes
  {
    return e_failure; // Return failure if version validation fails
  }
  printf("\033[1;97m\n▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌\n");

  printf("▐ \033[1;7;93m%-47c\033[1;92m %s \033[0m\033[1;7;93m%-46c\033[0m\033[1;97m ▌\n", ' ', "MP3 Tag Reader and Editor", ' ');

  printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");

  if (read_and_print_for_tag(viInfo) == e_failure) // Read and print all ID3 tags from the MP3 file
  {
    return e_failure; // Return failure if tag reading fails
  }

  free(viInfo->tag); // Free allocated memory for tag content

  printf("\033[1;97m▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌\n\n");
}