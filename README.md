# Campus Quest Leaderboard

A console-based C++ leaderboard for campus event organizers. Each team has an ID,
name, score, and number of completed missions. The program uses a **safe dynamic
array** (class `Team` stored in a dynamically-resized `Leaderboard` array) with
guaranteed bounds checking, deep-copy semantics (Rule of Five), and full input
validation.

## Features

- Register / find / remove teams
- Record mission points
- Show the leaderboard sorted by score (descending)
- Save to a text file on exit
- Duplicate-team-ID rejection
- Dynamic array growth with no fixed upper limit
- Memory-safe: deep copy, move semantics, proper cleanup, `nothrow` allocation

## Compile

From a terminal in the project folder:

```
g++ -Wall -Wextra -o leaderboard.exe leaderboard.cpp
```

## Run

```
.\leaderboard.exe
```

## Menu

```
=== CAMPUS QUEST LEADERBOARD ===
1. Register a team
2. Record mission points
3. Find a team
4. Remove a team
5. Show leaderboard
6. Save and exit
Choose:
```

Enter a number, follow the prompts, and press Enter after each input. Option 6
asks for a filename, saves the data, then exits.

## File Format

Each line in the save file is:

```
<teamId> <name> <score> <missionsCompleted>
```

Example:

```
1 Alpha 100 5
2 Bravo 250 10
3 Charlie 50 2
```

When loading, any malformed or duplicate lines are skipped with a warning instead
of stopping the program.
