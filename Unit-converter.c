#include<stdio.h>
#include <string.h>

#define FILENAME_TYPES "Types.dat"
#define FILENAME_HISTORY "history.dat"
#define MAX_UNITNAME_CHAR_COUNT 20
#define MAX_UNITS_IN_FILE 100
#define ADMIN_PASSWORD "123456"

void takeValidatedInput(int* inputPtr, int maxValue){
    while(1){
        scanf("%d", inputPtr);
        if(*inputPtr > 0 && *inputPtr <= maxValue)
            break;

        printf("Value must be between 1 and %d. Please try again: ", maxValue);
    }
}

void print_types()
{
    int count;
    char unit[15];
    FILE* file;
    file = fopen(FILENAME_TYPES, "r");
    while(!feof(file))
    {
        fscanf(file, "%d%s", &count, &unit);
        printf("%d  %s\n", count, unit);
    }
    fclose(file);
}

void convertSimpleMultiple(const char* conversionType)
{
    //Everything is converted into meter and then to the respective unit.
    int i=1, from, to;
    char unitName[MAX_UNITS_IN_FILE][MAX_UNITNAME_CHAR_COUNT];
    double multiple[MAX_UNITS_IN_FILE], inverseMultiple[MAX_UNITS_IN_FILE];
    double query, middle=0, result=0;
    FILE* file = fopen(conversionType, "r");
    if(file == NULL)
    {
        printf("File could not be opened: %s\n", conversionType);
        fclose(file);
        return;
    }
        while(!feof(file))
        {
            fscanf(file, "%s%lf", &unitName[i], &multiple[i]);
            printf("%d. %s\n", i, unitName[i]);
            inverseMultiple[i] = 1.0 / multiple[i];
            i++;
        }
        printf("From: ");
        takeValidatedInput(&from, i-1);
        printf("To: ");
        takeValidatedInput(&to, i-1);
        printf("How many %ss: ", unitName[from]);
        scanf("%lf", &query);

        middle = query * multiple[from];
        result = middle * inverseMultiple[to];

        char resultString[100];

        sprintf(resultString, "Result: %.2f %ss = %.2f %ss", query, unitName[from], result, unitName[to]);

        printf("===========================================\n");
        printf("%s\n", resultString);
        printf("===========================================\n");

        addHistoryEntry(resultString);
    fclose(file);

}


void convertMultipleWithConstant(const char* conversionType)
{
    //Everything is converted into Celsius and then to the respective unit.
    int i=1, from, to;
    double  m[MAX_UNITS_IN_FILE],
            c[MAX_UNITS_IN_FILE],
            mInverse[MAX_UNITS_IN_FILE],
            cInverse[MAX_UNITS_IN_FILE];
    double query, middle=0, result=0;
    char unitName[MAX_UNITS_IN_FILE][MAX_UNITNAME_CHAR_COUNT];
    FILE* file = fopen(conversionType, "r");
    if(file == NULL)
    {
        printf("File could not be opened: %s.\n", conversionType);
    }
    else
    {

        while(!feof(file))
        {

            fscanf(file, "%s%lf%lf", &unitName[i], &m[i], &c[i]);
            printf("%d. %s\n", i, unitName[i]);
            mInverse[i] = 1.0 / m[i];
            cInverse[i] = (-1.0) * c[i] / m[i];
            i++;
        }
        printf("From: ");
        takeValidatedInput(&from, i-1);
        printf("To: ");
        takeValidatedInput(&to, i-1);
        printf("How many %ss: ", unitName[from]);
        scanf("%lf", &query);

        middle = query * m[from] + c[from];
        result = middle * mInverse[to] + cInverse[to];
        char resultString[100];
        sprintf(resultString, "Result: %.2f %ss = %.2f %ss", query, unitName[from], result, unitName[to]);

        printf("===========================================\n");
        printf("%s\n", resultString);
        printf("===========================================\n");

        addHistoryEntry(resultString);
    }
    fclose(file);
}

void addHistoryEntry(char* text){
    FILE* historyFile = fopen(FILENAME_HISTORY, "a");
    if(historyFile == NULL){
        printf("Can't add to history because history file cannot be opened.\n");
        return;
    }

    char timeString[100];

time_t t = time(NULL);
struct tm * p = localtime(&t);

strftime(timeString, 100, "%I:%M:%S %p %d %b %Y", p);

    fprintf(historyFile, "[%s]\t%s\n", timeString, text);

    fclose(historyFile);

    printf("History entry added\n\n\n");

}



void addNewSimpleMultipleConversionRule(const char* conversionType){
    char unitName[MAX_UNITNAME_CHAR_COUNT];
    double multiple;

    printf("Enter unit name: ");
    scanf("%s", unitName);

    printf("Enter co-efficient for converting from base unit to this unit: ");
    scanf("%lf", &multiple);

    FILE* file = fopen(conversionType, "a");
    if(file == NULL){
        printf("Can't open file: %s.\n", conversionType);
        return;
    }

    fprintf(file, "\n%s\t%f", unitName, multiple);

    fclose(file);

    printf("Rule added successfully.\n\n");

}

void addNewMultipleAndConstantConversionRule(const char* conversionType){
    char unitName[MAX_UNITNAME_CHAR_COUNT];
    double multiple;
    double constant;

    printf("Enter unit name: ");
    scanf("%s", unitName);

    printf("Enter co-efficient for converting from base unit to this unit: ");
    scanf("%lf", &multiple);

    printf("Enter constant: ");
    scanf("%lf", &constant);

    FILE* file = fopen(conversionType, "a");
    if(file == NULL){
        printf("Can't open file: %s.\n", conversionType);
        return;
    }

    fprintf(file, "\n%s\t%f\t%f", unitName, multiple, constant);

    fclose(file);

    printf("Rule added successfully.\n\n");

}

void adminMode(){

    int request;

    system("cls");
    system("COLOR c0");
    printf("================== ADMIN MODE ========================\n");

    while(1){

    printf("What type of unit conversion rule do you want to add?\n\n");
    print_types();
    printf("(Press -1 to exit admin mode)\n");
    printf("Select a number from above: \n");
    scanf("%d",&request);

        switch(request)
        {
        case 1:
            addNewSimpleMultipleConversionRule("Length.dat");
            break;

        case 2:
            addNewSimpleMultipleConversionRule("Weight.dat");
            break;

        case 3:
            addNewSimpleMultipleConversionRule("Time.dat");
            break;

        case 4:
            addNewMultipleAndConstantConversionRule("Temperature.dat");
            break;
        }

        if(request == -1)
            break;

    }
}


void userMode(){
    int request;

    system("cls");
    system("COLOR E1");

    while(1)
    {
        printf("What type of conversion do you want?\n\n");
        print_types();
        printf("Select a number from above: \n");
        printf("(Enter -1 to exit user mode.)\n\n");
        scanf("%d",&request);

        if(request == -1)
            break;
        printf("\n\n");
        printf("Select the units you want to convert from and to: \n\n");

        switch(request)
        {
        case 1:
            convertSimpleMultiple("Length.dat");
            break;

        case 2:
            convertSimpleMultiple("Weight.dat");
            break;

        case 3:
            convertSimpleMultiple("Time.dat");
            break;

        case 4:
            convertMultipleWithConstant("Temperature.dat");
            break;
        }
    }
}


int authenticateAdmin(){
    char password[100];
    printf("Please enter the admin password: ");
    scanf("%s", password);

    if(strcmp(password, ADMIN_PASSWORD) == 0)
        return 1;
    else return 0;

}

int main()
{

    int modeSelection;

    printf("===========================================\n");
    printf("        Welcome to unit converter\n");
    printf("===========================================\n\n\n");


    while(1){

        system("color 0f");
        printf("There are 2 modes to use this program. Please select one:\n");
        printf("1. User mode\n");
        printf("2. Admin mode\n");
        printf("Your selection: ");

        scanf("%d", &modeSelection);

        switch(modeSelection){
        case 1:
            system("cls");
            userMode();
            break;

        case 2:
            system("cls");
            if(authenticateAdmin()){
                adminMode();
            } else {
                printf("Wrong password\n\n");
            }
        }

    }
    return 0;
}
