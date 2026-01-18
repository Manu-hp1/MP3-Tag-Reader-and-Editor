#ifndef VIEW_H // If not defined VIEW_H ---> Checks if VIEW_H was previously defined: If not present, code is processed; else code below till #endif is ignored
#define VIEW_H // Defines the macro VIEW_H if macro was not previously defined

#include "type.h"  // Include user-defined header file for custom type definitions
#include <stdio.h> // Header file for standard input and output (printf(), scanf(), etc.)

// Structure to store MP3 file view/tag information
typedef struct // typedef used to give alternate name for structure here
{
  char *tag;            // Pointer to store tag identifier (e.g., "ID3", "TIT2", etc.)
  int tag_size;         // Integer to store the size of the tag data
  char *TAG;            // Pointer to store TAG version identifier
  char *version;        // Pointer to store ID3 version information
  char *src_song_fname; // Pointer to store source filename ---> ex: sample.mp3
  FILE *fptr_src_song;  // File pointer to store address of the source MP3 file ---> ex: sample.mp3
} ViewInfo;             // ViewInfo is alternate name for this structure

/*
 * Function: read_and_validate_for_view
 * Description: Reads and validates command-line arguments for viewing MP3 tags
 * Parameters: argv[] - command-line argument array, viInfo - pointer to ViewInfo structure
 * Return: Status (SUCCESS/FAILURE)
 */
Status read_and_validate_for_view(char *argv[], ViewInfo *viInfo);

/*
 * Function: view_tags
 * Description: Main function to orchestrate viewing of all MP3 tags
 * Parameters: viInfo - pointer to ViewInfo structure containing file information
 * Return: Status (SUCCESS/FAILURE)
 */
Status view_tags(ViewInfo *viInfo);

/*
 * Function: open_files
 * Description: Opens the source MP3 file in read mode and validates file pointer
 * Parameters: viInfo - pointer to ViewInfo structure
 * Return: Status (SUCCESS/FAILURE)
 */
Status open_files(ViewInfo *viInfo);

/*
 * Function: read_and_print_for_tag
 * Description: Reads ID3v2 tag data and prints tag information to console
 * Parameters: viInfo - pointer to ViewInfo structure
 * Return: Status (SUCCESS/FAILURE)
 */
Status read_and_print_for_tag(ViewInfo *viInfo);

/*
 * Function: version_reader
 * Description: Reads and extracts ID3 version information from MP3 file header
 * Parameters: viInfo - pointer to ViewInfo structure
 * Return: Status (SUCCESS/FAILURE)
 */
Status version_reader(ViewInfo *viInfo);

/*
 * Function: Big_to_Little_Endian
 * Description: Converts big-endian byte order to little-endian byte order
 * Parameters: ch - pointer to character array containing bytes, size - pointer to store converted size
 * Return: void
 */
void Big_to_Little_Endian(char *ch, int *size);

/*
 * Function: TAG1_reader
 * Description: Reads and processes ID3v1 tag information (located at end of MP3 file)
 * Parameters: viInfo - pointer to ViewInfo structure
 * Return: Status (SUCCESS/FAILURE)
 */
Status TAG1_reader(ViewInfo *viInfo);

/*
 * Function: read_tag
 * Description: Reads tag content of specified size and stores in provided buffer
 * Parameters: size - number of bytes to read, store_content - buffer to store read data, viInfo - pointer to ViewInfo structure
 * Return: Status (SUCCESS/FAILURE)
 */
Status read_tag(int size, char *store_content, ViewInfo *viInfo);

void print(ViewInfo *viInfo);

#endif // End of header guard: Marks the end of conditional compilation block started by #ifndef VIEW_H