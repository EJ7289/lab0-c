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
        element_t *elem = list_entry(curr, element_t, list);
        free(elem->value);
        free(elem);
        curr = tmp;
    }
    free(head);
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
    if (!head || list_empty(head))
        return false;

    // Find the middle point
    struct list_head **indir = &head->next;
    const struct list_head *tail = head->prev;
    for (const struct list_head *fast = head->next;
         fast != tail && fast->next != tail; fast = fast->next->next) {
        indir = &(*indir)->next;
    }

    // Remove middle point from the queue
    struct list_head *tmp = *indir;
    list_del(tmp);

    // Delete the middle point
    element_t *elem = list_entry(tmp, element_t, list);
    free(elem->value);
    free(elem);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    element_t *entry = NULL, *safe = NULL;
    bool del = false;

    list_for_each_entry_safe (entry, safe, head, list) {
        if (&safe->list != head && strcmp(entry->value, safe->value) == 0) {
            del = true;
            list_del_init(&entry->list);
            free(entry->value);
            free(entry);
        } else if (del) {
            del = false;
            list_del_init(&entry->list);
            free(entry->value);
            free(entry);
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *curr = head->next;
    struct list_head *next = curr->next;

    for (; curr != head && curr->next != head;
         curr = curr->next->next, next = curr->next) {
        char *tmp = list_entry(curr, element_t, list)->value;
        list_entry(curr, element_t, list)->value =
            list_entry(next, element_t, list)->value;
        list_entry(next, element_t, list)->value = tmp;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *front = head->next;
    struct list_head *back = head->prev;

    for (; front != back && front->prev != back;
         front = front->next, back = back->prev) {
        char *tmp = list_entry(front, element_t, list)->value;
        list_entry(front, element_t, list)->value =
            list_entry(back, element_t, list)->value;
        list_entry(back, element_t, list)->value = tmp;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || list_is_singular(head) || k < 2) {
        return;
    }

    struct list_head *front = head->next;
    struct list_head *back = front;

    for (;; front = back->next, back = front) {
        for (int i = 0; i < k - 1; i++) {
            if (back->next == head || back == head)
                return;
            back = back->next;
        }

        struct list_head *tmp = back;
        for (; front != back && front != back->next;
             front = front->next, back = back->prev) {
            char *tmp2 = list_entry(front, element_t, list)->value;
            list_entry(front, element_t, list)->value =
                list_entry(back, element_t, list)->value;
            list_entry(back, element_t, list)->value = tmp2;
        }
        back = tmp;
    }
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
    if (!head || list_empty(head))
        return 0;
    else if (list_is_singular(head))
        return 1;

    struct list_head *prev = head->next;
    struct list_head *curr = prev->next;
    int count = 1;
    bool lbig = false;

    while (curr != head) {
        element_t *elem_p = list_entry(prev, element_t, list);
        const element_t *elem_c = list_entry(curr, element_t, list);

        // decide lbig
        if (strcmp(elem_p->value, elem_c->value) <= 0 && lbig == false)
            lbig = true;
        else if ((strcmp(elem_p->value, elem_c->value) > 0) && lbig == true)
            lbig = false;

        // Set prev->prev
        struct list_head *tmp = prev->prev;

        // Delete prev
        if (strcmp(elem_p->value, elem_c->value) <= 0) {
            list_del(prev);
            free(elem_p->value);
            free(elem_p);
            // prev = NULL;
            count--;
        }

        // Set new prev and new curr
        if (lbig == true) {
            prev = tmp;
            if (prev == head) {
                count++;
                lbig = false;
                prev = curr;
                curr = prev->next;
            }
        } else {
            count++;
            prev = curr;
            curr = prev->next;
        }
    }
    return count;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
