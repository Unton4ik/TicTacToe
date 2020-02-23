/******************************************************************************
 * File:---- linkedList.c                                                     *
 * Author:-- Anton Rieutskyi                                                  *
 * Created:- 28/10/2019                                                       *
 * Modified: 20/10/2019                                                       *
 * Purpose:- Implements the functionality for a generic linked list.          *
 *           Only implements the functions that are needed for the TicTacToe  *
 *           game: create, insert last, print element, print all, delete all. *
 * References: Code based on Rieutskyi A. (2019). COMP1001 Practical 07       *
 ******************************************************************************/
#include <stdlib.h>

#include "linkedList.h"
#include "interface.h"


/******************************************************************************
 * Function: createList                                                       *
 * Imports: none                                                              *
 * Exports: newList                                                           *
 * Purpose: allocates memory for a new linked list and sets all it values to  *
 *          zero.                                                             *
 ******************************************************************************/
LinkedList *createList( void )
{
    LinkedList *newList;
    newList = (LinkedList*) calloc( sizeof(LinkedList), 1 );
    return newList;
}



/******************************************************************************
 * Function: listInsertLast                                                   *
 * Imports: pointer to a linked list,                                         *
 *          pointer to new data                                               *
 * Exports: none                                                              *
 * Purpose: creates a new node with data and appends it to the end of a       *
 *          linked list                                                       *
 ******************************************************************************/
void listInsertLast( LinkedList *list, void *data )
{
    ListNode *newNode;

    if ( list != NULL )
    {
        /* if the list is empty, make the new node both head and tail */
        if ( list->head == NULL && list->tail == NULL )
        {
            newNode = (ListNode*) malloc( sizeof(ListNode) );
            newNode->data = data;
            newNode->next = NULL;
            newNode->last = NULL;
            list->head = newNode;
            list->tail = newNode;
            list->size = 1;
        }
        /* otherwise, put the node on the end of the list */
        else if ( list->head != NULL && list->tail != NULL )
        {
            newNode = (ListNode*) malloc( sizeof(ListNode) );
            newNode->data = data;
            newNode->next = NULL;
            newNode->last = list->tail;
            list->tail->next = newNode;
            list->tail = newNode;
            list->size++;
        }
        else
        {
            printErr( "List is corrupt or invalid" );
        }
    }
    else
    {
        printErr( "List points to NULL" );
    }
}



/******************************************************************************
 * Function: printElement                                                     *
 * Imports: output stream pointer,                                            *
 *          linked list pointer,                                              *
 *          index inside the list,                                            *
 *          printing function pointer                                         *
 * Exports: none                                                              *
 * Purpose: prints the contents of a single list element into the specified   *
 *          stream based on the provided printing function.                   *
 *          Having a file stream as a parameter enables printing to both      *
 *          terminal output and into files a lot easier since the format is   *
 *          identical.                                                        *
 ******************************************************************************/
void printElement(
    FILE *stream, LinkedList *list, int index, PrintFunc printNode )
{
    ListNode *currNode;
    int i;

    if ( list != NULL )
    {
        /* if the list is valid and the index is within the list,
         * iterate through the list until the required node and print it */
        if ( list->head != NULL && list->tail != NULL &&
            index >= 0 && index < list->size )
        {
            currNode = list->head;
            i = 0;
            while ( i < index )
            {
                currNode = currNode->next;
                i++;
            }

            printNode( stream, currNode->data );
        }
        else
        {
            if ( index < 0 || index >= list->size )
            {
                printErr( "List index outside of range" );
            }
            else
            {
                printErr( "List is corrupt or invalid" );
            }
        }
    }
    else
    {
        printErr( "List points to NULL" );
    }
}



/******************************************************************************
 * Function: printList                                                        *
 * Imports: output file stream,                                               *
 *          linked list pointer,                                              *
 *          printing function pointer                                         *
 * Exports: none                                                              *
 * Purpose: prints the contents of the entire linked list into the specified  *
 *          stream.                                                           *
 *          Functions almost identically to printElement, but calls the       *
 *          printing function on each node in the list.                       *
 ******************************************************************************/
void printList( FILE *stream, LinkedList *list, PrintFunc printNode )
{
    ListNode *currNode;

    if ( list != NULL )
    {
        if ( list->head != NULL && list->tail != NULL )
        {
            /* iterate over each node and print it */
            currNode = list->head;

            while ( currNode->next != NULL )
            {
                printNode( stream, currNode->data );
                currNode = currNode->next;
            }

            printNode( stream, currNode->data );
        }
        else
        {
            printErr( "List is corrupt or invalid" );
        }
    }
    else
    {
        printErr( "List points to NULL" );
    }
}



/******************************************************************************
 * Function: freeList                                                         *
 * Imports: linked list pointer,                                              *
 *          freeing function pointer                                          *
 * Exports: none                                                              *
 * Purpose: frees the memory allocated for a linked list and all of its nodes *
 *          using the provided freeing function.                              *
 ******************************************************************************/
void freeList( LinkedList *list, FreeFunc freeNode )
{
    ListNode *currNode;

    if ( list != NULL )
    {
        if ( list->head != NULL && list->tail != NULL )
        {
            /* iterate over each node
             * free the node's data and then the node itself */
            currNode = list->head;
            while ( currNode->next != NULL )
            {
                currNode = currNode->next;
                freeNode( currNode->last->data );
                free( currNode->last );
            }

            freeNode( currNode->data );
            free( currNode );

            /* when done, free the list itself */
            free( list );
        }
        else if ( list->head == NULL && list->tail == NULL )
        {
            free( list );
        }
        else
        {
            printErr( "List is corrupt or invalid" );
        }
    }
    else
    {
        printErr( "List points to NULL" );
    }
}
