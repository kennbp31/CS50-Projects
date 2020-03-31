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
            ranks[rank] = k;
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

    for (int k = 0; k < candidate_count - 1; k++)
    {
        for (int l = 1; l < candidate_count - k; l++)
        {
            if (ranks[k] < ranks[l])
            {
            preferences[k][l]++;
            }
            else
            {
            preferences[l][k]++;
            }
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int l = 1; l < candidate_count - i; l++)
        {
            if (preferences[i][l] > preferences[l][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = l;
                pair_count++;
            }
                else if (preferences[l][i] > preferences[i][l])
            {
                pairs[pair_count].winner = l;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }

    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int strength[pair_count]; 
    int winner[pair_count]; 
    int loser[pair_count];
    strength[0] = 0;
    //if preferences[pairs.winner][pairs.loser] > order.strength
    //order.strength = preferences[pairs.winner][pairs.loser]
    for (int j = 0; j < pair_count; j++)
    {
        for (int i = 0; i < pair_count; i++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] >= strength[j])
                {
                strength[j] = preferences[pairs[i].winner][pairs[i].loser];
                winner[j] = pairs[i].winner;
                loser[j] = pairs[i].loser;
                }
            
            else if (i == pair_count - 1 )
            {
                preferences[winner[j]][loser[j]] = 0;
            }
        }
        
    }
    
    for (int i = 0; i < pair_count; i++)
    {
        pairs[i].winner = winner[i];
        pairs[i].loser = loser[i];
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        //if ()
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    printf("%s\n", candidates[pairs[0].winner]);
    return;
}

