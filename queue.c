#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *h = (struct list_head *) malloc(sizeof(struct list_head));
    if (!h) {
        return NULL;
    }
    h->prev = h;
    h->next = h;
    return h;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    struct list_head *curr = head->next;
    while (curr != head) {
        struct list_head *tmp = curr->next;
        free(curr);
        curr = tmp;
    }
    free(curr);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    // Create a new queue in global memory
    element_t *newq = malloc(sizeof(element_t));
    if (!newq) {
        return false;
    }

    // Duplicate a global memory to storage s
    newq->value = strdup(s);
    if (!newq->value) {
        free(newq);
        return false;
    }

    // Insert s at the head of queue
    list_add(&newq->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    // Create a new queue
    element_t *newq = malloc(sizeof(element_t));
    if (!newq) {
        return false;
    }

    // Duplicate s in global memory and storage in new queue
    newq->value = strdup(s);
    if (!newq->value) {
        free(newq);
        return false;
    }

    // Insert s at the tail of queue
    list_add_tail(&newq->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    // Find the element of node
    element_t *h = list_entry(head->next, element_t, list);

    // Copy the string to sp
    if ((sp) && (bufsize > 0)) {
        strncpy(sp, h->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    // Remove the element
    list_del_init(&h->list);  // head->next
    return h;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    // Find the string to sp
    element_t *h = list_entry(head->prev, element_t, list);

    // Copy the string to sp
    if ((sp) && (bufsize > 0)) {
        strncpy(sp, h->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    // Remove the element
    list_del_init(&h->list);  // head->prev
    return h;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    struct list_head *curr = head->next;
    int count = 0;
    while (curr != head) {
        count++;
        curr = curr->next;
    }
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
