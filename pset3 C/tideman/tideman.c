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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name,  candidates[i]) == 0)
        {
            // update ranks[<user’s preference num>] = <num of candidate in the candidates[]>
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // iterates pair winners with losers
    for (int i = 0; i < candidate_count; i++)
    {
        // iterates losers (2, 3 etc)
        for (int j = i + 1; j < candidate_count; j++)
        {
            // number of voters who prefer candidate i over candidate j (like in table)
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                // if ij pair is prefered over ji set winners and losers
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                // update the global variable for each pair
                pair_count++;
            }
            // same just vice versa
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // init array with winning differences
    int strength[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        // how much a candidate wins over another
        strength[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }
    // bubble sort
    // loop for walking throught the array
    for (int i = 0; i < pair_count; i++)
    {
        // swapped keeps track of swapping
        int swapped = 0;
        // loop for comparison
        for (int j = i + 1; j < pair_count; j++)
        {
            // descending order, ascending - strength[j] < strength[i]
            if (strength[j] > strength[i])
            {
                // swap if greater is at the rear position
                pair tmp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = tmp;
                int tmp2 = strength[j];
                strength[j] = strength[i];
                strength[i] = tmp2;
                swapped = 1;
            }
        }
        // if there is not swapping in the last swap, then the array is already sorted
        if (swapped == 0)
        {
            return;
        }
    }
}

//function for preventing cycles
//a = initial winner, b = initial loser
bool loopcheck(int a, int b)
{
    // See if the current loser locks onto the current winner
    if (locked[b][a] == true)
    {
        // If it does - true
        return true;
    }
    // see if anyone else locks onto the current winner;
    for (int i = 0; i < candidate_count; i++)
    {
        // Recursively call the cycle checker, to see if the current loser locks onto 'i'
        if (locked[i][a] == true)
        {
            // pass the values so the base case checker does [loser][i], not [i][loser]
            // the base case checks whether the initial LOSER locked onto the winner
            return loopcheck(i, b);
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!loopcheck(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int i, j;
    for (j = 0; j < candidate_count; j++)
    {
        for (i = 0; i < candidate_count ; i++)
        {
            if (locked[i][j])
            {
                break;
            }
        }
        if (i == candidate_count)
        {
            printf("%s\n", candidates[j]);
        }
    }
    return;
}