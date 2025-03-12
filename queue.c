#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (!new) {
        return NULL;
    }

    INIT_LIST_HEAD(new);

    return new;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head) {
        struct list_head *node = head->next;

        while (node != head) {
            element_t *element_ptr = container_of(node, element_t, list);

            list_del(node);

            q_release_element(element_ptr);

            node = head->next;
        }

        free(head);
    }
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }

    element_t *node = (element_t *) malloc(sizeof(element_t));

    if (!node) {
        return false;
    }

    node->value = (char *) malloc(strlen(s) + 1);

    if (!node->value) {
        free(node);
        return false;
    }

    strncpy(node->value, s, strlen(s) + 1);

    list_add(&node->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }

    element_t *node = (element_t *) malloc(sizeof(element_t));

    if (!node) {
        return false;
    }

    node->value = (char *) malloc(strlen(s) + 1);

    if (!node->value) {
        free(node);
        return false;
    }

    strncpy(node->value, s, strlen(s) + 1);

    list_add_tail(&node->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || head->next == head) {
        return NULL;
    }

    struct list_head *node = head->next;

    element_t *element_ptr = container_of(node, element_t, list);

    if (sp) {
        strncpy(sp, element_ptr->value, bufsize);
        *(sp + bufsize - 1) = '\0';
    }

    list_del(node);

    return element_ptr;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || head->next == head) {
        return NULL;
    }

    struct list_head *node = head->prev;

    element_t *element_ptr = container_of(node, element_t, list);

    if (sp) {
        strncpy(sp, element_ptr->value, bufsize);
        *(sp + bufsize - 1) = '\0';
    }


    list_del(node);

    return element_ptr;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head) {
        return 0;
    }

    struct list_head *node;

    int len = 0;

    list_for_each(node, head) {
        len++;
    }

    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/

    if (!head || head->next == head) {
        return false;
    }

    if (head->next->next == head) {
        element_t *element_ptr = container_of(head->next, element_t, list);
        list_del(head->next);
        if (element_ptr->value) {
            free(element_ptr->value);
        }
        free(element_ptr);

        return true;
    }

    struct list_head *slow = NULL, *fast = NULL;

    for (slow = head->next, fast = head->next->next;
         fast->next != head && fast->next->next != head;
         slow = slow->next, fast = fast->next->next)
        ;

    element_t *element_ptr = container_of(slow->next, element_t, list);
    list_del(slow->next);
    if (element_ptr->value) {
        free(element_ptr->value);
    }
    free(element_ptr);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/

    if (head && head->next != head) {
        if (head->next->next == head) {
            return true;
        } else {
            struct list_head *node, *safe;
            char *delete_str = (char *) malloc(10);
            list_for_each_safe(node, safe, head) {
                element_t *node_element_ptr =
                    container_of(node, element_t, list);
                if (safe == head) {
                    if (strcmp(node_element_ptr->value, delete_str) == 0) {
                        list_del(node);
                        if (node_element_ptr->value != NULL) {
                            free(node_element_ptr->value);
                        }
                        free(node_element_ptr);
                    }

                    break;
                } else {
                    if (strcmp(node_element_ptr->value,
                               container_of(safe, element_t, list)->value) ==
                            0 ||
                        strcmp(node_element_ptr->value, delete_str) == 0) {
                        strncpy(delete_str, node_element_ptr->value, 10);
                        list_del(node);
                        if (node_element_ptr->value) {
                            free(node_element_ptr->value);
                        }
                        free(node_element_ptr);
                    } else {
                        strncpy(delete_str, "", 10);
                    }
                }
            }

            free(delete_str);

            return true;
        }
    }

    return false;
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