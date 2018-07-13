#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdint.h>


/*
 * Generic list - list data-structure for homogeneous data.
 * Implemented as a doubly-linked list.
 *
 * Written by PeteyCodes (@peteycodes, twitch.tv/peteycodes) 
 */


/** Type definitions **/

typedef struct list_item_s {
    void *data;
    struct list_item_s *prev;
    struct list_item_s *next;
} list_item_t;
// User should never have to interact with list_item_t values directly.

typedef struct list_s {
    list_item_t *first;
    list_item_t *last;
    int32_t count;
    void (*destroy_item_data)(void *);
} list_t;
// Should only use the list via functions, not direct property access

typedef struct {
    list_t *list;
    list_item_t *item;
} list_iterator_t;
// Should only use the iterator via functions, not direct property access


/** Public Interface **/

// Allocation / Destruction

list_t * list_create(void (*destroy_item_data)(void *));

void list_destroy(list_t *list);


// Data Access --

int32_t list_count(list_t *list);

void * list_get_at_index(list_t *list, int32_t idx);

int32_t list_index_of(list_t *list, void *value);


// Data Manipulation --

/**
 *  Insert given data at the end of the list.
 *
 *  All data stored in the list items should be allocated on the heap, not the stack.
*/
void list_append(list_t *list, void *data);

/**
 *  Insert given data in front of the item at the given index, so that 
 *  the inserted item will have the given index.
 *
 *  If the given index is out of bounds, the data will be added to the 
 *  appropriate end of the list.
 *
 *  All data stored in the list items should be allocated on the heap, not the stack.
*/
void list_insert_at(list_t *list, int32_t idx, void *data);

/**
 * Push the given data onto the front of the list.
 *
 *  All data stored in the list items should be allocated on the heap, not the stack.
 */
void list_prepend(list_t *list, void *data);

/**
 *  Remove the item at the given index, and return its data.
 *
 *  It is the caller's responsibility to clean up the memory for the
 *  data returned.
 */
void * list_remove_at(list_t *list, int32_t idx);

/**
 *  Remove the item containing the given data.
 */
void list_remove_data(list_t *list, void *data);

/**
 *  Remove the first item in the list and return its data.
 *
 *  It is the caller's responsibility to clean up the memory for the
 *  data returned.
 */
void * list_remove_first(list_t *list);

/**
 *  Remove the last item in the list and return its data.
 *
 *  It is the caller's responsibility to clean up the memory for the
 *  data returned.
 */
void * list_remove_last(list_t *list);


/** List Iteration **/
/*
 *  Example usage:
 *      list_iterator_t *iter = list_iterator(list);
 *      while (list_iterator_next(iter)) {
 *          // call list_iterator_data() to access the current item's data
 *      }
 *      list_iterator_destroy(iter);
 */

/**
 *  Create an iterator for the given list.
 */
list_iterator_t * list_iterator(list_t *list);

/**
 *  Access the data from the current iterator item.
 */
void * list_iterator_data(list_iterator_t *iter);

/**
 *  Destroy the given list iterator. The list will be unaffected.
 */
void list_iterator_destroy(list_iterator_t *iter);

/**
 *  Move to the next item in the list.
 *  @return - true if the iterator was moved. false if EOL.
 */ 
bool list_iterator_next(list_iterator_t *iter);



    
#endif


/*
 The collection will not allocate/free any memory associated with list item data. It will only 
 deal with memory management of the list itself.

 All data stored in the list items should be allocated on the heap, not the stack.
 */




/*
What do you do with collections?

- append an item
- iterate over each item
- get a count of the items
- grab an item by index
- search for an item by value
- determine if the list contains a given value
- prepend an item
- insert an item
- remove an item from the list by index
- remove an item from the head of the list
- remove an item from the tail of the list
- remove item containing given data

- sort the items
- reverse the items
- split the list into two
- join multiple lists into one
- grab a subset of the list

- Look to Javascript arrays for a useful set of operations?
*/

/*
 The collection will not allocate/free any memory associated with list item data. It will only 
 deal with memory management of the list itself.

 All data stored in the list items should be allocated on the heap, not the stack.
 */


// User should never have to interact with list_item_t values directly.
// Should only use the list via functions, not direct property access




