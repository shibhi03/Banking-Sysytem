# Banking-Sysytem

The code is a simple banking system that provides various functionalities like creating a new account, searching for an existing account, updating the account, depositing or withdrawing from the account, deleting the account, and recovering the account.

The program has a main function that displays a menu with options, and when the user chooses an option, the corresponding function is executed. The data of the user is stored in a binary file, "Sample bank.bin".

The program uses different libraries like "stdio.h", "conio.h", "ctype.h", "stdlib.h", "string.h", and "time.h" for various purposes. The program defines a structure, "userData," to store the user's details like name, date of birth, age, account number, balance amount, and PIN.

The program also uses different functions to perform various tasks. For example, the "create_account" function is used to create a new user account and store it in the binary file. The "search" function is used to search for an existing account, and the "update" function is used to update the user's details. The "drop" function is used to delete the account, and the "recover" function is used to recover the deleted account.

The code also uses some helper functions like "today," "display_menu," "rand_gen," "file_write," "dob," and "age." The "today" function calculates the current date, month, and year using the "time.h" library functions. The "display_menu" function displays the menu on the screen. The "rand_gen" function generates a random number for the account number and PIN. The "file_write" function writes the user details in the binary file. The "dob" function takes the user's date of birth as input and returns the date, month, and year. The "age" function takes the date, month, and year as input and returns the age of the user.
