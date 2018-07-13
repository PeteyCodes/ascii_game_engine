
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"


/* Internal Interface */
static list_item_t * list_item_create(void *data);


/* Public Interface */


// Allocation / Destruction

list_t * list_create(void (*destroy_item_data)(void *)) {
    list_t *list = malloc(sizeof(list_t));
    list->first = NULL;
    list->last = NULL;
    list->count = 0;
    list->destroy_item_data = destroy_item_data;

    return list;
}

void list_destroy(list_t *list) {
    list_item_t *item = list->first;
    while (item != NULL) {
        if ((item->data != NULL) && (list->destroy_item_data != NULL)) {
            list->destroy_item_data(item->data);
        }
        list_item_t *item_to_delete = item;
        item = item->next;
        free(item_to_delete);
    }
}


// Data Access --

int32_t list_count(list_t *list) {
    return list->count;
}

void * list_get_at_index(list_t *list, int32_t idx) {
    if ((idx > (list->count - 1)) || (idx < 0)) { return NULL; }

    list_item_t *curr_item = list->first;
    for (int32_t curr = 0; curr < idx; curr += 1) {
        curr_item = curr_item->next;
    }

    return curr_item->data;
}

int32_t list_index_of(list_t *list, void *value) {
    list_item_t *item = list->first;
    int32_t idx = 0;
    while (item != NULL) {
        if (item->data == value) {
            return idx;
        } else {
            item = item->next;
            idx += 1;
        }
    }

    return -1;  // Not found
}


// Data Manipulation --

void list_append(list_t *list, void *data) {
    list_item_t *item = list_item_create(data);

    item->prev = list->last;
    item->next = NULL;

    if (list->last != NULL) {
        list->last->next = item;
    }
    list->last = item;
    if (list->first == NULL) { 
        list->first = item; 
    }
    list->count += 1;
}

void list_insert_at(list_t *list, int32_t idx, void *data) {
    list_item_t *item = list_item_create(data);

    if (idx >= list->count) { 
        list_append(list, data);
        return;
    }
    if (idx <= 0) { 
        list_prepend(list, data); 
        return;
    }

    // Loop until we get to the item we're inserting after
    list_item_t *curr_item = list->first;
    for (int32_t curr = 1; curr < idx; curr += 1) {
        curr_item = curr_item->next;
    }

    item->next = curr_item->next;
    item->prev = curr_item;
    curr_item->next->prev = item;
    curr_item->next = item;
    
    list->count += 1; 
}

void list_prepend(list_t *list, void *data) {
    list_item_t *item = list_item_create(data);

    item->next = list->first;
    item->prev = NULL;

    if (list->first != NULL) {
        list->first->prev = item;
    }
    list->first = item;
    list->count += 1;
}

void * list_remove_at(list_t *list, int32_t idx) {
    void *data = NULL;

    if (idx <= 0) {
        return list_remove_first(list);
    }
    if (idx >= list->count - 1) {
        return list_remove_last(list);
    }

    list_item_t *item_to_remove = list->first;
    for (int32_t curr = 0; curr < idx; curr += 1) {
        item_to_remove = item_to_remove->next;
    }
    
    if (item_to_remove->prev != NULL) {
        item_to_remove->prev->next = item_to_remove->next;
    }
    if (item_to_remove->next != NULL) {
        item_to_remove->next->prev = item_to_remove->prev;
    }

    data = item_to_remove->data;
    free(item_to_remove); 
    list->count -= 1;
    
    return data;
}

void list_remove_data(list_t *list, void *data) {
    int32_t idx = list_index_of(list, data);
    if (idx >= 0) {
        list_remove_at(list, idx);
    }
}

void * list_remove_first(list_t *list) {
    void *data = NULL;
    if (list->first != NULL) {
        list_item_t *item_to_remove = list->first;
        list_item_t *new_first = list->first->next;
        if (new_first != NULL) {
            new_first->prev = NULL;
        }
        list->first = new_first;

        data = item_to_remove->data;
        free(item_to_remove); 
        list->count -= 1;
    }
    
    return data;
}

void * list_remove_last(list_t *list) {
    void *data = NULL;

    if (list->last != NULL) {
        list_item_t *item_to_remove = list->last;
        if (list->last->prev != NULL) {
            list->last->prev->next = NULL;
        }
        
        data = item_to_remove->data;
        free(item_to_remove);
        list->count -= 1;
    }
    
    return data;
}


// Iteration --

list_iterator_t * list_iterator(list_t *list) {
    list_iterator_t *iter = malloc(sizeof(list_iterator_t));
    iter->list = list;
    iter->item = NULL;      // This signals that the iterator hasn't started yet

    return iter;
}

void * list_iterator_data(list_iterator_t *iter) {
    return iter->item->data;
}

void list_iterator_destroy(list_iterator_t *iter) {
    free(iter);
}

bool list_iterator_next(list_iterator_t *iter) {
    if (iter->item == NULL) {
        iter->item = iter->list->first;
    } else {
        iter->item = iter->item->next;
    }
    if (iter->item == NULL) {
        return false;
    }
    return true;
}


/* Internal Functions */

static list_item_t * list_item_create(void *data) {
    list_item_t *item = malloc(sizeof(list_item_t));
    item->data = data;
    item->prev = NULL;
    item->next = NULL;

    return item;
}


/* 
    Test Harness 
*/

#ifdef __TEST__
int main() {
    
    printf("-- RUNNING ALL LIST TESTS --\n");
    list_t *list = list_create(NULL);

    char *item3 = "Item 3";
    list_append(list, "Item 1");
    list_append(list, "Item 2");
    list_append(list, item3);
    list_append(list, "Item 4");

    printf("--------------\n");
    list_iterator_t *i0 = list_iterator(list);
    while (list_iterator_next(i0)) {
        char *s = (char *)list_iterator_data(i0);
        printf("%s\n", s);
    }
    list_iterator_destroy(i0);

    printf("--------------\n");
    printf("%s\n", list_get_at_index(list, 0));
    printf("%s\n", list_get_at_index(list, 3));
    printf("%s\n", list_get_at_index(list, 2));

    int32_t idx = list_index_of(list, item3);
    printf("Index of item3: %d\n", list_index_of(list, item3));
    printf("Index of non-item: %d\n", list_index_of(list, "Item 0"));

    printf("list_count: %d\n", list_count(list));
    list_prepend(list, "Item 0");

    printf("--------------\n");
    list_iterator_t *i1 = list_iterator(list);
    while (list_iterator_next(i1)) {
        char *s = (char *)list_iterator_data(i1);
        printf("%s\n", s);
    }
    list_iterator_destroy(i1);

    printf("list_count: %d\n", list_count(list));
    list_insert_at(list, 10, "Item 6");
    list_insert_at(list, -1, "Item -1");
    printf("list_count before i_a: %d\n", list_count(list));
    list_insert_at(list, list_count(list)-1, "Item 5");    
    
    printf("list_count: %d\n", list_count(list));
    printf("--------------\n");
    list_iterator_t *i2 = list_iterator(list);
    while (list_iterator_next(i2)) {
        char *s = (char *)list_iterator_data(i2);
        printf("%s\n", s);
    }
    list_iterator_destroy(i2);
    
    printf("Remove first: %s\n", list_remove_first(list));
    printf("Remove last: %s\n", list_remove_last(list));
    
    printf("list_count: %d\n", list_count(list));
    printf("--------------\n");
    list_iterator_t *i3 = list_iterator(list);
    while (list_iterator_next(i3)) {
        char *s = (char *)list_iterator_data(i3);
        printf("%s\n", s);
    }
    list_iterator_destroy(i3);

    printf("Remove at 3: %s\n", list_remove_at(list, 3));

    list_destroy(list);

    return 0;
}
#endif


