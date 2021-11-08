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
    int mark, streakLength;
    Block *L, *TL, *T, *TR, *R, *BR, *B, *BL;

    bool initiateCheck()
    {
        Player player = (Player)mark;
        int score = 1 + (L != nullptr ? L->requestCheck(player, LEFT) : 0) + (R != nullptr ? R->requestCheck(player, RIGHT) : 0);
        if (score == streakLength)
            return true;

        score = 1 + (TL != nullptr ? TL->requestCheck(player, TOP_LEFT) : 0) + (BR != nullptr ? BR->requestCheck(player, BOTTOM_RIGHT) : 0);
        if (score == streakLength)
            return true;

        score = 1 + (T != nullptr ? T->requestCheck(player, TOP) : 0) + (B != nullptr ? B->requestCheck(player, BOTTOM) : 0);
        if (score == streakLength)
            return true;

        score = 1 + (TR != nullptr ? TR->requestCheck(player, TOP_RIGHT) : 0) + (BL != nullptr ? BL->requestCheck(player, BOTTOM_LEFT) : 0);
        return score == streakLength;
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
    Block(int _streakLength) : mark(-1), streakLength(_streakLength), L(nullptr), TL(nullptr), T(nullptr), TR(nullptr), R(nullptr), BR(nullptr), B(nullptr), BL(nullptr) {}

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

class Board
{
    std::vector<std::vector<Block *> > board;
    bool active;
    int winner, width;

    bool setMark(int pos, Player player)
    {
        pos--; // 0-based indexing

        int i = pos / width;
        int j = pos - i * width;

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
        for (int i = 0; i < width; i++)
            for (int j = 0; j < width; j++)
                if (board.at(i).at(j)->getMark() == -1)
                    return false;
        return true;
    }

public:
    Board(int _width) : active(1), winner(-1), width(_width)
    {

        for (int i = 0; i < width; i++)
        {
            std::vector<Block *> temp;
            for (int j = 0; j < width; j++)
                temp.push_back(new Block(width));
            board.push_back(temp);
        }

        for (int i = 0; i < width; i++)
            for (int j = 0; j < width; j++)
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
                    if (j < width - 1)
                        b->setTopRight(board.at(i - 1).at(j + 1));
                }
                if (j < width - 1)
                {
                    b->setRight(board.at(i).at(j + 1));
                    if (i < width - 1)
                        b->setBottomRight(board.at(i + 1).at(j + 1));
                }
                if (i < width - 1)
                {
                    b->setBottom(board.at(i + 1).at(j));
                    if (j > 0)
                        b->setBottomLeft(board.at(i + 1).at(j - 1));
                }
            }
    }
    ~Board()
    {
        for (int i = 0; i < width; i++)
            for (int j = 0; j < width; j++)
                delete board[i][j];
    }

    bool setMark(int pos)
    {
        return this->setMark(pos, Computer);
    }
    int getMark(int pos)
    {
        pos--; // 0-based indexing

        int i = pos / width;
        int j = pos - i * width;

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

    friend std::ostream &operator<<(std::ostream &os, Board &b);
    friend std::istream &operator>>(std::istream &is, Board &b);
};
std::ostream &operator<<(std::ostream &os, Board &b)
{
    for (int i = 0; i < b.width; i++)
    {
        for (int j = 0; j < b.width; j++)
        {
            int mark = b.getMark(1 + j + i * b.width);
            os << (mark == Human ? 'X' : (mark == Computer ? 'O' : '-')) << (j < (b.width - 1) ? " | " : "");
        }
        os << '\n';
        for (int _ = 0; _ < 6 + (b.width - 2) * 3 + (b.width - 3); _++)
            if (i < b.width - 1)
                os << '-';
        os << '\n';
    }

    return os;
}
std::istream &operator>>(std::istream &is, Board &b)
{
    while (1)
    {
        int markPos;
        std::cout << "\nEnter mark position: ";
        is >> markPos;

        if (markPos > 0 && markPos < b.width * b.width + 1)
            if (b.getMark(markPos) == -1)
            {
                b.setMark(markPos, Human);
                break;
            }

        std::cout << "Enter a valid position!\n";
    }

    return is;
}

class Board_3x3 : public Board
{
public:
    Board_3x3() : Board(3) {}
};

class Board_4x4 : public Board
{
public:
    Board_4x4() : Board(4) {}
};

class Board_5x5 : public Board
{
public:
    Board_5x5() : Board(5) {}
};

#endif