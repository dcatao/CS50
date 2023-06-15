#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a linked list
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in the hash table
#define N 20000

// Hash table
node *table[N];

// Number of words in the dictionary
unsigned int word_count = 0;

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Generate a hash value based on the word
    unsigned int hash_value = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        hash_value = (hash_value << 2) ^ word[i];
    }
    return hash_value % N;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get the hash value for the word
    unsigned int hash_value = hash(word);

    // Traverse the linked list at the hash value index
    node *cursor = table[hash_value];
    while (cursor != NULL)
    {
        // Compare word with the current node's word (case-insensitive)
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true; // Word found
        }
        cursor = cursor->next;
    }

    return false; // Word not found
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Could not open dictionary file.\n");
        return false;
    }

    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a new node for the word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(file);
            return false;
        }
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Get the hash value for the word
        unsigned int hash_value = hash(word);

        // Insert the new node into the linked list at the hash value index
        if (table[hash_value] == NULL)
        {
            // No nodes at this index yet, insert the new node as the first one
            table[hash_value] = new_node;
        }
        else
        {
            // Add the new node at the beginning of the list
            new_node->next = table[hash_value];
            table[hash_value] = new_node;
        }

        // Increment word count
        word_count++;
    }

    // Close the dictionary file
    fclose(file);

    return true;
}

// Returns the number of words in the dictionary
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        // Free the memory for each node in the linked list at the current index
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}
