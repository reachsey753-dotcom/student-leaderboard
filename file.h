#ifndef FILE_H
#define FILE_H

#define MAX_NAME 50

typedef struct
{
    int id;
    char name[MAX_NAME];
    int score;
    int missions;
} Team;

extern Team *team;
extern int size;
extern int capacity;

int findTeambyIndex(int id);
int ensuredCapacity(int required);
int addTeam(int id, const char *name, int score, int missions);
int recordMission(int id, int points);
int deleteTeam(int id);
void sortLeaderboard();
void displayTeam();
void loadTeams(const char *filename);
int saveTeams(const char *filename);
void cleanupTeams(void);

#endif
