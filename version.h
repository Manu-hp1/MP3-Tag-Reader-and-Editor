#ifndef VERSION_H // If not defined VERSION_H ---> Checks if VERSION_H was previously defined: If not present, code is processed; else code below till #endif is ignored
#define VERSION_H // Defines the macro VERSION_H if macro was not previously defined

#include "type.h"  // User-defined header file for custom type definitions (Status, e_success, e_failure)
#include <stdio.h> // Header file for standard input and output (printf(), scanf(), fopen(), fread(), fseek(), etc.)

// Structure to store MP3 file version information including ID3 tag and file pointers
typedef struct // typedef used to give alternate name for structure here
{
  char *version;   // Pointer to store ID3 version identifier (e.g., "ID3")
  char *src_fname; // Pointer to store source MP3 filename (e.g., sample.mp3)
  FILE *fptr_src;  // File pointer to store address/handle of the source MP3 file for reading
} VersionInfo;     // VersionInfo is alternate name for this structure

/*
 * Function: read_and_validate_for_version
 * Description: Reads and validates command-line arguments for version checking, ensures valid .mp3 file
 * Parameters: argv[] - command-line argument array, VERInfo - pointer to VersionInfo structure
 * Return: Status (e_success/e_failure)
 */
Status read_and_validate_for_version(char *argv[], VersionInfo *VERInfo);

/*
 * Function: open_files_for_version
 * Description: Opens the source MP3 file in read mode and validates the file pointer
 * Parameters: VERInfo - pointer to VersionInfo structure containing file information
 * Return: Status (e_success/e_failure)
 */
Status open_files_for_version(VersionInfo *VERInfo);

/*
 * Function: version_read
 * Description: Reads and displays the ID3 version information from the MP3 file header
 * Parameters: VERInfo - pointer to VersionInfo structure
 * Return: Status (e_success/e_failure)
 */
Status version_read(VersionInfo *VERInfo);

#endif // End of header guard: Marks the end of conditional compilation block started by #ifndef VERSION_H