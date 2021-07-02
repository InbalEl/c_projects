/******************************************************************************
* Project name:					 	dhcp
* Developer: 						Inbal Elmalech
* Project Lauch: 					Aug 13, 2020
* Submitted for review:				20/08/2020
* Reviewer:
******************************************************************************/
/*
* TODO:
*   - Convert CreateIllegatAdd to recursive
*   - Reuse Reuse Reuse
*   - Declare variables only within the block in wich they are relevant
*   - Write Release Address
*   - Fix byte mirroring and make sure the correct addresses are printed out
*/


/**********************   PREPROCESSOR DIRECTIVES   **************************/

#include <stdio.h> /* TODO: REMOVE */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <arpa/inet.h> /* inet_pton */
#include <limits.h> /* CHAR_BIT */
#include "dhcp.h" /* dhcp_t */

/******************************   STRUCTURES   *******************************/

typedef struct node node_t;

struct dhcp
{
    node_t *root;
    uint32_t net_add;
    unsigned int free_bits;
};

struct node
{
    node_t *child[2];
    char level;
    char is_full[2];
};

typedef enum children_indices
{
    LEFT = 0,       /* DO NOT CHANGE */
    RIGHT = 1       /* DO NOT CHANGE */
}child_i_t;

typedef enum bool
{
    FALSE = 0,
    TRUE = 1
}bool_t;

/****************************   DECLARATIONS   *******************************/

static int CreateIllgalAdd(dhcp_t *dhcp, node_t *root, int max_level,
                           uint32_t add);
static node_t InitChild(int new_node_level);
static void ByteMirror(void *data, size_t nof_bytes);
static void R_DhcpDestroy(node_t *subtree);
static int R_GetAddress(node_t *subtree, uint32_t *ip_add, int free_bits);
static node_t *CreateChild(int child_level);
static void R_ReleaseAddress(node_t *subtree, uint32_t ip_address, int levels);

/*************************    General Functions    ***************************/

dhcp_t *DhcpCreate(const char *net_address, unsigned int mask_bits_size)
{
    int res = 0;
    uint32_t net_add = 0;

    dhcp_t *dhcp = NULL;
    node_t *root = NULL;

    int max_level = 0;

    assert(net_address);

    dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));

    if (!dhcp)
    {
        return (dhcp);
    }
    
    root = (node_t *)malloc(sizeof(node_t));
    
    if (!root)
    {
        free(dhcp);
        dhcp = NULL;

        return (dhcp);
    }

    inet_pton(AF_INET ,net_address, &net_add);
    ByteMirror(&net_add, sizeof(uint32_t));

    max_level = (sizeof(uint32_t) * CHAR_BIT) - (mask_bits_size);

    dhcp->free_bits = max_level;
    dhcp->net_add = net_add;
    dhcp->root = root;

    root->child[LEFT] = NULL;
    root->child[RIGHT] = NULL;
    root->is_full[LEFT] = FALSE;
    root->is_full[RIGHT] = FALSE;
    root->level = 0;

    res = CreateIllgalAdd(dhcp, root, max_level, net_add);

    if (res)
    {
        return (NULL);
    }

    return (dhcp);
}

/*---------------------------------------------------------------------------*/

void DhcpDestroy(dhcp_t *dhcp)
{
    assert(dhcp);

    R_DhcpDestroy(dhcp->root);

    free(dhcp);
    dhcp = NULL;
}

/*---------------------------------------------------------------------------*/

int DhcpGetAddress(dhcp_t *dhcp, uint32_t *ip_address)
{
    assert(dhcp);
    assert(ip_address);

    *ip_address = dhcp->net_add >> dhcp->free_bits;

    return (R_GetAddress(dhcp->root, ip_address, dhcp->free_bits));
}

/*---------------------------------------------------------------------------*/

void DhcpReleaseAddress(dhcp_t *dhcp, uint32_t ip_address)
{
    assert(dhcp);

    R_ReleaseAddress(dhcp->root, ip_address, dhcp->free_bits);
}

/***************************** Utilty Functions ******************************/

static int CreateIllgalAdd(dhcp_t *dhcp, node_t *root, int max_level,
                             uint32_t add)
{
    node_t *curr_l_node = root;
    node_t *curr_r_node = root;
    int level = 1;

    while (0 != max_level)
    {
        curr_l_node->child[LEFT] = CreateChild(level);

        if (!curr_l_node->child[LEFT])
        {
            DhcpDestroy(dhcp);
            return (EXIT_FAILURE);
        }

        curr_r_node->child[RIGHT] = CreateChild(level);

        if (!curr_r_node->child[RIGHT])
        {
            DhcpDestroy(dhcp);

            return (EXIT_FAILURE);
        }

        if (max_level == (1))
        {
            curr_r_node->child[LEFT] = CreateChild(level);

            if (!curr_r_node->child[LEFT])
            {
                DhcpDestroy(dhcp);

                return (EXIT_FAILURE);
            }

            /* is_full */
            curr_r_node->is_full[LEFT] = TRUE;
            curr_r_node->is_full[RIGHT] = TRUE;
            curr_l_node->is_full[LEFT] = TRUE;
        }

        curr_l_node = curr_l_node->child[LEFT];
        curr_r_node = curr_r_node->child[RIGHT];
        --max_level;
        ++level;
    }

    return (EXIT_SUCCESS);
}

/*---------------------------------------------------------------------------*/

static node_t InitChild(int new_node_level)
{
    node_t new_node = {{NULL, NULL}, {FALSE, FALSE}, 0};
    new_node.level = new_node_level;

    return (new_node);
}

/*---------------------------------------------------------------------------*/

static void ByteMirror(void *data, size_t nof_bytes)
{
    printf("hi I'm a stub\n");
    /* TODO: stub */
}

/*---------------------------------------------------------------------------*/

static void R_DhcpDestroy(node_t *subtree)
{
    if (NULL == subtree)
    {
        return;
    }
    
    R_DhcpDestroy(subtree->child[LEFT]);
    R_DhcpDestroy(subtree->child[RIGHT]);

    free(subtree);
    subtree = NULL;
}

/*---------------------------------------------------------------------------*/

static int R_GetAddress(node_t *subtree, uint32_t *ip_add, int free_bits)
{
    int res = 1;

    if (0 == free_bits)
    {
        subtree->is_full[LEFT] = TRUE;
        subtree->is_full[RIGHT] = TRUE;
        return (EXIT_SUCCESS);
    }

    else if (!(subtree->is_full[LEFT]))
    {
        if (NULL == subtree->child[LEFT])
        {
            subtree->child[LEFT] = CreateChild(subtree->level + 1);
            if (NULL == (subtree->child[LEFT]))
            {
                return (EXIT_FAILURE);
            }
        }

        *ip_add <<= 1;

        res = (R_GetAddress(subtree->child[LEFT], ip_add, free_bits - 1));

        if ((subtree->child[LEFT]->is_full[LEFT]) &&
            (subtree->child[LEFT]->is_full[RIGHT]))
        {
            subtree->is_full[LEFT] = TRUE;
        }
        
    }

    else if (!(subtree->is_full[RIGHT]))
    {
        if (NULL == subtree->child[RIGHT])
        {
            subtree->child[RIGHT] = CreateChild(subtree->level + 1);
            if (NULL == (subtree->child[RIGHT]))
            {
                return (EXIT_FAILURE);
            }       
        }

        *ip_add = (*ip_add << 1) | 1;

        res = (R_GetAddress(subtree->child[RIGHT], ip_add, free_bits - 1));

        if ((subtree->child[1]->is_full[LEFT]) &&
            (subtree->child[1]->is_full[RIGHT]))
        {
            subtree->is_full[RIGHT] = TRUE;
        }
    }

    return (res);   
}

/*---------------------------------------------------------------------------*/

static node_t *CreateChild(int child_level)
{
    node_t *new_child = (node_t *)malloc(sizeof(node_t));

    if (!new_child)
    {
        return (new_child);
    }

    *new_child = InitChild(child_level);

    return (new_child);
}

/*---------------------------------------------------------------------------*/

static void R_ReleaseAddress(node_t *subtree, uint32_t ip_address, int levels)
{

}

/*****************************************************************************/