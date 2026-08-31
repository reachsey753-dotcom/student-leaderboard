#include "file.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

Team *teams = NULL;
int size = 0;
int capacity = 0;

int findTeamIndex(int id)
{
    for (int i = 0; i < size; i++)
    {
        if (teams[i].id == id)
            return i;
    }
    return -1;
}

int ensureCapacity(int required)
{
    if (capacity >= required)
        return 1;

    int newCapacity = (capacity == 0) ? 4 : capacity * 2;
    while (newCapacity < required)
        newCapacity *= 2;

    Team *temp = (Team *)realloc(teams, newCapacity * sizeof(Team));
    if (temp == NULL)
        return 0;

    teams = temp;
    capacity = newCapacity;
    return 1;
}

int addTeam(int id, const char *name, int score, int missions)
{
    if (id <= 0)
        return 0;
    if (name == NULL || strlen(name) == 0 || strlen(name) >= MAX_NAME)
        return 0;
    if (score < 0 || missions < 0)
        return 0;
    if (findTeamIndex(id) != -1)
        return 0;
    if (!ensureCapacity(size + 1))
        return 0;

    teams[size].id = id;
    strncpy(teams[size].name, name, MAX_NAME - 1);
    teams[size].name[MAX_NAME - 1] = '\0';
    teams[size].score = score;
    teams[size].missions = missions;
    size++;
    return 1;
}

int recordMission(int id, int points)
{
    if (points < 1 || points > 100)
        return 0;

    int idx = findTeamIndex(id);
    if (idx == -1)
        return 0;

    teams[idx].score += points;
    teams[idx].missions += 1;
    return 1;
}

int deleteTeam(int id)
{
    int idx = findTeamIndex(id);
    if (idx == -1)
        return 0;

    for (int i = idx; i < size - 1; i++)
        teams[i] = teams[i + 1];

    size--;
    return 1;
}

void sortLeaderboard(void)
{
    for (int i = 0; i < size - 1; i++)
    {
        int best = i;
        for (int j = i + 1; j < size; j++)
        {
            if (teams[j].score > teams[best].score ||
                (teams[j].score == teams[best].score && teams[j].missions > teams[best].missions))
            {
                best = j;
            }
        }
        if (best != i)
        {
            Team t = teams[i];
            teams[i] = teams[best];
            teams[best] = t;
        }
    }
}

void displayTeams(void)
{
    if (size == 0)
    {
        printf("No teams registered.\n");
        return;
    }

    for (int i = 0; i < size; i++)
    {
        printf("ID: %d | Name: %s | Score: %d | Missions: %d\n",
               teams[i].id, teams[i].name, teams[i].score, teams[i].missions);
    }
}

void loadTeams(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return;

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int id, score, missions;
        char name[MAX_NAME];

        int fields = sscanf(line, "%d|%39[^|]|%d|%d", &id, name, &score, &missions);
        if (fields != 4)
            continue;

        addTeam(id, name, score, missions);
    }

    fclose(fp);
}

int saveTeams(const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
        return 0;

    for (int i = 0; i < size; i++)
    {
        if (fprintf(fp, "%d|%s|%d|%d\n", teams[i].id, teams[i].name, teams[i].score, teams[i].missions) < 0)
        {
            fclose(fp);
            return 0;
        }
    }

    if (fclose(fp) != 0)
        return 0;

    return 1;
}

void cleanupTeams(void)
{
    free(teams);
    teams = NULL;
    size = 0;
    capacity = 0;
}
