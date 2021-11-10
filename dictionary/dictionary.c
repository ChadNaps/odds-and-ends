// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
    struct node *prev;
}
node;

// Number of buckets in hash table
const unsigned int N = 190787; // (75% load factor - 1 is prime) based on number of values in large dictionary

// Number of words loaded in dictionary
unsigned int W = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Convert word to lower case for proper hash_code
    int bufferLen = strlen(word) + 1;
    char *buffer = malloc(sizeof(char) * bufferLen);

    for (int i = 0; i < bufferLen; i++)
    {
        buffer[i] = tolower(word[i]);
    }

    // Prepare to check dictionary for word
    unsigned int hash_code = hash(buffer);
    node **itr = &(table[hash_code]);
    bool isInDictionary = false;

    // Search dictionary for word

    // If table has an entry at that location
    if (*itr != NULL)
    {
        // Check if it contains our word
        if (strcmp((*itr)->word, buffer) == 0)
        {
            isInDictionary = true;
        }

        // If it wasn't found and more entries exist at that index, check them too
        while (!isInDictionary && (*itr)->next != NULL)
        {
            itr = &(*itr)->next;

            if (strcmp((*itr)->word, buffer) == 0)
            {
                isInDictionary = true;
            }
        }
    }

    free(buffer);

    return isInDictionary;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash_code = 0;

    for (;*word != '\0'; word++)
    {
        hash_code = ((hash_code + *word) * 31) % N;
    }

    return hash_code;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dict;

    if ((dict = fopen(dictionary, "r")) == NULL)
    {
        return false;
    }

    // Update all table pointers to NULL
    for (unsigned int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char *buffer = malloc(sizeof(char) * (LENGTH + 1));

    // Format code scans word without new line into buffer
    while (fscanf(dict, "%[^\n]%*c", buffer) != EOF)
    {
        // Initialize hash code
        unsigned int hash_code = 0;

        // Hash word
        hash_code = hash(buffer);

        // Build node
        node *temp = malloc(sizeof(node));
        strcpy(temp->word, buffer);
        temp->next = NULL;
        temp->prev = NULL;

        // Insert in table
        node **searchItr = &table[hash_code];

        // Determine if element is first in table[index], else find end and insert there
        if (*searchItr == NULL)
        {
            // Insert
            *searchItr = temp;
        }
        else
        {
            // Find end of linked list
            while ((*searchItr)->next != NULL)
            {
                searchItr = &(*searchItr)->next;
            }

            // Insert
            temp->prev = *searchItr;
            (*searchItr)->next = temp;
        }

        // Update number of words in dictionary
        W++;
    }

    free(buffer);

    if (fclose(dict) == EOF)
    {
        return false;
    }

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return W;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    unsigned int i = 0;

    // Iterate through entire table
    while (i < N)
    {
        node **itr = &table[i++];

        // If index is empty: move on
        if (*itr == NULL)
        {
            continue;
        }

        // If index is not empty: find last element
        while ((*itr)->next != NULL)
        {
            *itr = (*itr)->next;
        }

        // While not pointing at first element, step back and free next element's memory
        while ((*itr)->prev != NULL)
        {
            *itr = (*itr)->prev;
            free((*itr)->next);

            // Update dictionary words loaded
            W--;
        }

        // Finally, free first element's memory
        free(*itr);
        W--;
    }

    if (W == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

