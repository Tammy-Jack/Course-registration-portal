#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_COURSES 100
#define FILENAME "courses.txt"

/* C does not have classes like C++/Java, so I used a struct
   as my simple "object" that groups a course's data together. */
struct Course {
    char code[20];
    char title[100];
    int units;
};

/* ---- Function prototypes (my "methods") ---- */
void displayMenu(void);
void addCourse(struct Course list[], int *count);
void viewCourses(struct Course list[], int count);
void searchCourse(struct Course list[], int count);
int recursiveSearch(struct Course list[], int count, char code[], int index);
void computeTotalUnits(struct Course list[], int count);
void saveToFile(struct Course list[], int count);
void loadFromFile(struct Course list[], int *count);
void toUpperCase(char str[]);
void trimSpaces(char str[]);
void clearInputBuffer(void);

int main(void) {
    struct Course courseList[MAX_COURSES];
    int courseCount = 0;
    int choice = 0;

    printf("=================================================\n");
    printf("     STUDENT COURSE MANAGEMENT SYSTEM \n");
    printf("=================================================\n");

    /* This do-while loop keeps showing the menu again and again
       (iteration) until the user chooses option 7 to exit. */
    do {
        displayMenu();
        printf("Enter your choice (1-7): ");

        /* Effective handling of invalid input: if the user types
           letters instead of a number, scanf will fail. */
        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input! Please enter a number between 1 and 7.\n\n");
            clearInputBuffer();
            choice = 0;
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addCourse(courseList, &courseCount);
                break;
            case 2:
                viewCourses(courseList, courseCount);
                break;
            case 3:
                searchCourse(courseList, courseCount);
                break;
            case 4:
                computeTotalUnits(courseList, courseCount);
                break;
            case 5:
                saveToFile(courseList, courseCount);
                break;
            case 6:
                loadFromFile(courseList, &courseCount);
                break;
            case 7:
                printf("\nExiting program. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Please select between 1 and 7.\n");
        }
        printf("\n");

    } while (choice != 7);

    return 0;
}

/* Prints the menu options to the screen. */
void displayMenu(void) {
    printf("-------------------------------------------------\n");
    printf("1. Add Course\n");
    printf("2. View All Courses\n");
    printf("3. Search Course by Code\n");
    printf("4. Compute Total Units\n");
    printf("5. Save to File\n");
    printf("6. Load from File\n");
    printf("7. Exit Program\n");
    printf("-------------------------------------------------\n");
}

/* Clears leftover characters (like the leftover newline) from the
   keyboard buffer so they do not interfere with the next input. */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* just throw the character away */
    }
}

/* Simple string processing: converts a string to uppercase so that
   course codes like "cos201" and "COS201" are treated the same. */
void toUpperCase(char str[]) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char) str[i]);
    }
}

/* Simple string processing: removes extra spaces from the front
   and back of a string (trimming). */
void trimSpaces(char str[]) {
    int start = 0;
    int end = (int) strlen(str) - 1;
    int i, j;

    while (str[start] != '\0' && isspace((unsigned char) str[start])) {
        start++;
    }
    while (end >= start && isspace((unsigned char) str[end])) {
        end--;
    }

    j = 0;
    for (i = start; i <= end; i++) {
        str[j] = str[i];
        j++;
    }
    str[j] = '\0';
}

/* Adds a new course after checking (validating) the input. */
void addCourse(struct Course list[], int *count) {
    char tempCode[20];
    char tempTitle[100];
    int tempUnits;

    if (*count >= MAX_COURSES) {
        printf("\nCourse list is full! Cannot add more courses.\n");
        return;
    }

    printf("\nEnter Course Code (e.g. COS201): ");
    fgets(tempCode, sizeof(tempCode), stdin);
    tempCode[strcspn(tempCode, "\n")] = '\0';
    trimSpaces(tempCode);
    toUpperCase(tempCode);

    if (strlen(tempCode) == 0) {
        printf("Course code cannot be empty. Course not added.\n");
        return;
    }

    printf("Enter Course Title: ");
    fgets(tempTitle, sizeof(tempTitle), stdin);
    tempTitle[strcspn(tempTitle, "\n")] = '\0';
    trimSpaces(tempTitle);

    if (strlen(tempTitle) == 0) {
        printf("Course title cannot be empty. Course not added.\n");
        return;
    }

    printf("Enter Course Units (e.g. 3): ");
    while (scanf("%d", &tempUnits) != 1 || tempUnits <= 0) {
        printf("Invalid units! Please enter a positive whole number: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    strcpy(list[*count].code, tempCode);
    strcpy(list[*count].title, tempTitle);
    list[*count].units = tempUnits;
    (*count)++;

    printf("\nCourse added successfully!\n");
}

/* Displays every course currently stored, using a loop (iteration). */
void viewCourses(struct Course list[], int count) {
    int i;

    if (count == 0) {
        printf("\nNo courses recorded yet.\n");
        return;
    }

    printf("\n%-10s %-35s %-6s\n", "CODE", "TITLE", "UNITS");
    printf("---------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%-10s %-35s %-6d\n", list[i].code, list[i].title, list[i].units);
    }
}

/* This is my recursive feature for the project.
   It searches for a course code without using a loop:
     Base case 1: index has passed the last course -> return -1 (not found)
     Base case 2: the course at 'index' matches      -> return index (found)
     Recursive case: check the next index                              */
int recursiveSearch(struct Course list[], int count, char code[], int index) {
    if (index >= count) {
        return -1;
    }
    if (strcmp(list[index].code, code) == 0) {
        return index;
    }
    return recursiveSearch(list, count, code, index + 1);
}

/* Gets a course code from the user and looks it up using recursion. */
void searchCourse(struct Course list[], int count) {
    char searchCode[20];
    int result;

    if (count == 0) {
        printf("\nNo courses to search.\n");
        return;
    }

    printf("\nEnter the Course Code to search for: ");
    fgets(searchCode, sizeof(searchCode), stdin);
    searchCode[strcspn(searchCode, "\n")] = '\0';
    trimSpaces(searchCode);
    toUpperCase(searchCode);

    result = recursiveSearch(list, count, searchCode, 0);

    if (result != -1) {
        printf("\nCourse Found!\n");
        printf("Code : %s\n", list[result].code);
        printf("Title: %s\n", list[result].title);
        printf("Units: %d\n", list[result].units);
    } else {
        printf("\nCourse with code '%s' not found.\n", searchCode);
    }
}

/* Adds up all the units using a loop (iteration). */
void computeTotalUnits(struct Course list[], int count) {
    int i;
    int total = 0;

    if (count == 0) {
        printf("\nNo courses recorded yet. Total units = 0\n");
        return;
    }

    for (i = 0; i < count; i++) {
        total += list[i].units;
    }

    printf("\nTotal number of courses: %d\n", count);
    printf("Total number of units  : %d\n", total);
}

/* Saves all courses to a text file so they can be loaded later. */
void saveToFile(struct Course list[], int count) {
    FILE *fp;
    int i;

    if (count == 0) {
        printf("\nNo courses to save.\n");
        return;
    }

    fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("\nError: Could not open file for saving.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        fprintf(fp, "%s|%s|%d\n", list[i].code, list[i].title, list[i].units);
    }

    fclose(fp);
    printf("\n%d course(s) saved successfully to '%s'.\n", count, FILENAME);
}

/* Loads courses from the text file, replacing whatever is
   currently in memory. */
void loadFromFile(struct Course list[], int *count) {
    FILE *fp;
    char line[200];
    char *token;
    int tempCount = 0;

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("\nError: No saved file found ('%s'). Save some courses first.\n", FILENAME);
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL && tempCount < MAX_COURSES) {
        token = strtok(line, "|");
        if (token == NULL) {
            continue;
        }
        strcpy(list[tempCount].code, token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            continue;
        }
        strcpy(list[tempCount].title, token);

        token = strtok(NULL, "|");
        if (token == NULL) {
            continue;
        }
        list[tempCount].units = atoi(token);

        tempCount++;
    }

    fclose(fp);
    *count = tempCount;
    printf("\n%d course(s) loaded successfully from '%s'.\n", *count, FILENAME);
}
