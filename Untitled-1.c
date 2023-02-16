#include <stdio.h>
// #include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int YEAR, MONTH, DATE;

typedef struct
{
    /* This is a structure useed to store user details such as 
    - user name, date of birth, age, account number, balance amount, PIN */

    char name[75];
    int date, month, year, age, ac, amt, pin;
}userData;

void today();
void display_menu();
int create_account();
userData *search(int returnValue);
int drop();
int update();
int recover();
void rand_gen(int *store, char file_name[], int start_val);
int file_write(char file[], userData *data, char mode[], int seek);
int dob(int *date, int *month, int *year);
int age(int *age, int date, int month, int year);

int main()
{
    /*
    This is the entry point of the program. The menu is displayed for user to choose.
    The user can then select the desired option, and the corresponding function will be executed. 
    The program uses different functions to perform the different tasks.
    */
    today();
    while (1)
    {
        display_menu();

        int choose, i;
        printf("Enter your choice: ");
        if (scanf("%d", &choose) != 1) {
            printf("Error: Invalid input!\n");
            continue;
        }

        switch (choose)
        {
            case 1:
                create_account();
                break;
            case 2:
                search(0);
                break;
            case 3:
                printf("\nWhat do you want to update?\n 1-Name\n 2-Date of Brith\nEnter [1/2]: ");

                if (scanf("%d", &i) != 1 && (i != 1 || i != 2)) 
                    printf("Error: Invalid input!\n");
                else 
                    update(i);
                break;
            case 4:
                drop();
                break;
            case 5:
                recover();
                break;

            case 6:
                update(3);
                break;
                
            case 7:
                printf("\nThanks for using our banking system!\n\n");
                exit(0);

            default:
                printf("\nInvalid choice! Please Enter again\n\n");
        }
    }
}

void display_menu()
{
    /* This is a function to display the Menu on the screen */
    printf("\n\n===========================\n");
    printf("\tSample Bank\n");
    printf("===========================\n");
    printf("-------MENU-------\n");
    printf("1. create\n");
    printf("2. Search\n");
    printf("3. Update\n");
    printf("4. Drop\n");
    printf("5. Recover\n");
    printf("6. Deposite or withdraw\n");
    printf("7. Exit\n");
}

void today()
{
    /* This Function calculates the current Date, month, and year using 'time.h' library functions */

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    YEAR = tm.tm_year+1900;
    MONTH = tm.tm_mon+1;
    DATE = tm.tm_mday;
}

int create_account()
{
    /* This function creates a new user account and stores it in the database */

    printf("\n\nOpen a Savings Account with India's No. 1* Bank\n");
    printf("Please enter the following details to open an account.\n\n");

    userData data;
    /* Name */
    char c;
    scanf("%c", &c);
    printf("Enter your name: ");
    scanf("%[^\n]s", data.name);
   
    dob(&data.date, &data.month, &data.year);
    int j = age(&data.age, data.date, data.month, data.year);
    if (j) return 0;

    /* Amount depositing */
    printf("Amount deposited: ");
    if (scanf("%d", &data.amt) != 1) {
        printf("Invaid Input!");
        return 0;
    };

    /* Account number and pin generated and displayed */
    rand_gen(&data.ac, "1.Account number.bin", 1000000000);
    rand_gen(&data.pin, "2.PIN.bin", 1000);

    printf("Your account number is: %d\n", data.ac);
    printf("Your PIN is: %d\n",  data.pin);
    printf("Don't forget your PIN!!!");
    
    /* The user details is being written in a file. */
    int i = file_write("Sample bank.bin", &data, "ab", 0);

    if (!i) printf("\n\nYour data is stored succesfully!");     /* If the file is written successfully */
    else printf("\n\nAn error has occured while storing your data!\nPlease try again.");    /* If any error in opening the file */

    return 0;
}

int dob(int *date, int *month, int *year)
{

    /* Date of birth */
    int i = 0, j = 0;
    *date = 0; *month = 0; *year = 0; 
    do{
        if (i) printf("Enter correct month!\n");
        if (j) printf("Enter the year and month!\n");

        printf("Enter your date of birth [dd/mm/yyyy]: ");
        scanf("%d/%d/%d", date, month, year);

        i = (*month > 12 || *month < 0) ? 1 : 0;    /* checking if the entered month is between 1 and 12 */
        j = (*year == 0 || *month == 0) ? 1 : 0;    /* checking if both year and month are entered */
    } while (i || j);

    return 0;
}

int age(int *age, int date, int month, int year)
{
    /* Calculating the age from given date of birth */
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    *age = ((YEAR - year)*365 + (MONTH - month)*days[month-1] + (DATE - date))/365;

    /* Checking age for 18+ */
    if (*age < 18)
    {
        printf("You are not eligible to open an account!\n");
        return 1;
    }
    return 0;
}

userData *search(int returnValue)
{
    FILE *fpt;
    userData *data = malloc(sizeof(userData));
    if (data == NULL) {
        printf("\nError: Memory allocation failed!\n");
        return NULL;
    }

    fpt = fopen("Sample bank.bin", "rb");
    if (fpt == NULL) {
        printf("\nError: File not found!\n");
        free(data);
        return NULL;
    }

    int account_number;
    printf("\nEnter your account Number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("\nError: Invalid input!\n");
        fclose(fpt);
        free(data);
        return NULL;
    }

    long offset = (account_number - 1000000000) * sizeof(userData);
    if (fseek(fpt, offset, SEEK_SET) != 0) {
        printf("\nError: Account not found!\n");
        fclose(fpt);
        free(data);
        return NULL;
    }

    if (fread(data, sizeof(userData), 1, fpt) != 1) {
        printf("\nError: Failed to read account data!\n");
        fclose(fpt);
        free(data);
        return NULL;
    }

    if (account_number != data->ac || data->age == 0) {
        printf("\nError: Account not found!\n");
        fclose(fpt);
        free(data);
        return NULL;
    }

    int pin;
    printf("Enter PIN to access data: ");
    if (scanf("%d", &pin) != 1) {
        printf("Error: Invalid input!\n");
        fclose(fpt);
        free(data);
        return NULL;
    }

    if (pin != data->pin) {
        printf("Error: Access denied!\n");
        fclose(fpt);
        free(data);
        return NULL;
    }

    printf("\nAccount Details....\n");
    printf("+=====================+=====================+=====================+==========+\n");
    printf("|%-20s |%-20s |%-20s |%-10s|\n", "Account Number", "Name", "Date of Birth", "Balance");
    printf("+---------------------+---------------------+---------------------+----------+\n");
    printf("|%-20d |%-20s |%02d/%02d/%-14d |%-10d|\n", data->ac, data->name, data->date, data->month, data->year, data->amt);
    printf("+=====================+=====================+=====================+==========+\n");

    fclose(fpt);
    if (returnValue) return data;
    free(data);
    return NULL;
}

int drop()
{
    userData *data = malloc(sizeof(userData));
    data = search(1);

    printf("\n\n------Delete Account-----\n");

    if (data != NULL && data->age != 0)
    {
        printf("\n\nAre you sure?[y/n]: ");
        char c;
        scanf("%c", &c);
        char confirm;
        scanf("%c", &confirm);
        if (toupper(confirm) == 'Y') 
        {
            file_write("Recovery file.bin", data, "ab", 0);

            strcpy(data->name, "NULL"); data->month = 0; data->year = 0; data->date = 0; data->age = 0; data->amt = 0;
            
            file_write("Sample bank.bin", data, "rb+", 1);
            printf("\n\nDeleted Successfully!");
        }
        else
        {
            printf("\n\nError: Deletion failed!");
        }
    }
    free(data);
    return 0;
}

int update(int choose)
{
    userData *data = malloc(sizeof(userData));
    data = search(1);

    printf("\n\n-----User Details Updation-----\n\n");

    if (data->age != 0 && data != NULL)
    {
        char c;
        switch (choose)
        {
                /* Name updation */
            case 1:
                scanf("%c", &c);
                printf("Enter your name: ");
                scanf("%[^\n]s", data->name);
                printf("\nUpdated successfully");
                break;

                /* Date of birth and age updation */
            case 2:
                dob(&data->date, &data->month, &data->year);
                int i = age(&data->age, data->date, data->month, data->year);
                if (i)
                {
                    return 0;
                }
                printf("\nUpdated successfully");
                break;

                /* Deposite and withdrawal */
            case 3:
                printf("Do you want to\n 1-deposite\n 2-withdraw\nEnter either [1/2]: ");
                int option, amt;
                scanf("%d", &option);

                switch (option)
                {
                case 1: /* Deposite */
                    printf("Enter the amount to desposite: ");
                    scanf("%d", &amt);
                    data->amt += amt;
                    printf("\nDeposited successfully");
                    break;
                
                case 2: /* withdrawal */
                    printf("Enter the amount to withdraw: ");
                    scanf("%d", &amt);
                    data->amt -= amt;
                    printf("\nWithdrawn successfully");
                    break;
                
                default:
                    printf("\nError: Invalid option!\n");
                    break;
                }
                break;
            
            default:
                printf("\nError: Invalid Input!\n");
                break;
        }
        // printf("%d/%d/%d", data.date, data.month, data.year);
        file_write("Sample bank.bin", data, "rb+", 1);
    }
    return 0;
}

int recover()
{
    userData data;
    FILE *fpt;

    if((fpt = fopen("Recovery file.bin", "rb")) == NULL)  printf("\nError opening file"); /* Opening file without error */

    else
    {
        printf("\n\n------Recover account------");
        printf("\n\nEnter your account number: ");
        int accountNumber;
        scanf("%d", &accountNumber);
        /* Searching account with entered account number */
        for (int i = 0; fread(&data, sizeof(userData), 1, fpt); i++)
        {
            if (data.ac ==  accountNumber) break;
        }
        
        int pin;
        printf("Confirm your recover(By entering PIN you are confirming to recover)\nEnter Your PIN: ");
        scanf("%d", &pin);
        if (pin == data.pin) /* Cheking for correct pin */
        {
            file_write("Sample bank.bin", &data, "rb+", 1); /* Entering the data into "BOC accounts" file*/
            printf("\n\nAccount recovered successfully!!");
        }
        else
        {
            printf("Recovery failed!\nIncorrect password!!");
        }
    }
    return 0;
}

void rand_gen(int *store, char file_name[], int start_val)
{
    FILE *fpt;

    /* Open the file with "rb+" mode to read and write */
    if ((fpt = fopen(file_name, "rb+")) == NULL)
    {
        /* If the file does not exist, create it and write the start value */
        fpt = fopen(file_name, "wb");
        fprintf(fpt, "%d", start_val);
        *store = start_val;
    }
    else
    {
        /* Read the value from the file */
        fscanf(fpt, "%d", store);
        (*store)++; /* Increment the value */

        /* Move the file pointer back to the beginning and write the new value */
        fseek(fpt, 0, SEEK_SET);
        fprintf(fpt, "%d", *store);
    }
    /* Close the file */
    fclose(fpt);
}

int file_write(char file[], userData *data, char mode[], int seek)
{
    FILE *fpt;
    /* Writting into the file */
    if ((fpt = fopen(file, mode)) == NULL) return 1;

    if (seek) {
        rewind(fpt);
        long offset = sizeof(userData)*(data->ac-1000000000);
        fseek(fpt, offset, SEEK_SET);
    }
    fwrite(data, sizeof(userData), 1, fpt);

    fclose(fpt);
    return 0;
}
