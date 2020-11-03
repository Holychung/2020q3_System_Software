#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

typedef struct __node {
    int value;
    struct __node *next;
} node_t;

void add_entry(node_t **head, int new_value)
{
    node_t **indirect = head;

    node_t *new_node = malloc(sizeof(node_t));
    new_node->value = new_value;
    new_node->next = NULL;

    assert(new_node);
    while (*indirect)
        indirect = &(*indirect)->next;
    *indirect = new_node;
}

node_t *find_entry(node_t *head, int value)
{
    node_t *current = head;
    for (; current && current->value != value; current = current->next)
        /* interate */;
    return current;
}

void remove_entry(node_t **head, node_t *entry)
{
    node_t **indirect = head;

    while ((*indirect) != entry)
        indirect = &(*indirect)->next;

    *indirect = entry->next;
    free(entry);
}

// node_t *swap_pair(node_t *head)
// {
//     for (node_t **node = &head; *node && (*node)->next; node = &(*node)->next->next) {
//         node_t *tmp = *node;
//         *node = (*node)->next;
//         tmp->next = (*node)->next;
//         (*node)->next = tmp;
//     }
//     return head;
// }

void swap_pair(node_t **head)
{
    for (node_t **node = head; *node && (*node)->next; node = &(*node)->next->next) {
        node_t *tmp = *node;
        *node = (*node)->next;
        tmp->next = (*node)->next;
        (*node)->next = tmp;
    }
}

// node_t *reverse(node_t *head)
// {
//     node_t *cursor = NULL;
//     while (head) {
//         node_t *next = head->next;
//         head->next = cursor; cursor = head;
//         head = next;
//     }
//     return cursor;
// }

// void reverse(node_t **head)
// {
//     node_t *cursor = NULL;
//     while (*head) {
//         node_t *next = (*head)->next;
//         (*head)->next = cursor; cursor = *head;
//         *head = next;
//     }
//     *head = cursor;
// }

void rev_recurive(node_t **head, node_t *cursor)
{
    if (*head == NULL) {
        *head = cursor;
        return;
    }

    node_t *next = (*head)->next;
    (*head)->next = cursor; cursor = *head;
    *head = next;
    rev_recurive(head, cursor);
}

void reverse(node_t **head)
{
    rev_recurive(head, NULL);
}

void shuffle(node_t **head)
{
    if (*head == NULL)
        return;

    srand(time(NULL));
    int len = 0;
    node_t **indirect = head;
    
    while (*indirect) {
        len++;
        indirect = &(*indirect)->next;
    }
    
    node_t *new_head = NULL;
    for (int i = len; i > 0; i--) {
        int random = rand() % i;
        indirect = head;
        while (random--)
            indirect = &(*indirect)->next;
        node_t *target = *indirect;
        *indirect = (*indirect)->next;
        target->next = NULL;
        if (new_head) {
            target->next = new_head;
            new_head = target;
        } else {
            new_head = target;
        }
    }
    *head = new_head;
}

void print_list(node_t *head)
{
    for (node_t *current = head; current; current = current->next)
        printf("%d ", current->value);
    printf("\n");
}

int main(int argc, char const *argv[])
{
    node_t *head = NULL;

    print_list(head);

    add_entry(&head, 72);
    add_entry(&head, 101);
    add_entry(&head, 108);
    add_entry(&head, 109);
    add_entry(&head, 110);
    add_entry(&head, 111);

    print_list(head);

    node_t *entry = find_entry(head, 101);
    remove_entry(&head, entry);

    entry = find_entry(head, 111);
    remove_entry(&head, entry);

    print_list(head);

    /* swap pair.
     * See https://leetcode.com/problems/swap-nodes-in-pairs/
     */
    // head = swap_pair(head);
    swap_pair(&head);
    print_list(head);

    // head = reverse(head);
    reverse(&head);
    print_list(head);

    shuffle(&head);
    print_list(head);
    shuffle(&head);
    print_list(head);

    return 0;
}