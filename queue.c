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

    if (head && head->next != head) {
        struct list_head *node = NULL, *safe = NULL;

        list_for_each_safe(node, safe, head) {
            safe->prev = node->prev;
            safe->prev->next = safe;
            node->prev = safe;

            node->next = safe->next;
            safe->next->prev = node;
            safe->next = node;

            if (node->next->next == head) {
                break;
            } else {
                safe = node->next;
            }
        }
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head && head->next != head && head->next->next != head) {
        struct list_head *node, *safe;

        list_for_each_safe(node, safe, head) {
            list_move(node, head);
        }
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/

    if (head && head->next != head && head->next->next != head) {
        struct list_head *node, *safe, *tmp_head = head;

        int count1 = 0;

        list_for_each_safe(node, safe, head) {
            list_move(node, tmp_head);
            count1++;

            if (count1 == k) {
                int count2 = 0;
                struct list_head *test_ptr;
                list_for_each(test_ptr, safe->prev) {
                    if (test_ptr == head || count2 == k) {
                        break;
                    }
                    count2++;
                }

                if (count2 == k) {
                    tmp_head = safe->prev;
                    count1 = 0;

                } else {
                    break;
                }
            }
        }
    }
}

struct list_head *mergeTwoLists(struct list_head *list1,
                                struct list_head *list2,
                                bool descend)
{
    struct list_head *head = NULL, **ptr = &head, **node;

    for (node = NULL; list1 && list2; *node = (*node)->next) {
        element_t *list1_element_ptr = container_of(list1, element_t, list);
        element_t *list2_element_ptr = container_of(list2, element_t, list);


        if (descend) {
            node = (strcmp(list1_element_ptr->value,
                           list2_element_ptr->value) >= 0)
                       ? &list1
                       : &list2;


            if (*ptr) {
                (*node)->prev = *ptr;
                // (*ptr)->next=*node;
                ptr = &(*ptr)->next;
            }

            *ptr = *node;

        } else {
            node = (strcmp(list1_element_ptr->value,
                           list2_element_ptr->value) <= 0)
                       ? &list1
                       : &list2;



            if (*ptr) {
                (*node)->prev = *ptr;
                // (*ptr)->next=*node;
                ptr = &(*ptr)->next;
            }

            *ptr = *node;
        }
    }

    if (!list1 && list2) {
        if (*ptr) {
            list2->prev = *ptr;
            // (*ptr)->next=list2;
            ptr = &(*ptr)->next;
        }

        *ptr = list2;
    } else if (!list2 && list1) {
        if (*ptr) {
            list1->prev = *ptr;
            // (*ptr)->next=list2;
            ptr = &(*ptr)->next;
        }

        *ptr = list1;
    }

    return head;
}

struct list_head *mergesort_list(struct list_head *head, bool descend)
{
    if (!head || !head->next) {
        return head;
    }

    struct list_head *slow = head;

    struct list_head *fast = head->next;

    for (; fast && fast->next;) {
        slow = slow->next;
        fast = fast->next->next;
    }

    struct list_head *mid = slow->next;

    if (mid) {
        mid->prev = NULL;
    }

    slow->next = NULL;

    struct list_head *left = mergesort_list(head, descend),
                     *right = mergesort_list(mid, descend);


    return mergeTwoLists(left, right, descend);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (head && head->next != head && head->next->next != head) {
        struct list_head **tmp_head = NULL, *node, *merge_list;



        node = head->prev;
        tmp_head = &node;
        (*tmp_head)->next = NULL;

        node = head->next;
        tmp_head = &node;
        (*tmp_head)->prev = NULL;


        INIT_LIST_HEAD(head);


        merge_list = mergesort_list(*tmp_head, descend);

        list_for_each(node, merge_list) {
            if (!node->next) {
                break;
            }
        }

        merge_list->prev = head;
        head->next = merge_list;

        head->prev = node;
        node->next = head;
    }
}

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