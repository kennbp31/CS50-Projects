#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
     for (int k = 0; k < candidate_count; k++)
    {
        if (strcmp(candidates[k], name) == 0)
        {
            //printf("%i,\n ", ranks[k]);
            ranks[k] = rank;
            //printf("%s %s %i %i\n",candidates[k], name, ranks[k], rank);
            return true;
        }
    }
    return false;
    // TODO
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    for (int k = 0; k < candidate_count; k++)
    {
        for (int l = 0; l < candidate_count; l++)
        {
            if (ranks[k] < ranks[l])
            {
            preferences[k][l]++;
            printf("winner %s, loser %s - pref = %i\n", candidates[k], candidates[l], preferences[k][l]);
            }
            //else if (ranks[l] < ranks[k])
            //{
           // preferences[l][k]++;
            //printf("winner %s, loser %s - pref = %i\n", candidates[l], candidates[k],preferences[l][k]);
           // }
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int l = 1; l < candidate_count; l++)
        {
            if (preferences[i][l] > preferences[l][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = l;
                printf("add_pairs - winner = %s, loser = %s\n", candidates[i], candidates[l]);
                pair_count++;
            }
               // else if (preferences[l][i] > preferences[i][l])
            //{
                //pairs[pair_count].winner = l;
                //pairs[pair_count].loser = i;
                 //printf("add_pairs - winner = %s, loser = %s\n", candidates[l], candidates[i]);
                //pair_count++;
            //}
        }

    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int temp_p, temp_w, temp_l;

    for (int j = 1; j < pair_count; j++)  
    {
        for (int i = 0; i < (pair_count - j); i++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] > preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                temp_p = preferences[pairs[i].winner][pairs[i].loser];
                temp_w = pairs[i].winner;
                temp_l = pairs[i].loser;
                
                preferences[pairs[i].winner][pairs[i].loser] = preferences[pairs[i + 1].winner][pairs[i + 1].loser];
                pairs[i].winner = pairs[i + 1].winner;
                pairs[i].loser = pairs[i + 1].winner;
                
                preferences[pairs[i + 1].winner][pairs[i + 1].loser] = temp_p;
                pairs[i + 1].winner = temp_w;
                pairs[i + 1].loser = temp_l;
            }
            
            
        }
    }
    
    for (int i = 0; i < pair_count; i++)
    {
        printf("winner %s, loser %s, pref %i\n", candidates[pairs[i].winner], candidates[pairs[i].loser], preferences[pairs[i].winner][pairs[i].loser]);
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles

// if a winner is locked to someone higher than themselves, 
// they cannot be beaten by someone beaten by someone lower than who they beat
void lock_pairs(void)
{
    locked[pairs[0].winner][pairs[0].loser] = true;
    
    for (int i = 0; i < pair_count; i++)
    {
            for (int j = 0; j < pair_count; j++)
            {
            if (pairs[i].winner == pairs[j].loser && j > i) //if the winner is also a loser, and they lost to a higher rank
            {
                for (int k = 0; k < pair_count; k++)
                {
                    if (pairs[j].loser == pairs[k].winner && k > i) //if the loser was a winner and beat a higher rank than they just lost too
                    {
                        locked[i][j] = false;
                    }
                
                    else
                    {
                        locked[i][j] = true;
                    }//the new winner 
                }
               
            }
            else
            {
                locked[i][j] = true;
            }
            }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < pair_count; i++)
    { 
        bool lost = false;
        for (int j = 0; j < pair_count; j++)
        {
            if (locked[i][j] == true)
            {
                for (int k = 0; k < pair_count; k++)
                {
                    if (locked[k][i] == true)
                    {
                        lost = true;
                    }
                }
            }
        }
        
        if (lost == false)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

