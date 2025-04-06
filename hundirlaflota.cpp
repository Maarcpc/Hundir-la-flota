#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>

using namespace std;

const int BOARD_SIZE = 10;
const char WATER = '~';
const char HIT = 'O';
const char MISS = 'X';
const vector<int> SHIP_SIZES = { 3, 4, 5, 6 };

struct Cell {
    char display = WATER;
    bool hasShip = false;
};

using Board = vector<vector<Cell>>;

struct Player {
    Board board;
    Board attackBoard;
    string name;
};

void initBoard(Board& board) {
    board.resize(BOARD_SIZE, vector<Cell>(BOARD_SIZE));
}

void printBoard(const Board& board, bool showShips) {
    cout << "    ";
    for (int col = 0; col < BOARD_SIZE; ++col)
        cout << setw(2) << col + 1 << " ";
    cout << "\n";

    for (int row = 0; row < BOARD_SIZE; ++row) {
        cout << setw(2) << row + 1 << " ";
        for (int col = 0; col < BOARD_SIZE; ++col) {
            char symbol = board[row][col].display;
            if (board[row][col].hasShip && showShips)
                symbol = '#'; // barco visible
            cout << " " << symbol << " ";
        }
        cout << endl;
    }
}

bool canPlaceShip(const Board& board, int row, int col, int size, bool vertical) {
    if (vertical) {
        if (row + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; ++i)
            if (board[row + i][col].hasShip) return false;
    }
    else {
        if (col + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; ++i)
            if (board[row][col + i].hasShip) return false;
    }
    return true;
}

void placeShip(Board& board, int size) {
    bool placed = false;
    while (!placed) {
        int row = rand() % BOARD_SIZE;
        int col = rand() % BOARD_SIZE;
        bool vertical = rand() % 2;

        if (canPlaceShip(board, row, col, size, vertical)) {
            for (int i = 0; i < size; ++i) {
                if (vertical)
                    board[row + i][col].hasShip = true;
                else
                    board[row][col + i].hasShip = true;
            }
            placed = true;
        }
    }
}

void setupPlayer(Player& player, const string& name) {
    player.name = name;
    initBoard(player.board);
    initBoard(player.attackBoard);
    for (int size : SHIP_SIZES)
        placeShip(player.board, size);
}

bool attack(Player& attacker, Player& defender) {
    int row, col;
    while (true) {
        cout << attacker.name << ", introdueix coordenades per atacar (fila columna): ";
        cin >> row >> col;
        if (cin.fail() || row < 1 || row > BOARD_SIZE || col < 1 || col > BOARD_SIZE) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Coordenades no vàlides. Torna-ho a provar." << endl;
        }
        else break;
    }

    --row; --col;

    if (defender.board[row][col].hasShip) {
        cout << "Aciert!" << endl;
        defender.board[row][col].display = HIT;
        attacker.attackBoard[row][col].display = HIT;
        return true;
    }
    else {
        cout << "Fallo!" << endl;
        defender.board[row][col].display = MISS;
        attacker.attackBoard[row][col].display = MISS;
        return false;
    }
}

bool allShipsSunk(const Board& board) {
    for (const auto& row : board)
        for (const auto& cell : row)
            if (cell.hasShip && cell.display != HIT)
                return false;
    return true;
}

void playGame(Player& p1, Player& p2) {
    bool gameOver = false;
    Player* current = &p1;
    Player* opponent = &p2;
    int turn = 1;

    while (!gameOver) {
        cout << "\n--- Torn " << turn << ": " << current->name << " ---\n";
        cout << "Tauler d'atacs:\n";
        printBoard(current->attackBoard, false);

        cout << "\nTauler del rival (sense vaixells):\n";
        printBoard(opponent->board, false);

        attack(*current, *opponent);
        gameOver = allShipsSunk(opponent->board);

        if (gameOver) {
            cout << "\nFi del joc! Ha guanyat " << current->name << "!\n";
        }
        else {
            swap(current, opponent);
            turn++;
        }
    }

    cout << "\nTauler final de " << p1.name << ":\n";
    printBoard(p1.board, true);
    cout << "\nTauler final de " << p2.name << ":\n";
    printBoard(p2.board, true);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Player player1, player2;
    setupPlayer(player1, "Jugador 1");
    setupPlayer(player2, "Jugador 2");

    cout << "\n--- Taulers inicials amb vaixells (DEBUG) ---\n";
    cout << "\nJugador 1:\n";
    printBoard(player1.board, true);
    cout << "\nJugador 2:\n";
    printBoard(player2.board, true);

    playGame(player1, player2);

    return 0;
}
