#include <cmath>
#include "tictactoe.h"

using namespace std;

int main()
{
    Board_3x3 board;

    while (board.isRunning())
    {
        system("clear");

        cout << board;

        cin >> board;

        if (!board.isRunning())
            break;

        while (1)
        {
            int markPos = rand() % 9 + 1;
            if (board.getMark(markPos) == -1)
            {
                board.setMark(markPos);
                break;
            }
        }
    }

    system("clear");
    cout << board;

    cout << "\n"
         << (board.getWinner() == Human ? "You won!" : (board.getWinner() == Computer ? "Computer wins!" : "No one won!"))
         << "\n";

    return 0;
}