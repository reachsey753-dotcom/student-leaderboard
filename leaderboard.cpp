#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>

const int NAME_BUFFER_SIZE = 64;

struct Team {
    int teamId;
    char name[NAME_BUFFER_SIZE];
    int score;
    int missionsCompleted;
};

class Leaderboard {
private:
    Team* data;
    int size;
    int capacity;

    void grow() {
        int newCap = capacity * 2;
        Team* newData = new Team[newCap];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCap;
    }

public:
    Leaderboard() : data(nullptr), size(0), capacity(0) {
        data = new Team[4];
        capacity = 4;
    }

    Leaderboard(int initialCapacity) : data(nullptr), size(0), capacity(0) {
        if (initialCapacity < 1) initialCapacity = 1;
        data = new Team[initialCapacity];
        capacity = initialCapacity;
    }

    Leaderboard(const Leaderboard& other) : data(nullptr), size(other.size), capacity(other.capacity) {
        data = new Team[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    Leaderboard& operator=(const Leaderboard& other) {
        if (this == &other) return *this;
        Team* newData = new Team[other.capacity];
        for (int i = 0; i < other.size; i++) {
            newData[i] = other.data[i];
        }
        delete[] data;
        data = newData;
        size = other.size;
        capacity = other.capacity;
        return *this;
    }

    Leaderboard(Leaderboard&& other) noexcept
        : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    Leaderboard& operator=(Leaderboard&& other) noexcept {
        if (this == &other) return *this;
        delete[] data;
        data = other.data;
        size = other.size;
        capacity = other.capacity;
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
        return *this;
    }

    ~Leaderboard() {
        delete[] data;
    }

    int findTeamIndex(int teamId) const {
        for (int i = 0; i < size; i++) {
            if (data[i].teamId == teamId) {
                return i;
            }
        }
        return -1;
    }

    bool ensureCapacity(int required) {
        if (required <= capacity) return true;
        int newCap = capacity;
        while (newCap < required) {
            newCap *= 2;
        }
        Team* newData = new (std::nothrow) Team[newCap];
        if (!newData) return false;
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCap;
        return true;
    }

    bool addTeam(int teamId, const char* name, int score, int missions, std::string& error) {
        error.clear();
        if (teamId <= 0) {
            error = "Team ID must be positive.";
            return false;
        }
        if (name == nullptr || std::strlen(name) == 0) {
            error = "Team name cannot be empty.";
            return false;
        }
        if (std::strlen(name) >= NAME_BUFFER_SIZE) {
            error = "Team name too long (max 63 chars).";
            return false;
        }
        if (score < 0) {
            error = "Score cannot be negative.";
            return false;
        }
        if (missions < 0) {
            error = "Mission count cannot be negative.";
            return false;
        }
        if (findTeamIndex(teamId) != -1) {
            error = "Duplicate team ID. Rejected.";
            return false;
        }
        if (!ensureCapacity(size + 1)) {
            error = "Memory allocation failed. Record not committed.";
            return false;
        }
        data[size].teamId = teamId;
        std::strcpy(data[size].name, name);
        data[size].score = score;
        data[size].missionsCompleted = missions;
        size++;
        return true;
    }

    bool recordMission(int teamId, int score, int missions, std::string& error) {
        error.clear();
        if (score < 0) {
            error = "Score cannot be negative.";
            return false;
        }
        if (missions < 0) {
            error = "Mission count cannot be negative.";
            return false;
        }
        int idx = findTeamIndex(teamId);
        if (idx == -1) {
            error = "Team ID not found.";
            return false;
        }
        data[idx].score = score;
        data[idx].missionsCompleted = missions;
        return true;
    }

    bool deleteTeam(int teamId, std::string& error) {
        error.clear();
        int idx = findTeamIndex(teamId);
        if (idx == -1) {
            error = "Team ID not found.";
            return false;
        }
        for (int i = idx; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
        return true;
    }

    void sortLeaderboard(int field) {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - 1 - i; j++) {
                bool swapNeeded = false;
                if (field == 0) {
                    swapNeeded = data[j].score < data[j + 1].score;
                } else {
                    swapNeeded = data[j].missionsCompleted < data[j + 1].missionsCompleted;
                }
                if (swapNeeded) {
                    Team temp = data[j];
                    data[j] = data[j + 1];
                    data[j + 1] = temp;
                }
            }
        }
    }

    void displayTeams() const {
        if (size == 0) {
            std::cout << "(leaderboard is empty)\n";
            return;
        }
        std::cout << "-------------------------------------------------------------\n";
        std::cout << " ID   | Name" << std::string(NAME_BUFFER_SIZE - 8, ' ')
                  << " | Score | Missions\n";
        std::cout << "-------------------------------------------------------------\n";
        for (int i = 0; i < size; i++) {
            std::cout << " " << data[i].teamId << " | " << data[i].name;
            for (int pad = static_cast<int>(std::strlen(data[i].name)); pad < NAME_BUFFER_SIZE - 4; pad++) {
                std::cout << ' ';
            }
            std::cout << "| " << data[i].score << " | " << data[i].missionsCompleted << "\n";
        }
        std::cout << "-------------------------------------------------------------\n";
        std::cout << "Total teams: " << size << "  (capacity: " << capacity << ")\n";
    }

    bool loadTeams(const char* filename, std::string& error) {
        error.clear();
        std::ifstream in(filename);
        if (!in.is_open()) {
            error = "Could not open file for reading.";
            return false;
        }
        std::string line;
        int lineNum = 0;
        int loaded = 0;
        int rejected = 0;
        while (std::getline(in, line)) {
            lineNum++;
            if (line.empty()) continue;
            std::stringstream ss(line);
            int id;
            std::string name;
            int score;
            int missions;
            if (!(ss >> id >> name >> score >> missions)) {
                rejected++;
                std::cout << "  [warn] line " << lineNum << ": malformed, skipped.\n";
                continue;
            }
            std::string addError;
            if (!addTeam(id, name.c_str(), score, missions, addError)) {
                rejected++;
                std::cout << "  [warn] line " << lineNum << ": " << addError << "\n";
                continue;
            }
            loaded++;
        }
        in.close();
        std::cout << "Loaded " << loaded << " team(s), rejected " << rejected << " line(s).\n";
        if (loaded == 0) {
            error = "No valid records loaded.";
            return false;
        }
        return true;
    }

    bool saveTeams(const char* filename, std::string& error) {
        error.clear();
        std::ofstream out(filename);
        if (!out.is_open()) {
            error = "Could not open file for writing.";
            return false;
        }
        for (int i = 0; i < size; i++) {
            out << data[i].teamId << " " << data[i].name << " "
                << data[i].score << " " << data[i].missionsCompleted << "\n";
        }
        out.close();
        return true;
    }

    bool searchTeam(int teamId, Team& result) const {
        int idx = findTeamIndex(teamId);
        if (idx == -1) return false;
        result = data[idx];
        return true;
    }

    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
};

static int readInt(const char* prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "  Invalid input. Enter an integer.\n";
            continue;
        }
        std::cin.ignore(10000, '\n');
        return value;
    }
}

static void readName(char* buffer, int bufferSize, const char* prompt) {
    while (true) {
        std::cout << prompt;
        std::cin.getline(buffer, bufferSize);
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "  Name too long. Try again.\n";
            continue;
        }
        if (std::strlen(buffer) == 0) {
            std::cout << "  Name cannot be empty. Try again.\n";
            continue;
        }
        return;
    }
}

static void showMenu() {
    std::cout << "=== CAMPUS QUEST LEADERBOARD ===\n";
    std::cout << "1. Register a team\n";
    std::cout << "2. Record mission points\n";
    std::cout << "3. Find a team\n";
    std::cout << "4. Remove a team\n";
    std::cout << "5. Show leaderboard\n";
    std::cout << "6. Save and exit\n";
    std::cout << "Choose: ";
}

int main() {
    Leaderboard lb;
    std::string error;
    int choice;

    while (true) {
        showMenu();
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "  Invalid choice. Try again.\n";
            continue;
        }
        std::cin.ignore(10000, '\n');

        if (choice == 1) {
            int id = readInt("  Team ID: ");
            char name[NAME_BUFFER_SIZE];
            readName(name, NAME_BUFFER_SIZE, "  Team Name: ");
            int score = readInt("  Score: ");
            int missions = readInt("  Missions Completed: ");
            if (lb.addTeam(id, name, score, missions, error)) {
                std::cout << "  Team registered.\n";
            } else {
                std::cout << "  Register failed: " << error << "\n";
            }
        } else if (choice == 2) {
            int id = readInt("  Team ID: ");
            int score = readInt("  New Score: ");
            int missions = readInt("  New Missions Completed: ");
            if (lb.recordMission(id, score, missions, error)) {
                std::cout << "  Mission points recorded.\n";
            } else {
                std::cout << "  Record failed: " << error << "\n";
            }
        } else if (choice == 3) {
            int id = readInt("  Team ID to find: ");
            Team t;
            if (lb.searchTeam(id, t)) {
                std::cout << "  Found: ID=" << t.teamId << " Name=" << t.name
                          << " Score=" << t.score << " Missions=" << t.missionsCompleted << "\n";
            } else {
                std::cout << "  Team ID not found.\n";
            }
        } else if (choice == 4) {
            int id = readInt("  Team ID to remove: ");
            if (lb.deleteTeam(id, error)) {
                std::cout << "  Team removed.\n";
            } else {
                std::cout << "  Remove failed: " << error << "\n";
            }
        } else if (choice == 5) {
            lb.sortLeaderboard(0);
            lb.displayTeams();
        } else if (choice == 6) {
            std::cout << "  Saving...\n";
            char filename[256];
            readName(filename, 256, "  Filename to save: ");
            if (lb.saveTeams(filename, error)) {
                std::cout << "  Saved successfully to '" << filename << "'. Goodbye!\n";
            } else {
                std::cout << "  Save failed: " << error << "\n";
            }
            break;
        } else {
            std::cout << "  Invalid choice.\n";
        }
    }

    return 0;
}
