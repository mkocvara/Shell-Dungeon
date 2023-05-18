// KF6017.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Character.h"

void one();
void two();
void three();
void four();
int ten(int a, int b, int c);
void alien();

int main()
{
    std::cout << "Hello World!\n";

    //one();
    //two();
    //three();
    //four();

    //int a = 5, b = 3, c = 7;
    //printf("ten(%d,%d,%d) = %d\n", a,b,c, ten(a,b,c));

    //alien();

    Character thomas = Character(10, 8, 25, "Thomas");
    Character eric = Character(11, 6, 30, "Eric");

    thomas.displayDetails();
    eric.displayDetails();

    //eric.takeDamage(15);
    //thomas.takeDamage(150);
    //thomas.displayDetails();

    thomas.fight(eric);

    thomas.displayDetails();
    eric.displayDetails();
}

void one()
{   
    printf("\nONE\n");

    int a = 12;

    std::cout << "cout: The variable contains the number: " << a << "\n";
    printf("printf: The variable contains the number: %d\n", a);
}

void two()
{   
    printf("\nTWO\n");

    int a = 12;

    std::cout << "cout: The variable contains the number: " << a*2 << "\n";
    printf("printf: The variable contains the number: %d\n", a*2);
}

void three()
{   
    printf("\nTHREE\n");

    int highScore = 0;
    printf("Enter High Score: ");
    scanf_s("%d", &highScore);

    char nameInitial, surnameInitial;
    printf("\nEnter First Initial: ");
    scanf_s(" %c", &nameInitial, 1);
    printf("\nEnter Surname Initial: ");
    scanf_s(" %c", &surnameInitial, 1);

    nameInitial = (nameInitial>90) ? nameInitial-32 : nameInitial;
    surnameInitial = (surnameInitial>90) ? surnameInitial-32 : surnameInitial;

    printf("%c.%c.'s High Score is %d.\n", nameInitial, surnameInitial, highScore);
}

void four()
{
    printf("\nFOUR\n");

    int a;
}

int ten(int a, int b, int c)
{
    printf("\nTEN\n");

    if (a < b && a < c) a = 0;
    if (b < a && b < c) b = 0;
    if (c < a && c < b) c = 0;

    return a + b + c;
}

int getActionInput(int maxNumber)
{
    int actionSelected = -1;

    while (actionSelected == -1)
    {
        scanf_s(" %d", &actionSelected);
        if (actionSelected <= maxNumber && actionSelected > 0)
            break;
        else
        {
            std::cout << "Not an action, try again.\n";
            actionSelected = -1;

            // clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }

    return actionSelected;
}

int feed() 
{ 
    printf("Nom.\n"); 
    return 10;
}

int stroke()
{ 
    printf("Purr.\n"); 
    return 3;
}

int kick()
{ 
    printf("EEK.\n"); 
    return -5;
}

int squeeze()
{ 
    printf("pft.\n"); 
    return 1;
}

int throwPet()
{ 
    printf("Weee.\n"); 
    return -20;
}

void clampInt(int &num, int min, int max)
{
    num = std::min(max, std::max(num, min));
}

void alien()
{
    printf("\nELEVEN, TWELVE\n");

    const int NUMACTIONS = 10;

    int alienHappiness = 100;

    std::string actions[NUMACTIONS];
    actions[0] = "Feed";
    actions[1] = "Stroke";
    actions[2] = "Kick";
    actions[3] = "Squeeze";
    actions[4] = "Throw";
    actions[9] = "Quit";

    printf("Select Action:\n");
    for (int i = 0; i < NUMACTIONS; i++)
    {
        if( !actions[i].empty() )
            printf("%d.\t%s\n", i+1, actions[i].c_str());
    }
    printf("\n");

    int actionSelected;
    bool quit = false;
    while (!quit)
    {
        actionSelected = getActionInput(NUMACTIONS);

        switch (--actionSelected)
        {
        case 0:
            alienHappiness += feed();
            break;
        case 1:
            alienHappiness += stroke();
            break;
        case 2:
            alienHappiness += kick();
            break;
        case 3:
            alienHappiness += squeeze();
            break;
        case 4:
            alienHappiness += throwPet();
            break;
        case 9:
            quit = true;
            break;
        default:
            break;
        }

        if (!quit)
        {
            clampInt(alienHappiness, 0, 100);
            printf("Alien's happiness is now at %d%\n\n", alienHappiness);
        }
    }
}