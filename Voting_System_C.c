#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VOTERS 100
#define MAX_CANDIDATES 5
#define MAX_ADDRESSES 10
#define MAX_ADMINS 5

typedef struct
{
    int candidateID;
    char name[50];
} Candidate;

typedef union
{
    int candidateID;
    char invalidChoice[50];
} VoteChoice;

typedef struct
{
    char address[50];
    int numVoters;
    int voterNumbers[MAX_VOTERS];
    char aadharNumbers[MAX_VOTERS][12];
    VoteChoice votes[MAX_VOTERS];
} VotingPoll;

enum CandidateNames
{
    Taylor_Swift,
    Will_Smith,
    Tom_Hanks,
    Benedict,
    Robert_Downey,
    Emma_Watson
};

typedef struct
{
    char username[50];
    char password[50];
    int isAdmin; // Add isAdmin field to indicate admin status
} Admin;

int adminLogin(Admin admins[], int numAdmins)
{
    char username[50];
    char password[50];

    printf("Admin Login\n");
    printf("Enter username: ");
    scanf("%s", username);

    for (int i = 0; i < numAdmins; i++)
    {
        if (strcmp(admins[i].username, username) == 0)
        {
            printf("Enter password: ");
            scanf("%s", password);

            if (strcmp(admins[i].password, password) == 0)
            {
                return i; // Successful login, return admin index
            }
            else
            {
                printf("Incorrect password!\n");
                return -1; // Incorrect password
            }
        }
    }

    printf("Admin not found!\n");
    return -1; // Admin not found
}

void createPoll(VotingPoll polls[], int *numPolls, int adminIndex)
{
    if (*numPolls < MAX_ADDRESSES)
    {
        printf("Enter the address for the new poll: ");
        scanf("%s", polls[*numPolls].address);
        polls[*numPolls].numVoters = 0;
        (*numPolls)++;
        printf("New poll created!\n");
    }
    else
    {
        printf("Maximum number of polls reached!\n");
    }
}

int isAadharDuplicate(int voterNumbers[], char aadharNumbers[][12], int numVoters, char *aadhar)
{
    for (int i = 0; i < numVoters; i++)
    {
        if (strcmp(aadharNumbers[i], aadhar) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void recordVote(VotingPoll *poll)
{
    char name[50];
    char aadhar[12];
    int voterNumber;
    int candidateChoice;

    printf("Enter your name: ");
    scanf("%s", name);

    printf("Enter your voter number: ");
    scanf("%d", &voterNumber);

    for (int i = 0; i < poll->numVoters; i++)
    {
        if (poll->voterNumbers[i] == voterNumber)
        {
            printf("Vote already casted!\n");
            return;
        }
    }

    printf("Enter your Aadhar number: ");
    scanf("%s", aadhar);

    if (isAadharDuplicate(poll->voterNumbers, poll->aadharNumbers, poll->numVoters, aadhar))
    {
        printf("You have already casted the vote.\n");
        return;
    }

    printf("Select a candidate:\n");
    printf("1. Taylor_Swift\n2. Will_Smith\n3. Tom_Hanks\n4. Benedict\n5. Robert_Downey\n6. Emma_Watson\n");
    printf("Enter the number of the candidate you want to vote for: ");
    scanf("%d", &candidateChoice);

    if (candidateChoice < 1 || candidateChoice > MAX_CANDIDATES)
    {
        printf("Invalid candidate choice!\n");
        strcpy(poll->votes[poll->numVoters].invalidChoice, "Invalid Choice");
    }
    else
    {
        poll->votes[poll->numVoters].candidateID = candidateChoice - 1;
    }

    poll->voterNumbers[poll->numVoters] = voterNumber;
    strcpy(poll->aadharNumbers[poll->numVoters], aadhar);
    (poll->numVoters)++;

    printf("Vote recorded successfully!\n");
}

void countVotes(VotingPoll *poll)
{
    int candidateVotes[MAX_CANDIDATES] = {0};

    for (int i = 0; i < poll->numVoters; i++)
    {
        if (poll->votes[i].candidateID >= 0 && poll->votes[i].candidateID < MAX_CANDIDATES)
        {
            candidateVotes[poll->votes[i].candidateID]++;
        }
    }

    printf("\n===== Voting Results =====\n");
    for (int i = 0; i < MAX_CANDIDATES; i++)
    {
        printf("Candidate %d received: %d votes\n", i + 1, candidateVotes[i]);
    }
}

int main()
{
    VotingPoll polls[MAX_ADDRESSES];
    int numPolls = 0;

    Admin admins[MAX_ADMINS];
    int numAdmins = 0;

    int pollResultsDisplayed[MAX_ADDRESSES] = {0};

    // Add some admin accounts (you can extend this with more admins)
    strcpy(admins[numAdmins].username, "admin1");
    strcpy(admins[numAdmins].password, "password1");
    admins[numAdmins].isAdmin = 1; // Mark as admin
    numAdmins++;

    while (1)
    {
        printf("\n===== Voting System =====\n");
        printf("1. Admin Login\n");
        printf("2. Record Vote\n");
        printf("3. Display Votes\n");
        printf("4. Create New Poll\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            int adminIndex = adminLogin(admins, numAdmins);
            if (adminIndex != -1)
            {
                printf("Admin login successful!\n");
                if (admins[adminIndex].isAdmin)
                {
                    // Admin is allowed to create polls
                    while (1)
                    {
                        printf("\n1. Create New Poll\n");
                        printf("2. Back to Main Menu\n");
                        printf("Enter your choice: ");
                        int adminChoice;
                        scanf("%d", &adminChoice);
                        switch (adminChoice)
                        {
                        case 1:
                            createPoll(polls, &numPolls, adminIndex);
                            continue;
                        case 2:

                            break;
                        default:
                            printf("Invalid choice! Please select a valid option.\n");
                        }
                        break;
                    }
                }
                else
                {
                    printf("You are not authorized to create polls.\n");
                }
            }
            else
            {
                printf("Admin login failed!\n");
            }
        }
        break;
        case 2:
            if (numPolls == 0)
            {
                printf("No polls created yet. Create a poll first.\n");
            }
            else
            {
                printf("Enter the address for voting: ");
                char address[50];
                scanf("%s", address);

                int pollIndex = -1;
                for (int i = 0; i < numPolls; i++)
                {
                    if (strcmp(polls[i].address, address) == 0)
                    {
                        pollIndex = i;
                        break;
                    }
                }

                if (pollIndex != -1)
                {
                    recordVote(&polls[pollIndex]);
                }
                else
                {
                    printf("No poll found with the given address.\n");
                }
            }
            break;
        case 3:
            if (numPolls == 0)
            {
                printf("No polls created yet. Create a poll first.\n");
            }
            else
            {
                printf("Enter the address for which you want to display votes: ");
                char address[50];
                scanf("%s", address);

                int pollIndex = -1;
                for (int i = 0; i < numPolls; i++)
                {
                    if (strcmp(polls[i].address, address) == 0)
                    {
                        pollIndex = i;
                        break;
                    }
                }

                if (pollIndex != -1)
                {
                    countVotes(&polls[pollIndex]);
                }
                else
                {
                    printf("No poll found with the given address.\n");
                }
            }
            break;
        case 4:
            if (numPolls == 0)
            {
                printf("No polls created yet. Create a poll first.\n");
            }
            else
            {
                printf("You must be an admin to create a poll.\n");
            }
            break;
        case 5:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice! Please select a valid option.\n");
        }
    }

    return 0;
}
