# Safe Dynamic Array Leaderboard

A console-based C++ leaderboard for event organizers. Each team has an ID, name,
score, and number of completed missions. The program uses a **safe dynamic array**
(class `Team` stored in a dynamically-resized `Leaderboard` array) with guaranteed
bounds checking, deep-copy semantics (Rule of Five), and full input validation.

## Features

- Add / remove / update teams
- Sort by score or by completed missions (descending)
- Search teams by ID
- Save to and load from a text file
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
 1. Add Team
 2. Record Mission (update score + missions)
 3. Delete Team
 4. Sort by Score & Display
 5. Sort by Missions & Display
 6. Display Unsorted
 7. Search by ID
 8. Save to File
 9. Load from File
10. Exit
```

Enter a number, follow the prompts, and press Enter after each input.

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
