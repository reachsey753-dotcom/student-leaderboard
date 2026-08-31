# Student Mission Leaderboard

A lightweight command-line application for registering student teams, recording mission points, and displaying a ranked leaderboard. The project is written in C++ and stores team data locally so progress is available the next time the program starts.

## Features

- Register teams with a unique positive ID and name
- Award `1–100` points for each completed mission
- Search for a team by ID
- Remove a team from the leaderboard
- Rank teams by total score
- Break tied scores by number of completed missions
- Save and load leaderboard data from `teams.txt`
- Grow the in-memory team list dynamically

## Project structure

```text
student-leaderboard/
├── main.cpp   # Console menu and user interaction
├── file.cpp   # Team management, ranking, and persistence
├── file.h     # Team structure and function declarations
└── README.md
```

## Requirements

- A C++ compiler with C++11 support or newer, such as GCC, Clang, or MSVC
- A terminal or command prompt

No third-party libraries are required.

## Getting started

### 1. Clone the repository

```bash
git clone https://github.com/reachsey753-dotcom/student-leaderboard.git
cd student-leaderboard
```

### 2. Build the program

Using GCC or Clang on Linux/macOS:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp file.cpp -o student-leaderboard
```

Using MinGW on Windows:

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp file.cpp -o student-leaderboard.exe
```

### 3. Run it

Linux/macOS:

```bash
./student-leaderboard
```

Windows PowerShell:

```powershell
.\student-leaderboard.exe
```

## How to use

The program displays this menu:

```text
======STUDENT-MISSION======
1. Register a team
2. Record mission point
3. Find a Team
4. Remove a Team
5. Leaderboard
6. Save and exit
===========================
```

Choose an option by entering its number. Use **Save and exit** when finished so the current leaderboard is written to disk.

### Input rules

- Team IDs must be positive and unique.
- Team names must contain fewer than 50 characters. The current console input accepts a single word.
- Mission points must be between `1` and `100`, inclusive.
- Scores and mission counts cannot be negative.

## Ranking rules

Teams are ordered using the following criteria:

1. Higher total score
2. More completed missions when scores are equal

## Data storage

The application creates `teams.txt` in the current working directory when you save. Each line uses this format:

```text
team_id|team_name|score|missions
```

Example:

```text
101|Alpha|250|4
102|Phoenix|190|3
```

The file is loaded automatically when the application starts. Keep `teams.txt` in the same directory where you run the executable.

## Possible improvements

- Accept team names containing spaces
- Add safer handling for invalid non-numeric input
- Add automated tests
- Support additional ranking and reporting options
- Replace the text file with a structured format or database

## Contributing

Contributions are welcome. Fork the repository, create a focused branch, make and test your changes, and then open a pull request describing what you improved.
