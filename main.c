/**
 * ----------------------------------------------------------------------------------------------------------------
 * TITLE: MP3 Tag Reader and Editor
 * AUTHOR: Manu H P
 * DATE: 27-Nov-2025
 * DESCRIPTION: Main program to read, view, and edit MP3 ID3 tag data with command-line interface.
 *              Supports viewing tags, editing specific tags, and displaying version information.
 * ----------------------------------------------------------------------------------------------------------------
 */

#include <stdio.h>   // Header file for standard input/output functions (printf, scanf, etc.)
#include <string.h>  // Header file for string manipulation functions (strcmp, strlen, strcpy, etc.)
#include "type.h"    // User-defined header file for custom type definitions (Status, e_success, e_failure)
#include "view.h"    // User-defined header file for MP3 tag viewing operations and ViewInfo structure
#include "edit.h"    // User-defined header file for MP3 tag editing operations and EditInfo structure
#include "version.h" // User-defined header file for MP3 version reading operations and VersionInfo structure

/**
 * -----------------------------------------------------------------------------------------------------------
 * INFO: DISPLAY HELP
 * -----------------------------------------------------------------------------------------------------------
 * Function: display_help
 * Description: Displays usage instructions, valid command-line options, and example commands for the user
 * Parameters: None
 * Return: void
 * -----------------------------------------------------------------------------------------------------------
 * This function prints comprehensive help information explaining how to use the MP3 Tag Reader and Editor,
 * including all valid options, their meanings, and syntax for different operations.
 * -----------------------------------------------------------------------------------------------------------
 * Sample Commands:
 *  ./a.out --help                              → Display help information
 *  ./a.out --version sample.mp3                → Display ID3 version information
 *  ./a.out -v sample.mp3                       → View all tags (Title, Artist, Album, Year, Genre, Comment)
 *  ./a.out -e -t "Song Name" sample.mp3        → Edit title tag
 *  ./a.out -e -a "Artist Name" sample.mp3      → Edit artist tag
 *  ./a.out -e -A "Album Name" sample.mp3       → Edit album tag
 *  ./a.out -e -y "2025" sample.mp3             → Edit year tag
 *  ./a.out -e -g "Pop" sample.mp3              → Edit genre tag
 *  ./a.out -e -c "My Comment" sample.mp3       → Edit comment tag
 * -----------------------------------------------------------------------------------------------------------
 */
void display_help()
{
  printf("\033[1;91mUsage: \033[1;97m./a.out [\033[1;91moptions\033[0m] \033[1;97mfilename\n"); // Display usage syntax with color formatting (red for options, white for text)

  printf("\033[1;91mOptions:\n"); // Display "Options:" header in red color

  printf("  \033[1;91m--help               \033[1;97mDisplay help\n"); // Display --help option: Shows this help menu

  printf("  \033[1;91m--version            \033[1;97mDisplay version\n"); // Display --version option: Shows ID3 version information from MP3 file

  printf("  \033[1;91m-v\033[0m                   \033[1;97mView tags\n"); // Display -v option: Views all tags in the MP3 file (TIT2, TPE1, TALB, TYER, TCON, COMM)

  // Display -e option: Edit tags with various sub-options
  // -t: Title, -a: Artist, -A: Album, -y: Year, -g: Genre, -c: Comment
  printf("  \033[1;91m-e \033[1;93m-t/-a/-A/-y/-g/-c/\033[0m \033[1;97m<\033[1;96mvalue\033[1;0m\033[1;97m>  Edit tags\n");
}

/**
 * -----------------------------------------------------------------------------------------------------------
 * INFO: DISPLAY ERROR
 * -----------------------------------------------------------------------------------------------------------
 * Function: display_error
 * Description: Displays an error message when invalid command-line arguments are passed
 * Parameters: argv - command-line argument vector (to display program name)
 * Return: void
 * -----------------------------------------------------------------------------------------------------------
 * This function is called when the user provides incorrect arguments or insufficient parameters.
 * It displays the program name and suggests using --help for proper usage instructions.
 * -----------------------------------------------------------------------------------------------------------
 */
void display_error(char **argv)
{

  printf("\033[1;91mERROR: \033[1;97m%s: Invalid Arguments\n", argv[0]); // Display error message with program name (argv[0]) in red and white color formatting

  printf("\033[1;92mUsage: \033[1;97m\"%s --help\" for help\n", argv[0]); // Suggest using --help option for correct usage information (green "Usage:", white text)
}

/**
 * --------------------------------------------------------------------------------------------------
 * INFO: MAIN FUNCTION
 * --------------------------------------------------------------------------------------------------
 * Function: main
 * Description: Entry point of MP3 Tag Reader and Editor program
 *
 * This function processes command-line arguments and routes control to appropriate functionality:
 * 1. --help      : Displays help information
 * 2. --version   : Displays ID3 version from MP3 file
 * 3. -v          : Views all tags from MP3 file
 * 4. -e          : Edits a specific tag with user-provided value
 *
 * Parameters:
 *   argc - Argument count (number of command-line arguments)
 *   argv - Argument vector (array of command-line argument strings)
 *
 * Return:
 *   0 - Success
 *   1 - Failure (invalid arguments or operation failed)
 *
 * Command-line Argument Structure:
 *   argv[0] → Program name (./a.out)
 *   argv[1] → Primary option (--help, --version, -v, -e)
 *   argv[2] → Secondary option or filename (-t, -a, -A, -y, -g, -c, or sample.mp3)
 *   argv[3] → User-provided value (for edit operation)
 *   argv[4] → Filename (for edit operation)
 *
 * Example Usage:
 *    ./a.out --help                           → Display help
 *    ./a.out --version sample.mp3             → Display ID3 version
 *    ./a.out -v sample.mp3                    → View all tags
 *    ./a.out -e -t "Hello Song" sample.mp3    → Edit title tag to "Hello Song"
 *    ./a.out -e -a "Artist" sample.mp3        → Edit artist tag to "Artist"
 * --------------------------------------------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
  // ----------------------- ARGUMENT COUNT VALIDATION -----------------------
  if (argc < 2) // Check if minimum number of arguments provided (at least program name (./a.out) + one option)
  {
    display_error(argv); // Display error message for insufficient arguments
    return 1;            // Return failure status
  }

  // ----------------------- HELP OPTION -----------------------
  if (strcmp(argv[1], "--help") == 0) // Check if user requested help information (--help)
  {
    display_help(); // Display comprehensive help information
    return 0;       // Return success status after displaying help
  }

  /**
   * ----------------------- FILENAME VALIDATION -----------------------
   * Check if options that require a filename have sufficient arguments
   * Most options (--version, -v, -e) need at least 3 arguments
   */
  if (argc < 3)
  {
    display_error(argv); // Display error if filename not provided
    return 1;            // Return failure status (added missing return)
  }

  /**
   * ----------------------- VERSION OPTION -----------------------
   * Check if user wants to view ID3 version (--version) with exactly 3 arguments
   * Expected: ./a.out --version sample.mp3
   */
  else if (strcmp(argv[1], "--version") == 0 && argc == 3)
  {
    VersionInfo VERInfo; // Declare VersionInfo structure to store version data

    if (read_and_validate_for_version(argv, &VERInfo) == e_failure) // Validate command-line arguments for version operation
    {
      return 0; // Return if validation fails (file not found or invalid)
    }
    else
    {
      version_read(&VERInfo); // Read and display ID3 version information from MP3 file
    }
  }

  /**
   * ----------------------- VIEW TAG OPTION -----------------------
   * Check if user wants to view all tags (-v) with exactly 3 arguments
   * Expected: ./a.out -v sample.mp3
   */
  else if (strcmp(argv[1], "-v") == 0 && argc == 3)
  {
    ViewInfo viInfo; // Declare ViewInfo structure to store tag information

    if (read_and_validate_for_view(argv, &viInfo) == e_failure) // Validate command-line arguments for view operation
    {
      return 0; // Return if validation fails (file not found or invalid format)
    }
    else
    {
      view_tags(&viInfo); // View all tags (TITLE, ARTIST, ALBUM, YEAR, GENRE, COMMENT) from MP3 file
    }
  }
  /**
   * ----------------------- EDIT TAG OPTION -----------------------
   * Check if user wants to edit a specific tag (-e) with exactly 5 arguments
   * Expected: ./a.out -e -t "New Title" sample.mp3
      argv[1] = "-e" (edit mode)
      argv[2] = "-t/-a/-A/-y/-g/-c" (tag specifier)
      argv[3] = "New Value" (user-provided content)
      argv[4] = "sample.mp3" (filename)
  */
  else if (strcmp(argv[1], "-e") == 0 && argc == 5)
  {
    EditInfo editInfo; // Declare EditInfo structure to store edit information

    // Validate command-line arguments for edit operation
    // This maps tag flags (-t, -a, -A, -y, -g, -c) to ID3 tags (TIT2, TPE1, TALB, TYER, TCON, COMM)
    if (read_and_validate_for_edit(argv, &editInfo) == e_failure)
    {

      printf("\033[1;91mFailed to edit tag.\033[0m\n"); // Display error message if validation fails (invalid flag or file)
      return 1;                                         // Return failure status
    }
    else
    {
      do_edit_tags(&editInfo); // Perform tag editing operation (opens files, edits tag, writes back)
    }

    printf("\033[1;97mTag edited successfully.\n"); // Display success message after tag editing completes
  }

  // ----------------------- INVALID OPTION -----------------------
  // Handle any invalid or unrecognized command-line options
  else
  {
    display_error(argv); // Display error message for invalid arguments
    return 1;            // Return failure status
  }

  return 0; // Return success status if program executed successfully
}
