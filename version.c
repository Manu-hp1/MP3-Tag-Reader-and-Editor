#include "version.h" // Include version header file for VersionInfo structure and function declarations
#include <stdio.h>   // Header file for standard input/output functions (printf, fprintf, fopen, fread, fwrite, fseek, etc.)
#include <string.h>  // Header file for string manipulation functions (strcmp, strstr, strlen, etc.)
#include <stdlib.h>  // Header file for memory allocation functions (malloc, free, etc.)
#include "type.h"    // User-defined header file for custom type definitions (Status, e_success, e_failure)

/*
 * Function: open_files_for_version
 * Description: Opens the source MP3 file in read mode and performs error handling
 * Parameters: VERInfo - pointer to VersionInfo structure containing filename
 * Return: Status (e_success/e_failure)
 */
Status open_files_for_version(VersionInfo *VERInfo)
{
  VERInfo->fptr_src = fopen(VERInfo->src_fname, "r"); // Open source MP3 file (e.g., sample.mp3) in read mode

  if (VERInfo->fptr_src == NULL) // Error handling: Check if file pointer is NULL (file opening failed)
  {
    perror("fopen"); // Print system error message for file opening failure

    fprintf(stderr, "\033[1;91mERROR: Unable to open file %s\033[0m\n", VERInfo->src_fname); // Print custom error message with red color formatting indicating unable to open file
    return e_failure;                                                                        // Return failure status
  }
  return e_success; // Return success if file opened successfully
}

/*
 * Function: read_and_validate_for_version
 * Description: Validates command-line arguments for version operation, checks for .mp3 extension and valid filename
 * Parameters: argv[] - command-line argument array, VERInfo - pointer to VersionInfo structure
 * Return: Status (e_success/e_failure)
 */
Status read_and_validate_for_version(char *argv[], VersionInfo *VERInfo)
{
  if (argv[2][0] != '.') // Validate that source filename doesn't start with '.' (hidden file or invalid format)
  {
    if (strstr(argv[2], ".mp3")) // Check if ".mp3" extension is present in the source filename
    {
      // Step 2: Store source filename in VersionInfo structure
      VERInfo->src_fname = argv[2]; // Copy source filename (e.g., sample.mp3) to VERInfo structure
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
 * Function: version_read
 * Description: Reads ID3 version from MP3 file header and displays formatted version information
 * Parameters: VERInfo - pointer to VersionInfo structure
 * Return: Status (e_success/e_failure)
 */
Status version_read(VersionInfo *VERInfo)
{
  if (open_files_for_version(VERInfo) == e_failure) // Open the source MP3 file for reading
  {
    return e_failure; // Return failure if file opening fails
  }

  VERInfo->version = (char *)malloc(3 * sizeof(char)); // Allocate memory for 3 bytes to store ID3 version identifier

  fread(VERInfo->version, 1, 3, VERInfo->fptr_src); // Read first 3 bytes from MP3 file to check for "ID3" header

  char size;
  fread(&size, 1, 1, VERInfo->fptr_src); // Read 1 byte to get the version number (e.g., version 2, 3, or 4)

  if (strcmp(VERInfo->version, "ID3") != 0) // Validate if the file contains "ID3" tag header
  {
    printf("\033[1;91mERROR: \033[1;97mInvalid source file without filename without ID3\n"); // Print error message if ID3 tag not found (invalid MP3 file format)
    return e_failure;                                                                        // Return failure if ID3 tag not present
  }

  printf("\033[1;97m\n▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌\n");
  printf("▐ \033[1;7;93m%-41c\033[1;92m %s \033[0m\033[1;7;93m%-46c\033[0m\033[1;97m ▌\n", ' ', "MP3 Tag Reader and Editor", ' ');
  printf("\033[1;97m▐▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▌\n");
  printf("▐%-48c\033[1;7m VERSION \033[0m\033[1;3m : %sv2.%-50d▌\n\033[0m", ' ', VERInfo->version, size);
  printf("\033[1;97m▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌\n\n");

  free(VERInfo->version); // Free allocated memory for version string

  fseek(VERInfo->fptr_src, 7, SEEK_CUR); // Skip next 7 bytes in the file (remaining ID3 header information: revision, flags, and size)

  return e_success; // Return success after displaying version information
}
