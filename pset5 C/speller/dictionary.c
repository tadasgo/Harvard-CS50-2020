// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 15000;

// Hash table
node *table[N];
int amountOfWords = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // set cursor at the number produced by hash(word) = head of list
    node *cursor = table[hash(word)];

    // iterate linked list till its end
    while (cursor != NULL)
    {
        // word is found return true, else go to next node
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number for the table array
unsigned int hash(const char *word)
{
    // djb2 hash from http://www.cse.yorku.ca/~oz/hash.html
    // just the value used by the djb2 hash function
    unsigned long hash = 5381;
    int c;

    //iterates word characters
    // the value in c is checked against the null character
    while ((c = *word++))
    {
        // tolower to make hash case insesitive
        hash = ((hash << 5) + hash) + tolower(c);
    }
    // modulo of the hash table size to protect from going out of bounds
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // allocate memory + error check
    char *dictWord = malloc(LENGTH + 1);
    if (dictWord == NULL)
    {
        return false;
    }
    // open dictionary
    FILE *file = fopen(dictionary, "r");

    // ready the file
    while (fscanf(file, "%s", dictWord) != EOF)
    {
        // allocate memory for a node where dictWord will be stored
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // copy word from dictWord to node n and count total words
        strcpy(n->word, dictWord);
        amountOfWords++;

        // new node pointer set to target the same as list head to prevent losing data
        n->next = table[hash(dictWord)];
        // then list head targets new node
        table[hash(dictWord)] = n;
    }
    // close file and prevent memory leakage
    fclose(file);
    free(dictWord);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return amountOfWords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // two cursors to travel the list and free its elements without losing other elements after cleared one
    node *cursor;
    node *tmp;

    // iteration for every index in the table
    for (int i = 0; i < N; i++)
    {
        // if one line of list cleared - repeat for loop and go to other index
        if (table[i] == NULL)
        {
            continue;
        }
        // set cursors to the list head
        cursor = table[i];
        tmp = cursor;

        // iterati list until its end
        while (cursor->next != NULL)
        {
            // move cursor to next node
            cursor = cursor->next;
            // clear current node
            free(tmp);
            // temp follows cursor
            tmp = cursor;
        }
        // free cursor and with it tmp
        free(cursor);
    }
    return true;
}
