/******************************************************************************
 * File:---- linkedList.h                                                     *
 * Author:-- Anton Rieutskyi                                                  *
 * Created:- 28/09/2019                                                       *
 * Modified: 20/10/2019                                                       *
 * Purpose:- Defines a generic Linked List data type that holds pointers to   *
 *           arbitrary data.                                                  *
 *           Declares functions for creating, modifying, viewing and deleting *
 *           linked lists.                                                    *
 *                                                                            *
 * References: Based on code by Rieutskyi A. (2019). COMP1001 Practical 07    *
 ******************************************************************************/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>

/******************************************************************************
 * Struct: ListNode                                                           *
 * Purpose: represents a node that holds arbitrary data and links to its      *
 *          surrounding nodes.                                                *
 * Consists of:                                                               *
 *     data - a typeless pointer to data of any type                          *
 *     next - a pointer to the next node                                      *
 *     last - a pointer to the previous node                                  *
 ******************************************************************************/
typedef struct NODE
{
    void *data;
    struct NODE *next;
    struct NODE *last;
} ListNode;



/******************************************************************************
 * Struct: LinkedList                                                         *
 * Purpose: holds the first and the last elements of a linked list and keeps  *
 *          track of its size.                                                *
 * Consists of:                                                               *
 *     head - a pointer to the first node in the list                         *
 *     tail - a pointer to the last node in the list                          *
 *     size - the number of nodes in the list                                 *
 ******************************************************************************/
typedef struct
{
    ListNode *head;
    ListNode *tail;
    int size;
} LinkedList;



/******************************************************************************
 * PrintFunc and FreeFunc: type definitions for function pointers used to     *
 *     handle the contents of a linked list.                                  *
 *     The programmer is required to provide both of these functions for each *
 *     data type they store to enable proper handling of the data inside      *
 *     the list.                                                              *
 ******************************************************************************/
typedef void ( *PrintFunc )( FILE*, void* );
typedef void ( *FreeFunc )( void* );



/* Function Prototypes */
LinkedList *createList( void );
void listInsertLast( LinkedList*, void* );
void printElement( FILE*, LinkedList*, int, PrintFunc );
void printList( FILE*, LinkedList*, PrintFunc );
void freeList( LinkedList*, FreeFunc );

#endif
