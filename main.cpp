#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

using namespace std;

bool placeMove(int *boardState, int position, int player)
{
    if(boardState[position] == 0)
    {
        boardState[position] = player;
        return true;
    }
    else
        return false;
}

int getInput(sf::RectangleShape *buttons, sf::Vector2i mousePos)
{
    for(int i = 0; i < 9; ++i)
    {
        if(buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
            return i;
    }
    return 10;
}

int checkWin(int *board)
{
    int checkColumb = 0;
    int checkRow = 0;
    int player;
    for(int i = 1; i <= 2; ++i)
    {
        if(i == 1)
            player = -1;
        else
            player = 1;

        for(int iy = 0; iy < 3; ++iy)
        {
            for(int ix = 0; ix < 3; ++ix)
            {
                if(board[ix + 3*iy] == player)
                    checkRow++;

                if(board[iy + 3*ix] == player)
                    checkColumb++;
            }

            if(checkRow == 3 || checkColumb == 3)
            {
                if(i == 1)
                    return -1;
                else
                    return 1;
            }

            checkRow = 0;
            checkColumb = 0;
        }
    }

    int checkDiag1 = 0;
    int checkDiag2 = 0;
    for(int i = 1; i <= 2; ++i)
    {
        if(i == 1)
            player = -1;
        else
            player = 1;
        for(int ixy = 0; ixy < 3; ++ixy)
        {
            if(board[ixy + ixy*3] == player)
                checkDiag1++;

            if(board[(2-ixy) + ixy*3] == player)
                checkDiag2++;
        }
        if(checkDiag1 == 3 || checkDiag2 == 3)
        {
            if(i == 1)
                return -1;
            else
                return 1;
        }

        checkDiag1 = 0;
        checkDiag2 = 0;
    }

    return 0;
}

void resizeBoard(sf::RectangleShape *board, int winXres, int winYres)
{
    for(int i = 0; i < 4; ++i)
    {
        board[i].setSize(sf::Vector2f(winXres/70, winXres/2));
        int originX = board[0].getSize().x/2;
        int originY = board[0].getSize().y/2;
        board[i].setOrigin(sf::Vector2f(originX, originY));
        board[i].setFillColor(sf::Color::White);
    }

    int offset = winXres/12;

    board[0].setPosition(sf::Vector2f(winXres/2 - offset, winYres/2));
    board[1].setPosition(sf::Vector2f(winXres/2 + offset, winYres/2));
    board[2].setPosition(sf::Vector2f(winXres/2, winYres/2 - offset));
    board[3].setPosition(sf::Vector2f(winXres/2, winYres/2 + offset));
}

void resizeButtons(sf::RectangleShape *buttons, int winXres, int winYres)
{
    for(int i = 0; i < 9; ++i)
    {
        buttons[i].setSize(sf::Vector2f(winXres/7, winXres/7));
        int originX = buttons[0].getSize().x/2;
        int originY = buttons[0].getSize().y/2;
        buttons[i].setOrigin(sf::Vector2f(originX, originY));
        buttons[i].setFillColor(sf::Color::Red);
    }

    //puts them in the proper positions
    for(int ix = 0; ix < 3; ++ix)
    {
        for(int iy = 0; iy < 3; ++iy)
        {
            int offset = (winXres/6);

            buttons[iy*3 + ix].setPosition((winXres/2-offset) + offset*ix,  + (winYres/2-offset) + offset*iy);
        }
    }
}

void resizeCross(sf::RectangleShape &cross, int winXres, int winYres)
{
    cross.setFillColor(sf::Color::White);
    cross.setSize(sf::Vector2f(winXres/55, winXres/7));
    cross.setOrigin(sf::Vector2f(cross.getSize().x/2, cross.getSize().y/2));
}

void resizeCircle(sf::CircleShape &circle, sf::RectangleShape &cross, int winXres, int winYres)
{
    circle.setRadius((cross.getSize().y/sqrt(2))/2);
    circle.setFillColor(sf::Color::Black);
    circle.setOutlineColor(sf::Color::White);
    circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
    circle.setOutlineThickness(winXres/60);
}

int minimax(int *board, bool Maximizer, int player)
{
    bool movesLeft = false;
    int boardCopy[9];
    for(int i = 0; i < 9; ++i)
    {
        boardCopy[i] = board[i];
        if(board[i] == 0)
            movesLeft = true;
    }

    int score = checkWin(board);
    if(score != 0)
        return score;

    if(!movesLeft)
    {
        return 0;
    }

    int nextPlayer;
    if(player == -1)
        nextPlayer = 1;
    else
        nextPlayer = -1;

    int bestMove;
    if(Maximizer)
    {
        bestMove = -10;
        for(int i = 0; i < 9; ++i)
        {
            for(int i = 0; i < 9; ++i)
            {
                boardCopy[i] = board[i];
            }
            if(boardCopy[i] == 0)
            {
                placeMove(boardCopy, i, player);
                bestMove = max(bestMove, minimax(boardCopy, false, nextPlayer));
            }
        }
        return bestMove;
    }
    else
    {
        bestMove = 10;
        for(int i = 0; i < 9; ++i)
        {

            for(int i = 0; i < 9; ++i)
            {
                boardCopy[i] = board[i];
            }
            if(boardCopy[i] == 0)
            {
                placeMove(boardCopy, i, player);
                bestMove = min(bestMove, minimax(boardCopy, true, nextPlayer));
            }
        }
        return bestMove;
    }
}

int AImove(int *board)
{
    int BestVal = 10;
    int MoveVal = BestVal;
    int bestMove = 0;

    int player = -1;
    int nextPlayer = 1;

    int boardCopy[9];
    for(int i = 0; i < 9; ++i)
    {
        for(int i = 0; i < 9; ++i)
        {
            boardCopy[i] = board[i];
        }
        if(boardCopy[i] == 0)
        {
            placeMove(boardCopy, i, player);
            MoveVal = minimax(boardCopy, true, nextPlayer);
            cout << BestVal << ", " << MoveVal << '\n';
            if(MoveVal < BestVal)
            {
                BestVal = MoveVal;
                bestMove = i;
                cout << "called\n";
            }
        }
    }
    cout << '\n';
    return bestMove;
}

int main()
{
    //window initialization
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    int winXres = 600;
    int winYres = 600;

    sf::RenderWindow win(sf::VideoMode(winXres, winYres), "rikki tikki tavi", sf::Style::Default, settings);
    int boardState[9]{0};


    //rectangles for drawing the grid
    sf::RectangleShape board[4];
    resizeBoard(board, winXres, winYres);
    board[2].rotate(90);
    board[3].rotate(90);

    //hit boxes for the grid
    sf::RectangleShape buttons[9];
    resizeButtons(buttons, winXres, winYres);

    //one segment of the cross
    sf::RectangleShape cross;
    resizeCross(cross, winXres, winYres);
    cross.rotate(45);

    sf::CircleShape circle;
    resizeCircle(circle, cross, winXres, winYres);

    sf::Font font;
    font.loadFromFile("Inconsolata.ttf");

    sf::Text reset;
    reset.setString("Reset");
    reset.setFont(font);
    reset.setFillColor(sf::Color::White);

    sf::Text whoWon;
    whoWon.setFont(font);
    whoWon.setFillColor(sf::Color::White);

    int player = 1;
    int winstate = 0;

    //game loop
    while(win.isOpen())
    {
        sf::Event event;

        int input;

        if(player == -1 && winstate == 0)
        {
            placeMove(boardState, AImove(boardState), player);
            player = 1;
            winstate = checkWin(boardState);
        }

        while(win.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                win.close();

            else if(event.type == sf::Event::Resized)
            {
                winXres = win.getSize().x;
                winYres = win.getSize().y;
                sf::View view(sf::Vector2f(winXres/2, winYres/2), sf::Vector2f(winXres, winYres));
                win.setView(view);
                resizeBoard(board, winXres, winYres);
                resizeButtons(buttons, winXres, winYres);
                resizeCross(cross, winXres, winYres);
                resizeCircle(circle, cross, winXres, winYres);
            }

            else if(event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePos;
                if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    mousePos = sf::Mouse::getPosition(win);
                else
                    break;

                if(reset.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    for(int i = 0; i < 9; ++i)
                    {
                        boardState[i] = 0;
                    }
                    winstate = 0;
                    player = 1;
                }

                input = getInput(buttons, mousePos);
                if(input < 9)
                {
                    if(winstate == 0)
                    {
                        if(!placeMove(boardState, input, player))
                            break;

                        if(player == -1)
                            player = 1;
                        else
                            player = -1;
                    }
                }

                winstate = checkWin(boardState);
            }
        }

        win.clear();

        if(winstate == 1)
        {
            whoWon.setString(" CROSS WON!");
            whoWon.setPosition(0, winYres/2);
            whoWon.setCharacterSize(win.getSize().x/25);
            win.draw(whoWon);
        }
        else if(winstate == -1)
        {
            whoWon.setString(" CIRCLE WON!");
            whoWon.setPosition(0, winYres/2);
            whoWon.setCharacterSize(win.getSize().x/25);
            win.draw(whoWon);
        }

        bool checkTie = true;
        for(int i = 0; i < 9; ++i)
        {
            if(boardState[i] == 0)
                checkTie = false;
        }

        if(checkTie)
        {
            whoWon.setString("  TIE!");
            whoWon.setPosition(0, winYres/2);
            whoWon.setCharacterSize(win.getSize().x/25);
            win.draw(whoWon);
        }

        for(int i = 0; i < 4; ++i)
            win.draw(board[i]);

        for(int i = 0; i < 9; ++i)
        {
            //win.draw(buttons[i]);

            if(boardState[i] == 1)
            {
                cross.setPosition(buttons[i].getPosition());
                win.draw(cross);
                cross.rotate(90);
                win.draw(cross);
            }
            else if(boardState[i] == -1)
            {
                circle.setPosition(buttons[i].getPosition());
                win.draw(circle);
            }
        }

        if(reset.getGlobalBounds().contains(sf::Mouse::getPosition(win).x, sf::Mouse::getPosition(win).y))
            reset.setFillColor(sf::Color::Red);
        else
            reset.setFillColor(sf::Color::White);

        win.draw(reset);

        win.display();

    }

    return 0;
}
