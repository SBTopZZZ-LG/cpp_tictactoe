#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>
#include <vector>

enum Player
{
    Human = 0,
    Computer = 1
};
enum Direction
{
    LEFT,
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT,
    BOTTOM,
    BOTTOM_LEFT
};

class Block
{
    int mark;
    Block *L, *TL, *T, *TR, *R, *BR, *B, *BL;

    bool initiateCheck()
    {
        Player player = (Player)mark;
        int score = 1 + (L != nullptr ? L->requestCheck(player, LEFT) : 0) + (R != nullptr ? R->requestCheck(player, RIGHT) : 0);
        if (score == 3)
            return true;

        score = 1 + (TL != nullptr ? TL->requestCheck(player, TOP_LEFT) : 0) + (BR != nullptr ? BR->requestCheck(player, BOTTOM_RIGHT) : 0);
        if (score == 3)
            return true;

        score = 1 + (T != nullptr ? T->requestCheck(player, TOP) : 0) + (B != nullptr ? B->requestCheck(player, BOTTOM) : 0);
        if (score == 3)
            return true;

        score = 1 + (TR != nullptr ? TR->requestCheck(player, TOP_RIGHT) : 0) + (BL != nullptr ? BL->requestCheck(player, BOTTOM_LEFT) : 0);
        return score == 3;
    }
    Block *evaluateDirection(Direction dir)
    {
        switch (dir)
        {
        case LEFT:
            return L;
        case TOP_LEFT:
            return TL;
        case TOP:
            return T;
        case TOP_RIGHT:
            return TR;
        case RIGHT:
            return R;
        case BOTTOM_RIGHT:
            return BR;
        case BOTTOM:
            return B;
        case BOTTOM_LEFT:
            return BL;
        }

        return nullptr;
    }

public:
    Block() : mark(-1), L(nullptr), TL(nullptr), T(nullptr), TR(nullptr), R(nullptr), BR(nullptr), B(nullptr), BL(nullptr) {}

    void setLeft(Block *ptr)
    {
        L = ptr;
    }
    void setTopLeft(Block *ptr)
    {
        TL = ptr;
    }
    void setTop(Block *ptr)
    {
        T = ptr;
    }
    void setTopRight(Block *ptr)
    {
        TR = ptr;
    }
    void setRight(Block *ptr)
    {
        R = ptr;
    }
    void setBottomRight(Block *ptr)
    {
        BR = ptr;
    }
    void setBottom(Block *ptr)
    {
        B = ptr;
    }
    void setBottomLeft(Block *ptr)
    {
        BL = ptr;
    }

    bool setMark(Player player)
    {
        mark = player;
        return initiateCheck();
    }
    int getMark()
    {
        return mark;
    }

    int requestCheck(Player player, Direction dir)
    {
        Block *ptr = evaluateDirection(dir);
        if (player == mark)
        {
            if (ptr != nullptr)
                return 1 + ptr->requestCheck(player, dir);
            return 1;
        }
        return 0;
    }
};
class Board_3x3
{
    std::vector<std::vector<Block *> > board;
    bool active;
    int winner;

    bool setMark(int pos, Player player)
    {
        pos--; // 0-based indexing

        int i = pos / 3;
        int j = pos - i * 3;

        bool result = board[i][j]->setMark(player);
        active = !result;
        if (!active)
            winner = player;

        if (!active)
            if (allFilled())
                active = false;

        return result;
    }
    bool allFilled()
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (board.at(i).at(j)->getMark() == -1)
                    return false;
        return true;
    }

public:
    Board_3x3() : active(1), winner(-1)
    {
        for (int i = 0; i < 3; i++)
        {
            std::vector<Block *> temp;
            for (int j = 0; j < 3; j++)
                temp.push_back(new Block());
            board.push_back(temp);
        }

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                Block *b = board.at(i).at(j);

                if (j > 0)
                {
                    b->setLeft(board.at(i).at(j - 1));
                    if (i > 0)
                        b->setTopLeft(board.at(i - 1).at(j - 1));
                }
                if (i > 0)
                {
                    b->setTop(board.at(i - 1).at(j));
                    if (j < 2)
                        b->setTopRight(board.at(i - 1).at(j + 1));
                }
                if (j < 2)
                {
                    b->setRight(board.at(i).at(j + 1));
                    if (i < 2)
                        b->setBottomRight(board.at(i + 1).at(j + 1));
                }
                if (i < 2)
                {
                    b->setBottom(board.at(i + 1).at(j));
                    if (j > 0)
                        b->setBottomLeft(board.at(i + 1).at(j - 1));
                }
            }
    }
    ~Board_3x3()
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                delete board[i][j];
    }

    bool setMark(int pos)
    {
        return this->setMark(pos, Computer);
    }
    int getMark(int pos)
    {
        pos--; // 0-based indexing

        int i = pos / 3;
        int j = pos - i * 3;

        return board[i][j]->getMark();
    }

    bool isRunning()
    {
        return active;
    }
    int getWinner()
    {
        return winner;
    }

    friend std::ostream &operator<<(std::ostream &os, Board_3x3 &b);
    friend std::istream &operator>>(std::istream &is, Board_3x3 &b);
};

std::ostream &operator<<(std::ostream &os, Board_3x3 &b)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int mark = b.getMark(1 + j + i * 3);
            os << (mark == Human ? 'X' : (mark == Computer ? 'O' : '-')) << (j < 2 ? " | " : "");
        }
        os << '\n'
           << (i < 2 ? "---------\n" : "");
    }

    return os;
}
std::istream &operator>>(std::istream &is, Board_3x3 &b)
{
    while (1)
    {
        int markPos;
        std::cout << "\nEnter mark position: ";
        is >> markPos;

        if (markPos > 0 && markPos < 10)
            if (b.getMark(markPos) == -1)
            {
                b.setMark(markPos, Human);
                break;
            }

        std::cout << "Enter a valid position!\n";
    }

    return is;
}

#endif