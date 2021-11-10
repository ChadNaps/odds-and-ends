#include <cs50.h>
#include <stdio.h>
#include <strings.h>

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
void merge_sort(pair array[], int size);
void merge(pair arrayL[], pair arrayR[], int size);
bool circleExists(int winner, int loser);
string findWinner(void);

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
    // Search candidates array for name
    for (int i = 0; i < candidate_count; i++)
    {
        // If found, add name to rank index of ranks array, return success
        if (!strcasecmp(candidates[i], name))
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Loop through ranks
    for (int i = 0; i < candidate_count; i++)
    {
        // Find index of favored candidate
        int favored_candidate_index = ranks[i];

        // Loop through remaining ranks
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Find index of disfavored candidates
            int disfavored_candidate_index = ranks[j];

            // Update preferences
            preferences[favored_candidate_index][disfavored_candidate_index]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Loop through preferences
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // If candidate i is preffered over candidate j, add pair to pairs array
            if (preferences[i][j] > 0 && preferences[i][j] > preferences[j][i])
            {
                pair i_j;
                i_j.winner = i;
                i_j.loser = j;
                pairs[pair_count++] = i_j;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    merge_sort(pairs, pair_count);

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!circleExists(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    string winner = findWinner();

    printf("%s\n", winner);

    return;
}

// Recursive merge sort
void merge_sort(pair array[], int size)
{
    // If size is one, array is sorted - so return
    if (size == 1)
    {
        return;
    }

    // Sort left half of array
    merge_sort(&array[0], size / 2);
    // Sort right half of array
    if (size % 2 == 1)
    {
        merge_sort(&array[size / 2], (size / 2) + 1);
    }
    else
    {
        merge_sort(&array[size / 2], size / 2);
    }
    // Merge two halves of array
    merge(&array[0], &array[size / 2], size);
}

void merge(pair arrayL[], pair arrayR[], int size)
{
    pair tempArray[size];

    // Merge left and right arrays
    for (int temp = 0, left = 0, right = 0; temp < size; temp++)
    {
        // If left winner is bigger, add him
        if (preferences[arrayL[left].winner][arrayL[left].loser] >=
            preferences[arrayR[right].winner][arrayR[right].loser] &&
            &arrayL[left] != &arrayR[0])
        {
            tempArray[temp] = arrayL[left++];
        }
        // If left array ends, add from right array instead
        else if (&arrayL[left] == &arrayR[0])
        {
            tempArray[temp] = arrayR[right++];
        }
        // If right winner is bigger, add him
        else if (preferences[arrayL[left].winner][arrayL[left].loser] <
                 preferences[arrayR[right].winner][arrayR[right].loser] &&
                 &arrayL[left] != &arrayR[0])
        {
            tempArray[temp] = arrayR[right++];
        }
        // If right array ends, add from the left array instead
        else
        {
            tempArray[temp] = arrayL[left++];
        }
    }

    // Copy values from temp array to working array
    for (int i = 0; i < size; i++)
    {
        arrayL[i] = tempArray[i];
    }
}

bool circleExists(int winner, int loser)
{
    // If loser has beat winner, circle exists
    if (locked[loser][winner])
    {
        return true;
    }

    // Else check who has beat the winner
    for (int i = 0; i < candidate_count; i++)
    {
        // If someone has
        if (locked[i][winner])
        {
            // Check if the person who beat the winner has lost to anyone
            return circleExists(i, loser);
        }
    }

    return false;
}

string findWinner(void)
{
    int winner = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner])
        {
            winner = i;
            i = -1;
        }
    }

    return candidates[winner];
}
