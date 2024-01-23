#include "../inc/main.h"

t_node *create_empty_node(void)
{
    t_node *new;

    new = (t_node *) malloc(sizeof(t_node) * 1);
    if (!new)
        return (NULL);
    new->r = 0;
    new->g = 0;
    new->b = 0;
    new->next = NULL;
    return (new);
}

t_node *create_node_from_values(int r, int g, int b)
{
    t_node *new;

    new = create_empty_node();
    if (!new)
        return (NULL);
    new->r = r;
    new->g = g;
    new->b = b;
    return (new);
}

t_node *create_node_from_buffer(BYTE buffer[BYTESREAD])
{
    return (create_node_from_values(buffer[0], buffer[1], buffer[2]));
}

t_node *create_node_from_buffer_and_insert(BYTE buffer[BYTESREAD], t_node *insert_at)
{
    t_node *new;

    new = create_node_from_buffer(buffer);
    if (!new)
        return (NULL);
    new->next = insert_at;
    return (new);
}

void print_linked_list(t_node *root)
{
    int i;

    i = 0;
    while (root)
    {
        printf("[%i]:\n\t", i);
        printf("[b: %i]\n\t", root->b);
        printf("[g: %i]\n\t", root->g);
        printf("[r: %i]\n", root->r);
        root = root->next;
        i++;
    }
    printf("\n");
}

void free_linked_list(t_node *root)
{
    t_node *tmp;

    while (root)
    {
        tmp = root;
        root = root->next;
        free(tmp);
        tmp = NULL;
    }
}

void remove_last_node_linked_list(t_node *root)
{
    t_node *tmp;

    tmp = root;
    while (root->next)
    {
        tmp = root;
        root = root->next;
    }
    free(tmp->next);
    tmp->next = NULL;
}
