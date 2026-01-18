#ifndef TYPE_H     // if not defined TYPE_H ---> Checks if CONTACT_H previously present or not: If not present, code is processed else code below till #endif is ignored
#define TYPE_H     // defines the macro TYPE_H if macro not
#include <stdio.h> //Header file for standard input and output (printf(), scanf(), etc.)
typedef enum // typedef used to give alternate name for enum here
{
  e_help,        // e_help is first member of OperationType with default value 0
  e_view,        // e_view is second member of OperationType with default value 1 with reference of previous member's value
  e_edit,        // e_edit is third member of OperationType with default value 2 with reference of previous member's value
  e_unsupported  // e_unsupported is fourth member of OperationType with default value 3 with reference of previous member's value
} OperationType; // OperationType is alternate name for this enum

typedef enum // typedef used to give alternate name for enum here
{
  e_success, // e_success is the first member of Status with default value 0
  e_failure // e_failure is the second member of Status with default value 1 with reference of previous member's value
} Status; // Status is alternate name for this enum

#endif // End of header guard: This preprocessor directive is used to signal the end any conditional block
