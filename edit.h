#ifndef EDIT_H // If not defined EDIT_H ---> Checks if EDIT_H was previously defined: If not present, code is processed; else code below till #endif is ignored
#define EDIT_H // Defines the macro EDIT_H if macro was not previously defined

#include <stdio.h> // Header file for standard input and output (printf(), scanf(), fopen(), fread(), fwrite(), etc.)
#include "type.h"  // User-defined header file for custom type definitions (Status, e_success, e_failure)

// Structure to store MP3 file edit information including tag data, file pointers, and user input
typedef struct // typedef used to give alternate name for structure here
{
  char *TAG;             // Pointer to store current tag identifier (e.g., "TIT2", "TPE1", "TALB", etc.)
  char *mode;            // Pointer to store edit mode/operation type
  int old_size;          // Integer to store the original size of the tag being edited
  char *user_content;    // Pointer to store new content provided by user for tag modification
  int user_content_size; // Integer to store the size/length of user-provided content
  char user_tag[8];      // Character array to store user-specified tag identifier (up to 7 chars + null terminator)
  char *original_fname;  // Pointer to store original MP3 filename (e.g., sample.mp3)
  FILE *fptr_original;   // File pointer to access original MP3 file for reading
  FILE *fptr_temp;       // File pointer to access temporary file for writing modified data
} EditInfo;              // EditInfo is alternate name for this structure

/**
 * FUNCTION: read_and_validate_for_edit
 * DESCRIPTION: Reads and validates command-line arguments for edit operation, checks for valid tag and .mp3 file
 * PARAMETERS: argv[] - command-line argument vector, editInfo - pointer to EditInfo structure
 * RETURN: Status (e_success/e_failure)
 */
Status read_and_validate_for_edit(char *argv[], EditInfo *editInfo);

/*
 * Function: open_file
 * Description: Opens original MP3 file in read mode and temporary file in write mode for editing
 * Parameters: editInfo - pointer to EditInfo structure containing file information
 * Return: Status (e_success/e_failure)
 */
Status open_file(EditInfo *editInfo);

/*
 * Function: do_edit_tags
 * Description: Main orchestration function to perform complete tag editing operation on MP3 file
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 */
Status do_edit_tags(EditInfo *editInfo);

/*
 * Function: copy_header_edit
 * Description: Copies ID3 header (first 10 bytes) from original file to temporary file
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 */
Status copy_header_edit(EditInfo *editInfo);

/*
 * Function: convert_big_to_little_endian_for_edit
 * Description: Converts 4-byte integer from big-endian to little-endian format for size calculation
 * Parameters: arr - pointer to integer array containing bytes to be converted
 * Return: void
 */
void convert_big_to_little_endian_for_edit(int *arr);

/*
 * Function: tag_edit
 * Description: Edits/replaces the content of a specific tag with user-provided content
 * Parameters: editInfo - pointer to EditInfo structure containing tag and new content information
 * Return: Status (e_success/e_failure)
 */
Status tag_edit(EditInfo *editInfo);

/*
 * Function: skip_tag
 * Description: Skips/copies a tag without modification when it doesn't match the target tag
 * Parameters: editInfo - pointer to EditInfo structure (note: parameter name has typo "editInfoeditinfo")
 * Return: Status (e_success/e_failure)
 */
Status skip_tag(EditInfo *editInfoeditinfo);

/*
 * Function: compare_tag
 * Description: Compares current tag identifier with user-specified tag to determine edit or skip operation
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 */
Status compare_tag(EditInfo *editInfo);

/*
 * Function: copy_remaining_data
 * Description: Copies all remaining data (including audio frames) from original file to temporary file after tag editing
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 */
Status copy_remaining_data(EditInfo *editInfo);

/*
 * Function: copy_data_from_temp_to_original_file
 * Description: Copies edited data from temporary file back to original file, replacing old content
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 */
Status copy_data_from_temp_to_original_file(EditInfo *editInfo);

/*
 * Function: close_all_file
 * Description: Closes all open file pointers (original and temporary files) and performs cleanup
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 */
Status close_all_file(EditInfo *editInfo);

#endif // End of header guard: Marks the end of conditional compilation block started by #ifndef EDIT_H