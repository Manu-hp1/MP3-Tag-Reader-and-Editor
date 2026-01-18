#include <stdio.h>  // Header file for standard input/output functions (printf, fprintf, fopen, fread, fwrite, fseek, ftell, rewind, etc.)
#include <string.h> // Header file for string manipulation functions (strcmp, strcpy, strlen, strstr, etc.)
#include <stdlib.h> // Header file for memory allocation and utility functions (malloc, free, tmpfile, etc.)
#include "type.h"   // User-defined header file for custom type definitions (Status, e_success, e_failure)
#include "edit.h"   // User-defined header file for EditInfo structure and function declarations

/*
 * Function: read_and_validate_for_edit
 * Description: Reads and validates command-line arguments for edit operation, maps user flags to ID3v2.3 and ID3v2.4 tags
 * Parameters: argv[] - command-line argument array, editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 *
 * Flag mappings:
 * -t : TITLE  (TIT2)
 * -a : ARTIST (TPE1)
 * -y : YEAR   (TYER)
 * -A : ALBUM  (TALB)
 * -g : GENRE  (TCON)
 * -c : COMMENT(COMM)
 */
Status read_and_validate_for_edit(char *argv[], EditInfo *editInfo)
{
  editInfo->mode = (char *)malloc(5 * sizeof(char)); // Allocate memory for 5 bytes to store tag mode identifier (4 chars + null terminator)

  if (strcmp(argv[2], "-t") == 0) // Check if user wants to edit TITLE tag (-t flag)
  {
    strcpy(editInfo->user_tag, "TITLE"); // Copy "TITLE" to user_tag array
    editInfo->user_tag[6] = '\0';        // Null-terminate the string (length 5 + '\0')
    strcpy(editInfo->mode, "TIT2");      // Store corresponding ID3v2 tag identifier
    editInfo->mode[4] = '\0';            // Null-terminate mode string
  }
  else if (strcmp(argv[2], "-a") == 0) // Check if user wants to edit ARTIST tag (-a flag)
  {
    strcpy(editInfo->user_tag, "ARTIST"); // Copy "ARTIST" to user_tag array
    editInfo->user_tag[7] = '\0';         // Null-terminate the string (length 6 + '\0')
    strcpy(editInfo->mode, "TPE1");       // Store corresponding ID3v2 tag identifier
    editInfo->mode[4] = '\0';             // Null-terminate mode string
  }
  else if (strcmp(argv[2], "-y") == 0) // Check if user wants to edit YEAR tag (-y flag)
  {
    strcpy(editInfo->user_tag, "YEAR"); // Copy "YEAR" to user_tag array
    editInfo->user_tag[5] = '\0';       // Null-terminate the string (length 4 + '\0')
    strcpy(editInfo->mode, "TYER");     // Store corresponding ID3v2 tag identifier
    editInfo->mode[4] = '\0';           // Null-terminate mode string
  }
  else if (strcmp(argv[2], "-A") == 0) // Check if user wants to edit ALBUM tag (-A flag)
  {
    strcpy(editInfo->user_tag, "ALBUM"); // Copy "ALBUM" to user_tag array
    editInfo->user_tag[6] = '\0';        // Null-terminate the string (length 5 + '\0')
    strcpy(editInfo->mode, "TALB");      // Store corresponding ID3v2 tag identifier
    editInfo->mode[4] = '\0';            // Null-terminate mode string
  }
  else if (strcmp(argv[2], "-g") == 0) // Check if user wants to edit GENRE tag (-g flag)
  {
    strcpy(editInfo->user_tag, "GENRE"); // Copy "GENRE" to user_tag array
    editInfo->user_tag[6] = '\0';        // Null-terminate the string (length 5 + '\0')
    strcpy(editInfo->mode, "TCON");      // Store corresponding ID3v2 tag identifier
    editInfo->mode[4] = '\0';            // Null-terminate mode string
  }

  else if (strcmp(argv[2], "-c") == 0) // Check if user wants to edit COMMENT tag (-c flag)
  {
    strcpy(editInfo->user_tag, "COMMENT"); // Copy "COMMENT" to user_tag array
    editInfo->user_tag[8] = '\0';          // Null-terminate the string (length 7 + '\0')
    strcpy(editInfo->mode, "COMM");        // Store corresponding ID3v2 tag identifier
    editInfo->mode[4] = '\0';              // Null-terminate mode string
  }
  else
  {
    printf("\033[1;97mWrong TAG passed!\n"); // Print error message for invalid flag
    return e_failure;                        // Return failure status
  }

  editInfo->user_content = malloc((strlen(argv[3]) + 1) * sizeof(char)); // Allocate memory for user-provided content (new tag value) based on its length
  strcpy(editInfo->user_content, argv[3]);                               // Copy user-provided content from command-line argument to allocated memory

  int i = 0;
  while (editInfo->user_content[i])
  {
    i++; // Increment index until null character found
  }
  editInfo->user_content[i] = '\0'; // Ensure null termination

  if (argv[4][0] != '.') // Validate that source filename doesn't start with '.' (hidden file or invalid format)
  {
    if (strstr(argv[4], ".mp3")) // Check if ".mp3" extension is present in the source filename
    {
      // Step 2: Store source MP3 filename in EditInfo structure
      editInfo->original_fname = argv[4]; // Copy source filename (e.g., sample.mp3)
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
 * Function: open_file
 * Description: Opens original MP3 file in read-write mode and creates a temporary file for editing
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 */
Status open_file(EditInfo *editInfo)
{

  editInfo->fptr_original = fopen(editInfo->original_fname, "r+"); // Open source MP3 file (e.g., sample.mp3) in read and write mode (r+)

  if (editInfo->fptr_original == NULL) // Error handling: Check if original file pointer is NULL (file opening failed)
  {
    perror("fopen"); // Print system error message for file opening failure
    // Print custom error message with red color formatting
    fprintf(stderr, "\033[1;91mERROR: Unable to open file %s\033[0m\n", editInfo->original_fname);
    return e_failure; // Return failure status
  }

  /**
   * INFO:
   * tmpfile() creates and opens a temporary file in read-write mode automatically.
   * The file is deleted automatically when closed or when the program ends.
   * This is safer than manually creating/deleting temp files.
   */
  editInfo->fptr_temp = tmpfile();

  return e_success; // Return success if both files opened successfully
}

/*
 * Function: copy_header_edit
 * Description: Copies the 10-byte ID3v2 header from original file to temporary file
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 *
 * ID3v2 Header structure (10 bytes):
 * - 3 bytes: "ID3" identifier
 * - 2 bytes: Version (major.minor)
 * - 1 byte:  Flags
 * - 4 bytes: Tag size (synchsafe integer)
 */
Status copy_header_edit(EditInfo *editInfo)
{
  char arr[10]; // Array to store 10-byte ID3v2 header

  fread(arr, 1, 10, editInfo->fptr_original); // Read first 10 bytes (header) from original file

  fwrite(arr, 1, 10, editInfo->fptr_temp); // Write the 10-byte header to temporary file

  if (ftell(editInfo->fptr_original) == ftell(editInfo->fptr_temp)) // Verify both file pointers are at the same position (byte 10) after copying
  {
    return e_success; // Return success if positions match
  }
  return e_failure; // Return failure if positions don't match (copy error)
}

/*
 * Function: convert_big_to_little_endian_for_edit
 * Description: Converts 4-byte integer from big-endian to little-endian format (byte reversal)
 * Parameters: arr - pointer to integer (treated as 4-byte array for byte manipulation)
 * Return: void
 *
 * Conversion: [0][1][2][3] → [3][2][1][0]
 */

void convert_big_to_little_endian_for_edit(int *arr)
{
  char *p = (char *)arr; // Cast integer pointer to char pointer for byte-level access
  char temp;             // Temporary variable for swapping bytes

  // Swap byte 0 with byte 3
  temp = p[0];
  p[0] = p[3];
  p[3] = temp;

  // Swap byte 1 with byte 2
  temp = p[1];
  p[1] = p[2];
  p[2] = temp;
}

/*
 * Function: skip_tag
 * Description: Copies a tag frame without modification when it doesn't match the target tag
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 *
 * Frame structure:
 * - 4 bytes: Tag identifier (e.g., TIT2, TPE1)
 * - 4 bytes: Size
 * - 3 bytes: Flags
 * - N bytes: Content (N = size - 1)
 */
Status skip_tag(EditInfo *editInfo)
{
  char tag[5]; // Array to store 4-byte tag identifier + null terminator

  fseek(editInfo->fptr_original, -4, SEEK_CUR); // Move file pointer back 4 bytes (called after reading tag in compare_tag)

  fread(tag, 1, 4, editInfo->fptr_original); // Read 4-byte tag identifier from original file

  fwrite(tag, 1, 4, editInfo->fptr_temp); // Write tag identifier to temporary file

  int size[1]; // Array to store tag size (4 bytes)

  fread(size, 1, 4, editInfo->fptr_original); // Read 4-byte size field from original file

  fwrite(size, 1, 4, editInfo->fptr_temp); // Write size field to temporary file

  convert_big_to_little_endian_for_edit(&size[0]); // Convert size from big-endian to little-endian to get actual value

  char flag[3]; // Array to store 3-byte flags field

  fread(flag, 1, 3, editInfo->fptr_original); // Read 3-byte flags from original file

  fwrite(flag, 1, 3, editInfo->fptr_temp); // Write flags to temporary file

  char content[size[0]]; // Array to store tag content based on size

  fread(content, 1, size[0] - 1, editInfo->fptr_original); // Read content (size-1 bytes) from original file

  fwrite(content, 1, size[0] - 1, editInfo->fptr_temp); // Write content to temporary file

  if (ftell(editInfo->fptr_original) == ftell(editInfo->fptr_temp)) // Verify both file pointers are at the same position after copying
  {
    return e_success; // Return success if positions match
  }
  return e_failure; // Return failure if copying 10 bytes of header from original file to temporary file failed
}

/*
 * Function: copy_remaining_data
 * Description: Copies all remaining data (tags and audio frames) from original to temporary file
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 */
Status copy_remaining_data(EditInfo *editInfo)
{
  char ch; // Variable to store one byte at a time

  while (fread(&ch, 1, 1, editInfo->fptr_original) == 1) // Read one byte at a time until end of file
  {
    fwrite(&ch, 1, 1, editInfo->fptr_temp); // Write each byte to temporary file
  }

  return e_success; // Return failure after copying all remaining data
}

/*
 * Function: do_edit
 * Description: Edits the target tag by replacing old content with user-provided content
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 *
 * Process:
 * 1. Copy tag identifier
 * 2. Replace old size with new size (based on user content length)
 * 3. Copy flags
 * 4. Write new user content
 * 5. Skip old content in original file
 * 6. Copy remaining data
 */
Status do_edit(EditInfo *editInfo)
{
  char tag[5]; // Array to store 4-byte tag identifier + null terminator

  fseek(editInfo->fptr_original, -4, SEEK_CUR); // Move file pointer back 4 bytes (called after reading tag in compare_tag)

  fread(tag, 1, 4, editInfo->fptr_original); // Read 4-byte tag identifier from original file

  fwrite(tag, 1, 4, editInfo->fptr_temp); // Write tag identifier to temporary file (keeping same tag name)

  int size[1]; // Array to store old tag size (4 bytes)

  fread(size, 1, 4, editInfo->fptr_original); // Read old size from original file

  convert_big_to_little_endian_for_edit(&size[0]); // Convert old size from big-endian to little-endian to get actual value

  editInfo->old_size = size[0]; // Store old size for later use

  editInfo->user_content_size = strlen(editInfo->user_content) + 1; // Calculate new size: user content length + 1 (for encoding byte)

  convert_big_to_little_endian_for_edit(&editInfo->user_content_size); // Convert new size from little-endian to big-endian format for writing to file

  int new_size[1];
  new_size[0] = editInfo->user_content_size;

  fwrite(new_size, 1, 4, editInfo->fptr_temp); // Write new size to temporary file (in big-endian format)

  char flag[3]; // Array to store 3-byte flags field

  fread(flag, 1, 3, editInfo->fptr_original); // Read flags from original file

  fwrite(flag, 1, 3, editInfo->fptr_temp); // Write flags to temporary file (keeping same flags)

  fwrite(editInfo->user_content, 1, strlen(editInfo->user_content), editInfo->fptr_temp); // Write new user-provided content to temporary file

  fseek(editInfo->fptr_original, editInfo->old_size - 1, SEEK_CUR); // Skip old content in original file (move pointer past old content)

  if (copy_remaining_data(editInfo) == e_success) // Copy all remaining data (other tags and audio) to temporary file
  {
    return e_success; // Return success if remaining data copied successfully
  }

  printf("ERROR: Copying remaining data failed\n");
  return e_failure; // Return failure if copying remaining data failed
}

/*
 * Function: compare_tag
 * Description: Compares each tag in the file with target tag and calls edit or skip accordingly
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 *
 * Process: Loops through first 6 tags (TIT2, TPE1, TALB, TYER, TCON, COMM)
 */
Status compare_tag(EditInfo *editInfo)
{
  editInfo->TAG = malloc(5 * sizeof(char)); // Allocate memory for 5 bytes to store tag identifier (4 bytes + null terminator)

  for (int i = 0; i < 6; i++) // Loop through first 6 tags in the MP3 file
  {
    fread(editInfo->TAG, 1, 4, editInfo->fptr_original); // Read 4-byte tag identifier from original file
    editInfo->TAG[5] = '\0';                             // Null-terminate the tag string

    if (strcmp(editInfo->TAG, editInfo->mode) == 0) // Compare current tag with user-specified target tag (mode)
    {
      do_edit(editInfo);   // If tags match, edit this tag with user content
      free(editInfo->TAG); // Free allocated memory for TAG identifier
      return e_success;    // Return success after editing target tag
    }
    else
    {
      skip_tag(editInfo); // If tags don't match, skip (copy without modification)
    }
  }
}

/*
 * Function: close_all_file
 * Description: Closes all open file pointers and frees allocated memory
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (implied success)
 */
Status close_all_file(EditInfo *editInfo)
{
  free(editInfo->user_content); // Free memory allocated for user content

  fclose(editInfo->fptr_temp);     // Close temporary file pointer
  fclose(editInfo->fptr_original); // Close original file pointer

  return e_success;
}

/*
 * Function: do_edit_tags
 * Description: Main orchestration function for tag editing - coordinates all editing operations
 * Parameters: editInfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 *
 * Workflow:
 * 1. Display selected tag for editing
 * 2. Open files (original and temporary)
 * 3. Copy ID3v2 header
 * 4. Compare and edit target tag
 * 5. Copy edited data back to original file
 * 6. Close all files and cleanup
 */
Status do_edit_tags(EditInfo *editInfo)
{

  printf("\033[1;97mSELECTED FOR EDITING \033[1;92m%s\n", editInfo->user_tag); // Display which tag is selected for editing with green color formatting

  if (open_file(editInfo) == e_failure) // Open original file (r+) and create temporary file
  {
    return e_failure; // Return failure if file opening fails
  }

  if (copy_header_edit(editInfo) == e_failure) // Copy 10-bytes header from original to temporary file
  {
    return e_failure; // Return failure if header copy fails
  }

  if (compare_tag(editInfo) == e_failure) // Compare tags and perform edit operation on target tag
  {
    return e_failure; // Return failure if tag comparison/editing fails
  }

  copy_data_from_temp_to_original_file(editInfo); // Copy all edited data from temporary file back to original file

  close_all_file(editInfo); // Close all files and free allocated memory
  return e_success;         // Return success if all edit operations are done successfully
}

/**
 * --------------------------------------------------------------------------------------
 * INFO: COPY ENTIRE DATA FROM TEMPORARY FILE TO ORIGINAL FILE
 * --------------------------------------------------------------------------------------
 * This function overwrites the original file with the edited content from temp file
 */

/*
 * Function: copy_data_from_temp_to_original_file
 * Description: Copies all edited data from temporary file back to original file, replacing old content
 * Parameters: editinfo - pointer to EditInfo structure
 * Return: Status (e_success/e_failure)
 *
 * Process:
 * 1. Rewind both file pointers to beginning
 * 2. Copy byte-by-byte from temp to original
 * 3. Original file now contains edited data
 */
Status copy_data_from_temp_to_original_file(EditInfo *editInfo)
{
  rewind(editInfo->fptr_original); // Move original file pointer to the beginning of the file

  rewind(editInfo->fptr_temp); // Move temporary file pointer to the beginning of the file

  char ch; // Variable to store one byte at a time

  while (fread(&ch, 1, 1, editInfo->fptr_temp) == 1) // Read one byte at a time from temporary file until end of file
  {

    fwrite(&ch, 1, 1, editInfo->fptr_original); // Write each byte to original file (overwriting old content)
  }
  if (ftell(editInfo->fptr_original) == ftell(editInfo->fptr_temp)) // Verify both file pointers are at the same position after copying
  {
    return e_success; // Return success if positions match
  }
  printf("\033[1;91mCopying remaining data failed\033[1;97m\n");
  return e_failure; // Return failure if copying all data back to original file failed
}