// Xiao Lin, CSE 374, HW4, 10/25/2017
/* This program reads the listed files and copies each line from the input to stdout
   if it contains the search pattern anywhere in the input line.
   User can choose to ignore case as well as include line number*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 500  // Handle input lines up to 500 chars
#define MAX_STRING_PATTERN 100   // Search term is no longer than 100 chars

// Retusn a lowercase copy of the string
void convert_to_lower_case(char *original_line, char* buf);
/* Prints the file name and the given line to stdout,
   including the line number if include_line_num is 1 */
void print_line(int line_num, char *file_name, char *line);
/* Reads each line of the file, and if that line contains the search pattern,
   print the line out along with the name of the file and possibly the line number */
void process_file(char *file_name, char *search_pattern);
/* If user inputs either -i and/or -n, change the global variables ignore_case and/or
   include_line_num to 1 */
int check_options(char *argv[]);

// Global variables to store user's option choices
int ignore_case = 0;
int include_line_num = 0;

int main(int argc, char *argv[]) {
  /* There must be a search pattern and at least 1 file in the arguments.
     Otherwise, print error message to stderror and exit */
  if (argc < 3) {
    fprintf(stderr, "%s need a search term and at least 1 file.\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int option_count = check_options(argv);
  // search pattern comes right after the options in the arguments
  char *search_pattern = argv[option_count + 1];
  char search_pattern_copy[MAX_STRING_PATTERN];
  strncpy(search_pattern_copy, search_pattern, MAX_STRING_PATTERN);
  search_pattern_copy[strlen(search_pattern)] = '\0';

  // char *search_pattern_copy = (char*)search_pattern;
  // If option "-i" is used, convert the search pattern to lowercase
  if (ignore_case == 1) {
    convert_to_lower_case(search_pattern, search_pattern_copy);
  }

  int i;   // Index to go through all the file names
  /* The first file name is right after the search pattern
     and two arguments away from the last option */
  for (i = option_count + 2; i < argc; i++) {
    char *file_name = argv[i];
    process_file(file_name, search_pattern_copy);
  }
  exit(EXIT_SUCCESS);
}

int check_options(char *argv[]) {
  int option_count = 0;
  char *case_insensitive = "-i";
  char *include_line_number = "-n";
  if (strcmp(argv[1], case_insensitive) == 0
      || strcmp(argv[2], case_insensitive) == 0) {
    ignore_case = 1;
    option_count++;
  }
  if (strcmp(argv[1], include_line_number) == 0
      || strcmp(argv[2], include_line_number) == 0) {
    include_line_num = 1;
    option_count++;
  }
  return option_count;
}

void process_file(char *file_name, char *search_pattern) {
  FILE *ifp = fopen(file_name, "r");
  // If err occurs when opening the file, write err msge to stderr and exit
  if (ifp  == NULL) {
    fprintf(stderr, "%s Can't open %s\n", file_name, file_name);
    exit(EXIT_FAILURE);
  } else {
    int line_number = 1;
    char line[MAX_LINE_LENGTH];
    while (!feof(ifp)) {
      // If err occurs when reading file, write err msg to stderr and exit
      if (fgets(line, MAX_LINE_LENGTH, ifp) == NULL) {
        fprintf(stderr, "%s Can't read line#%d\n", file_name, line_number);
        exit(EXIT_FAILURE);
      } else {
        if (ignore_case == 1) {
          char line_copy[MAX_LINE_LENGTH];
          strncpy(line_copy, line, MAX_LINE_LENGTH);
          line_copy[strlen(line)] = '\0';
          convert_to_lower_case(line, line_copy);
	  /* If -n option is not used, first argument will be 0.
	     Otherwise, it will be the line_number of the current line*/
          if (strstr(line_copy, search_pattern)) {
            print_line((line_number*include_line_num), file_name, line_copy);
          }
        } else {
          if (strstr(line, search_pattern)) {
            print_line((line_number*include_line_num), file_name, line);
          }
        }
        line_number++;
      }
    }
  }
    // Reached end of file, close it
    fclose(ifp);
}

void convert_to_lower_case(char *original_line, char *buf) {
  int i = 0;
  while ( original_line[i] ) {
    buf[i] = tolower(original_line[i]);
    i++;
  }
}

void print_line(int line_num, char *file_name, char *line) {
  if (line_num > 0) {
    printf("%s %d %s", file_name, line_num, line);
  } else {
    printf("%s %s", file_name, line);
  }
  printf("\n");
}
