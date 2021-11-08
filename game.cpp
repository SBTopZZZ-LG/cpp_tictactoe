#include <cmath>
#include "tictactoe.h"

using namespace std;

int main()
{
    Board *board;
    int choice;

    while (1)
    {
        system("clear");
        cout << "---Choose a board---\n1] 3 by 3\n2] 4 by 4\n3] 5 by 5\n\n9] Exit\n\n->";

        cin >> choice;

        if (choice == 1)
            board = new Board_3x3();
        else if (choice == 2)
            board = new Board_4x4();
        else if (choice == 3)
            board = new Board_5x5();
        else if (choice == 9)
            return 0;
        else
            continue;

        break;
    }

    while ((*board).isRunning())
    {
        system("clear");
        cout << (*board);

        cin >> (*board);

        if (!(*board).isRunning())
            break;

        while (1)
        {
            int markPos = rand() % ((choice == 1 ? 9 : (choice == 2 ? 16 : 25)) + 1);
            if ((*board).getMark(markPos) == -1)
                (*board).setMark(markPos);
            else
                continue;
            break;
        }
    }

    system("clear");
    cout << (*board);

    cout << ((*board).getWinner() == Human ? "\nYou won!\n" : ((*board).getWinner() == Computer ? "\nComputer wins!\n" : "\nNo one won!\n"));

    delete board;

    return 0;
}