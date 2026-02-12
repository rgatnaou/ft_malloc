/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgatnaou <rgatnaou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 15:16:57 by jterrazz          #+#    #+#             */
/*   Updated: 2026/02/11 16:26:30 by rgatnaou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/malloc.h"
#include <stdio.h>
// #include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define MAX_PTRS 1000
#define MAX_SIZE 4096
#define ALIGNMENT 16

int main(void)
{
    void *ptrs[MAX_PTRS] = {0};
    size_t sizes[MAX_PTRS] = {0};
    int i;
    int index;

    srand(time(NULL));

    while (1) // infinite stress loop
    {
        /* Random allocations */
        // printf("\n******start :\n");
        for (i = 0; i < MAX_PTRS; i++)
        {
            // printf("start loop: i=%d\n",i);
            sizes[i] = rand() % MAX_SIZE + 1;
            // printf("allocation size: %ld\n",sizes[i]);
            ptrs[i] = malloc(sizes[i]);

            if (!ptrs[i])
            {
                printf("malloc failed\n");
                exit(1);
            }

            /* Alignment check */
            if ((uintptr_t)ptrs[i] % ALIGNMENT != 0)
            {
                printf("❌ Alignment broken at %p\n", ptrs[i]);
                abort();
            }

            /* Fill full block (detect overflow issues) */
            memset(ptrs[i], 0xAA, sizes[i]);
        }
		printf("✅ Allocated %d blocks\n", MAX_PTRS);
        /* Random free order */
        for (i = 0; i < MAX_PTRS; i++)
        {
            index = rand() % MAX_PTRS;
            if (ptrs[index])
            {
                free(ptrs[index]);
                ptrs[index] = NULL;
            }
        }
		printf("✅ Freed blocks in random order\n");
    }
    return 0;
}
