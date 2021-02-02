/*
 ***********************************************************************************************************************
 *
 *                                                    PTHREAD_CREATE
 *                                         LINUX/UNIX 系统编程手册 & 操作系统导论
 *                        THE LINUX PROGRAMMING INTERFACE & Operating Systems Three Easy Pieces
 *
 *                                  File    : pthread_exit_example.c
 *                                  By      : Ying Wang Team
 *                                  Version : V1.02.03
 *
 *                                  Compile and link with -pthread
 *                                  sudo gcc -Wall -g -std=c2x pthread_exit_example.c -pthread
 *
 *                        线程退出函数
 *                                  void pthread_exit(void *value_ptr);
 *
 ***********************************************************************************************************************
 */

 /*     Include the header file     */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_enter(void *data)
{
	while (1)
	{
		printf("data = %d\n", *(int *)data);
		sleep(1);
	}
	pthread_exit(NULL);
}

int main(int argc, char**argv)
{
	int num = 100;
	int ret = 0;
	pthread_t thid;
	ret = pthread_create(&thid, NULL, thread_enter, (void *)&num);
	if (ret < 0)
	{
		perror("pthread create error\n");
		return -1;
	}
	printf("thid = %lu\n", thid);
	sleep(5);
	pthread_cancel(thid);
	pthread_join(thid, NULL);
	printf("=================\n");
	return 0;

}

/*

线程退出
void pthread_exit(void *value_ptr);
正常退出
非正常退出
value_ptr : 保存线程退出的状态

*/