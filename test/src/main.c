/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jterrazz <jterrazz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 11:28:38 by jterrazz          #+#    #+#             */
/*   Updated: 2019/07/22 12:11:36 by jterrazz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "./test.h"

// static void *run_test_thread() {
// 	void *ret = malloc(80);
// 	pthread_exit(NULL);
// 	return ret;
// }

int main(void) {
	// pthread_t *thread = malloc(sizeof(pthread_t) * 100);

	// run_test_malloc();
	// run_test_realloc();
	// run_test_mixed();

	// int i = 0;
	// while (i < 100) {
	// 	if (pthread_create(thread + i, NULL, run_test_thread, NULL)) {
	// 		printf("Error pthread");
	// 		return 1;
	// 	}
	// 	if (pthread_join(thread[i], NULL)) {
	// 		printf("error pthread join");
	// 		return 1;
	// 	}
	// 	i++;
	// }
	run_test_rlimit(); // Always at the end
	// show_alloc_mem_ex();
}
