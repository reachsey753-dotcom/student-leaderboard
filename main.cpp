#include <cstdio>
#include "file.h"

int main()
{
    int choice;
    int running = 1;

    loadTeams("teams.txt");

    while (running)
    {
        printf("======STUDENT-MISSION======\n");
        printf("1. Register a team\n");
        printf("2. Record mission point\n");
        printf("3. Find a Team\n");
        printf("4. Remove a Team\n");
        printf("5. Leaderboard\n");
        printf("6. Save and exit\n");
        printf("===========================\n");
        printf("Enter option:");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int id;
            char name[MAX_NAME];
            printf("Enter team ID: ");
            scanf("%d", &id);
            printf("Enter team name: ");
            scanf("%39s", name);
            if (addTeam(id, name, 0, 0))
                printf("Team registered.\n");
            else
                printf("Registration failed (invalid or duplicate).\n");
            break;
        }
        case 2:
        {
            int id, points;
            printf("Enter team ID: ");
            scanf("%d", &id);
            printf("Enter points (1-100): ");
            scanf("%d", &points);
            if (recordMission(id, points))
                printf("Mission recorded.\n");
            else
                printf("Failed (invalid points or team not found).\n");
            break;
        }
        case 3:
        {
            int id;
            printf("Enter team ID: ");
            scanf("%d", &id);
            int idx = findTeamIndex(id);
            if (idx == -1)
                printf("Team not found.\n");
            else
                printf("ID: %d | Name: %s | Score: %d | Missions: %d\n",
                       teams[idx].id, teams[idx].name, teams[idx].score, teams[idx].missions);
            break;
        }
        case 4:
        {
            int id;
            printf("Enter team ID: ");
            scanf("%d", &id);
            if (deleteTeam(id))
                printf("Team removed.\n");
            else
                printf("Team not found.\n");
            break;
        }
        case 5:
            sortLeaderboard();
            displayTeams();
            break;
        case 6:
            if (saveTeams("teams.txt"))
                printf("Saved successfully.\n");
            else
                printf("Save failed.\n");
            cleanupTeams();
            printf("Save and exit\n");
            running = 0;
            break;
        default:
            printf("Invalid, try again!\n");
        }
    }

    return 0;
}
