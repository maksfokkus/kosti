#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ==================== МОДУЛЬ 1: ГРАФІЧНЕ ВІДОБРАЖЕННЯ КІСТОК ====================
class DiceRenderer {
public:
    static void drawDice(int value) {
        switch(value) {
            case 1: drawOne(); break;
            case 2: drawTwo(); break;
            case 3: drawThree(); break;
            case 4: drawFour(); break;
            case 5: drawFive(); break;
            case 6: drawSix(); break;
            default: cout << "[ПОМИЛКА]"; break;
        }
    }

private:
    static void drawOne() {
        cout << "+-------+\n";
        cout << "|       |\n";
        cout << "|   •   |\n";
        cout << "|       |\n";
        cout << "+-------+\n";
    }
    static void drawTwo() {
        cout << "+-------+\n";
        cout << "| •     |\n";
        cout << "|       |\n";
        cout << "|     • |\n";
        cout << "+-------+\n";
    }
    static void drawThree() {
        cout << "+-------+\n";
        cout << "| •     |\n";
        cout << "|   •   |\n";
        cout << "|     • |\n";
        cout << "+-------+\n";
    }
    static void drawFour() {
        cout << "+-------+\n";
        cout << "| •   • |\n";
        cout << "|       |\n";
        cout << "| •   • |\n";
        cout << "+-------+\n";
    }
    static void drawFive() {
        cout << "+-------+\n";
        cout << "| •   • |\n";
        cout << "|   •   |\n";
        cout << "| •   • |\n";
        cout << "+-------+\n";
    }
    static void drawSix() {
        cout << "+-------+\n";
        cout << "| •   • |\n";
        cout << "| •   • |\n";
        cout << "| •   • |\n";
        cout << "+-------+\n";
    }
};

// ==================== МОДУЛЬ 2: ЛОГІКА ГРАЛЬНИХ КІСТОК ====================
class Dice {
private:
    int sides;
    int currentValue;
public:
    Dice(int s = 6) : sides(s), currentValue(1) {}

    int roll() {
        currentValue = (rand() % sides) + 1;
        return currentValue;
    }

    int getValue() const { return currentValue; }
    void setValue(int val) { if(val >= 1 && val <= sides) currentValue = val; }
};

// ==================== МОДУЛЬ 3: ГРАВЕЦЬ ====================
class Player {
private:
    string name;
    int score;
    int totalWins;
    vector<int> history;
public:
    Player(string n = "Гравець") : name(n), score(0), totalWins(0) {}

    string getName() const { return name; }
    int getScore() const { return score; }
    int getTotalWins() const { return totalWins; }
    vector<int> getHistory() const { return history; }

    void addScore(int points) { 
        score += points; 
        history.push_back(points);
    }
    void addWin() { totalWins++; }
    void resetScore() { score = 0; history.clear(); }
    void resetAll() { score = 0; totalWins = 0; history.clear(); }
};

// ==================== МОДУЛЬ 4: РЕЖИМИ ГРИ ====================
enum GameMode {
    MODE_CLASSIC,      
    MODE_POKER_DICE,   
    MODE_FIRST_TO_100, 
    MODE_BEST_OF_5     
};

// ==================== МОДУЛЬ 5: ІГРОВИЙ ДВИЖОК ====================
class GameEngine {
private:
    vector<Dice> dice;
    vector<Player> players;
    GameMode currentMode;
    int currentRound;
    int maxRounds;
    int currentPlayerIndex;
    bool gameActive;
    int targetScore;

public:
    GameEngine() : currentMode(MODE_CLASSIC), currentRound(1), 
                   maxRounds(5), currentPlayerIndex(0), gameActive(false), targetScore(100) {
        for(int i = 0; i < 5; i++) {
            dice.push_back(Dice(6));
        }
    }

    void setMode(GameMode mode) { currentMode = mode; }
    void setMaxRounds(int rounds) { maxRounds = rounds; }
    void setTargetScore(int target) { targetScore = target; }

    void addPlayer(string name) {
        players.push_back(Player(name));
    }

    int evaluatePokerCombo(const vector<int>& values) {
        int counts[7] = {0};
        for(int v : values) counts[v]++;
        
        bool hasPair = false, hasThree = false, hasFour = false, hasFive = false;
        int pairCount = 0;
        
        for(int i = 1; i <= 6; i++) {
            if(counts[i] == 2) { hasPair = true; pairCount++; }
            if(counts[i] == 3) hasThree = true;
            if(counts[i] == 4) hasFour = true;
            if(counts[i] == 5) hasFive = true;
        }
        
        if(hasFive) return 1000;        
        if(hasFour) return 800;         
        if(hasThree && hasPair) return 600; 
        if(hasThree) return 400;        
        if(pairCount == 2) return 300;  
        if(hasPair) return 200;         
        
        int sum = 0;
        for(int v : values) sum += v;
        return sum;
    }

    string getComboName(int score) {
        if(score == 1000) return "ПОКЕР!";
        if(score == 800) return "КАРЕ!";
        if(score == 600) return "ФУЛ-ХАУС!";
        if(score == 400) return "Трійка";
        if(score == 300) return "Дві пари";
        if(score == 200) return "Пара";
        return "Нічого (сума очок)";
    }

    void determineWinnerClassic() {
        cout << "\n========== РЕЗУЛЬТАТИ ==========\n";
        int maxScore = -1;
        int winnerIdx = -1;
        bool tie = false;
        
        for(size_t i = 0; i < players.size(); i++) {
            cout << players[i].getName() << ": " << players[i].getScore() << " очок\n";
            if(players[i].getScore() > maxScore) {
                maxScore = players[i].getScore();
                winnerIdx = i;
                tie = false;
            } else if(players[i].getScore() == maxScore) {
                tie = true;
            }
        }
        
        if(tie) {
            cout << "\n*** НІЧИЯ! ***\n";
        } else if(winnerIdx >= 0) {
            players[winnerIdx].addWin();
            cout << "\n*** ПЕРЕМОЖЕЦЬ: " << players[winnerIdx].getName() << "! ***\n";
        }
    }

    int rollAllDice() {
        int total = 0;
        cout << "\n";
        for(size_t d = 0; d < dice.size(); d++) {
            int val = dice[d].roll();
            total += val;
            DiceRenderer::drawDice(val);
        }
        cout << "Сума: " << total << "\n";
        return total;
    }

    void playClassicMode() {
        cout << "\n========== РЕЖИМ: ХТО БІЛЬШЕ ОЧОК ==========\n";
        cout << "Кожен гравець кидає 5 кісток. Хто набрав більше очок - переміг!\n\n";

        for(size_t i = 0; i < players.size(); i++) {
            cout << "--- Хід гравця: " << players[i].getName() << " ---\n";
            cout << "Натисніть Enter, щоб кинути кістки...";
            string dummy; getline(cin, dummy);

            int total = 0;
            cout << "\nРезультат кидка:\n";
            for(size_t d = 0; d < dice.size(); d++) {
                int val = dice[d].roll();
                total += val;
                cout << "Кістка " << (d+1) << ":\n";
                DiceRenderer::drawDice(val);
                cout << "Очок: " << val << "\n\n";
            }

            players[i].addScore(total);
            cout << ">>> Загальна сума очок " << players[i].getName() << ": " << total << " <<<\n\n";
        }

        determineWinnerClassic();
    }

    void playFirstTo100() {
        cout << "\n========== РЕЖИМ: ПЕРШИЙ ДО " << targetScore << " ==========\n";
        cout << "Гравці по черзі кидають кістки. Перший, хто досягне " << targetScore << " очок - переміг!\n\n";

        bool winnerFound = false;
        while(!winnerFound) {
            for(size_t i = 0; i < players.size() && !winnerFound; i++) {
                cout << "\n--- Раунд " << currentRound << " | Хід: " << players[i].getName() 
                     << " (Поточний рахунок: " << players[i].getScore() << ") ---\n";
                cout << "Натисніть Enter, щоб кинути кістки...";
                string dummy; getline(cin, dummy);

                int total = rollAllDice();
                players[i].addScore(total);

                cout << "\n>>> " << players[i].getName() << " набрав " << total 
                     << " очок. Загалом: " << players[i].getScore() << " <<<\n";

                if(players[i].getScore() >= targetScore) {
                    cout << "\n*** ПЕРЕМОЖЕЦЬ: " << players[i].getName() << "! ***\n";
                    players[i].addWin();
                    winnerFound = true;
                }
            }
            currentRound++;
        }
    }

    void playBestOf5() {
        cout << "\n========== РЕЖИМ: НАЙКРАЩИЙ З 5 РАУНДІВ ==========\n";

        for(int round = 1; round <= 5; round++) {
            cout << "\n========== РАУНД " << round << " ==========\n";

            for(size_t i = 0; i < players.size(); i++) {
                cout << "\n--- Хід: " << players[i].getName() << " ---\n";
                cout << "Натисніть Enter...";
                string dummy; getline(cin, dummy);

                int total = rollAllDice();
                players[i].addScore(total);
                cout << "Сума: " << total << " очок\n";
            }

            int maxScore = -1;
            int winnerIdx = -1;
            for(size_t i = 0; i < players.size(); i++) {
                int roundScore = players[i].getHistory().back();
                if(roundScore > maxScore) {
                    maxScore = roundScore;
                    winnerIdx = i;
                }
            }

            if(winnerIdx >= 0) {
                players[winnerIdx].addWin();
                cout << "\n>>> Переможець раунду " << round << ": " 
                     << players[winnerIdx].getName() << " (" << maxScore << " очок) <<<\n";
            }
        }

        cout << "\n========== ФІНАЛЬНИЙ РАХУНОК ==========\n";
        int maxWins = -1;
        int overallWinner = -1;
        for(size_t i = 0; i < players.size(); i++) {
            cout << players[i].getName() << ": " << players[i].getTotalWins() << " перемог\n";
            if(players[i].getTotalWins() > maxWins) {
                maxWins = players[i].getTotalWins();
                overallWinner = i;
            }
        }

        if(overallWinner >= 0) {
            cout << "\n*** ЗАГАЛЬНИЙ ПЕРЕМОЖЕЦЬ: " << players[overallWinner].getName() << "! ***\n";
        }
    }

    void playPokerDice() {
        cout << "\n========== РЕЖИМ: ПОКЕР НА КІСТКАХ ==========\n";
        cout << "Комбінації (від найслабшої до найсильнішої):\n";
        cout << "1. Нічого (сума очок)\n2. Пара (2 однакові)\n3. Дві пари\n";
        cout << "4. Трійка (3 однакові)\n5. Фул-хаус (3+2)\n6. Каре (4 однакові)\n7. Покер (5 однакових)\n\n";

        for(size_t i = 0; i < players.size(); i++) {
            cout << "--- Хід: " << players[i].getName() << " ---\n";
            cout << "Натисніть Enter...";
            string dummy; getline(cin, dummy);

            vector<int> values;
            cout << "\nВаші кістки:\n";
            for(size_t d = 0; d < dice.size(); d++) {
                int val = dice[d].roll();
                values.push_back(val);
                DiceRenderer::drawDice(val);
            }

            int comboScore = evaluatePokerCombo(values);
            players[i].addScore(comboScore);

            string comboName = getComboName(comboScore);
            cout << ">>> Комбінація: " << comboName << " (" << comboScore << " очок) <<<\n\n";
        }

        determineWinnerClassic();
    }

    void resetGame() {
        for(auto& p : players) p.resetScore();
        currentRound = 1;
        currentPlayerIndex = 0;
        gameActive = false;
    }

    void resetAll() {
        players.clear();
        resetGame();
    }

    bool hasPlayers() const { return !players.empty(); }
    int getPlayerCount() const { return players.size(); }
};

// ==================== МОДУЛЬ 6: МЕНЮ ТА ІНТЕРФЕЙС ====================
class GameMenu {
private:
    GameEngine engine;

    void clearScreen() {
        cout << "\033[2J\033[H";
    }

    void printHeader() {
        cout << "+========================================+\n";
        cout << "|         Г Р А   В   К І С Т К И        |\n";
        cout << "|            (Dice Game v1.0)            |\n";
        cout << "+========================================+\n\n";
    }

    void printMainMenu() {
        cout << "ГОЛОВНЕ МЕНЮ:\n";
        cout << "1. Правила гри\n";
        cout << "2. Розпочати гру\n";
        cout << "3. Налаштування\n";
        cout << "4. Статистика\n";
        cout << "5. Вийти з гри\n";
        cout << "\nВведіть номер пункту меню: ";
    }

    void printRules() {
        clearScreen();
        printHeader();
        cout << "========== ПРАВИЛА ГРИ ==========\n\n";
        cout << "Гра в кістки - це класична настільна гра з використанням\n";
        cout << "гральних кубиків (кісток).\n\n";
        cout << "*** МЕТА ГРИ ***\n";
        cout << "Залежно від обраного режиму - набрати найбільше очок,\n";
        cout << "першим досягти цільової кількості очок, або зібрати\n";
        cout << "найкращу комбінацію.\n\n";
        cout << "*** РЕЖИМИ ГРИ ***\n";
        cout << "1. ХТО БІЛЬШЕ - кожен гравець кидає 5 кісток, перемагає\n";
        cout << "   той, у кого більше очок.\n\n";
        cout << "2. ПЕРШИЙ ДО 100 - гравці по черзі кидають кістки,\n";
        cout << "   перший, хто досягне 100 очок - переміг.\n\n";
        cout << "3. НАЙКРАЩИЙ З 5 - проводиться 5 раундів, перемагає\n";
        cout << "   той, хто виграв більше раундів.\n\n";
        cout << "4. ПОКЕР НА КІСТКАХ - зібери найкращу комбінацію!\n\n";
        cout << "*** КЕРУВАННЯ ***\n";
        cout << "- Enter: кинути кістки\n";
        cout << "- Цифри 1-5: вибір пункту меню\n\n";
        cout << "Натисніть Enter, щоб повернутися в меню...";
        string dummy; getline(cin, dummy);
    }

    void setupGame() {
        clearScreen();
        printHeader();
        cout << "========== НАЛАШТУВАННЯ ГРИ ==========\n\n";

        int playerCount;
        cout << "Введіть кількість гравців (2-4): ";
        cin >> playerCount;
        cin.ignore();

        if(playerCount < 2 || playerCount > 4) {
            cout << "Помилка! Кількість гравців має бути від 2 до 4.\n";
            return;
        }

        engine.resetAll();

        for(int i = 0; i < playerCount; i++) {
            string name;
            cout << "Введіть ім'я гравця " << (i+1) << ": ";
            getline(cin, name);
            if(name.empty()) name = "Гравець " + to_string(i+1);
            engine.addPlayer(name);
        }

        cout << "\nОберіть режим гри:\n";
        cout << "1. ХТО БІЛЬШЕ ОЧОК\n";
        cout << "2. ПЕРШИЙ ДО 100\n";
        cout << "3. НАЙКРАЩИЙ З 5 РАУНДІВ\n";
        cout << "4. ПОКЕР НА КІСТКАХ\n";
        cout << "Ваш вибір: ";

        int modeChoice;
        cin >> modeChoice;
        cin.ignore();

        switch(modeChoice) {
            case 1: engine.setMode(MODE_CLASSIC); break;
            case 2: engine.setMode(MODE_FIRST_TO_100); break;
            case 3: engine.setMode(MODE_BEST_OF_5); break;
            case 4: engine.setMode(MODE_POKER_DICE); break;
            default: engine.setMode(MODE_CLASSIC); break;
        }

        cout << "\nНалаштування завершено! Гравців: " << engine.getPlayerCount() << "\n";
        cout << "Натисніть Enter, щоб почати...";
        string dummy; getline(cin, dummy);
    }

    void playGame() {
        if(!engine.hasPlayers()) {
            cout << "\nСпочатку налаштуйте гру (пункт 3 меню)!\n";
            return;
        }

        clearScreen();
        printHeader();

        cout << "Оберіть режим гри для запуску:\n";
        cout << "1. ХТО БІЛЬШЕ ОЧОК\n";
        cout << "2. ПЕРШИЙ ДО 100\n";
        cout << "3. НАЙКРАЩИЙ З 5 РАУНДІВ\n";
        cout << "4. ПОКЕР НА КІСТКАХ\n";
        cout << "Ваш вибір: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: engine.playClassicMode(); break;
            case 2: engine.playFirstTo100(); break;
            case 3: engine.playBestOf5(); break;
            case 4: engine.playPokerDice(); break;
            default: 
                cout << "Невірний вибір!\n";
                return;
        }

        cout << "\nНатисніть Enter, щоб продовжити...";
        string dummy; getline(cin, dummy);

        cout << "\nБажаєте зіграти ще раз? (так/ні): ";
        string answer;
        getline(cin, answer);

        if(answer == "так" || answer == "Так" || answer == "т" || answer == "Т") {
            engine.resetGame();
            playGame();
        }
    }

    void showStats() {
        clearScreen();
        printHeader();
        cout << "========== СТАТИСТИКА ==========\n";
        cout << "(Функція в розробці - потребує збереження даних)\n\n";
        cout << "Натисніть Enter, щоб повернутися...";
        string dummy; getline(cin, dummy);
    }

public:
    void run() {
        bool running = true;

        while(running) {
            clearScreen();
            printHeader();
            printMainMenu();

            string choice;
            getline(cin, choice);

            if(choice == "1") {
                printRules();
            }
            else if(choice == "2") {
                if(!engine.hasPlayers()) {
                    setupGame();
                }
                playGame();
            }
            else if(choice == "3") {
                setupGame();
            }
            else if(choice == "4") {
                showStats();
            }
            else if(choice == "5") {
                cout << "\nДякуємо за гру! До побачення!\n";
                running = false;
            }
            else {
                cout << "\nНевірний вибір! Спробуйте ще раз.\n";
            }
        }
    }
};

// ==================== ГОЛОВНА ФУНКЦІЯ ====================
int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    setlocale(LC_ALL, "ru_RU.UTF-8");

    srand(static_cast<unsigned>(time(0))); 
    
    GameMenu menu;
    menu.run();

    return 0;
}
