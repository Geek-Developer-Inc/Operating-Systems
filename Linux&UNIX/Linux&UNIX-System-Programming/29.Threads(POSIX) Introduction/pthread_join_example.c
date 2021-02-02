/*
 ***********************************************************************************************************************
 *
 *                                                      PTHREAD_JOIN
 *                                         LINUX/UNIX 系统编程手册 & 操作系统导论
 *                        THE LINUX PROGRAMMING INTERFACE & Operating Systems Three Easy Pieces
 *
 *                                  File    : pthread_join_example.c
 *                                  By      : Ying Wang Team
 *                                  Version : V1.02.02
 *
 *                                  Compile and link with -pthread
 *                                  sudo gcc -Wall -g -std=c2x pthread_join_example.c -lpthread
 *
 *                        线程等待函数
 *                                  int pthread_join(pthread_t thread, void **value_ptr);
 *
 ***********************************************************************************************************************
 */

 /*     Include the header file     */
#include <stdio.h>
#include <pthread.h>/*    pthread_create()    */
#include <assert.h> /*     void assert (int expression);     */


/*     The thread entry function     */
void *thread_enter(void *arg)
{

	printf("data = %d\n", *(int *)arg);
	pthread_exit(NULL);


}


int main(int argc, char**argv)
{

	pthread_t thread_id;

	int num = 100;
	int ret = 0;

	printf("main thread begin\n");

	ret = pthread_create(&thread_id, NULL, thread_enter, (void *)&num);

	if (ret < 0)
	{
		perror("pthread create error\n");
		return -1;
	}

	printf("thread_id = %lu\n", thread_id);   /*   打印线程ID*/

	
	ret = pthread_join(thread_id, NULL);
	assert(0 == ret);

	printf("main thread end\n");

	return 0;

}

/*

Name
pthread_join - join with a terminated thread
等待(子)线程结束(阻塞函数),一直等到参数 pthid 指定的线程返回
Synopsis
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);
  thread就是你要指定的线程的id
   retval保存线程退出的状态 一般指定NULL
Compile and link with -pthread.

Description
The pthread_join() function waits for the thread specified by thread to terminate.
If that thread has already terminated, then pthread_join() returns immediately. 
The thread specified by thread must be joinable.
If retval is not NULL, then pthread_join() copies the exit status of the target
thread (i.e., the value that the target thread supplied to pthread_exit(3)) into the 
location pointed to by *retval. If the target thread was canceled, then PTHREAD_CANCELED 
is placed in *retval.
pthread_join() 函数等待线程指定的线程终止。
如果该线程已终止，则pthread_join() 立即返回。
线程指定的线程必须是可联接的。
如果 retval 不是 NULL，pthread_join() 复制目标的退出状态
线程（即，目标线程提供给pthread_exit（3））的值。
位置指向 *retval。如果目标线程已取消，则PTHREAD_CANCELED
放置在 *retval 中。
If multiple threads simultaneously try to join with the same thread, the results are 
undefined. If the thread calling pthread_join() is canceled, then the target thread will 
remain joinable (i.e., it will not be detached).
如果多个线程同时尝试与同一线程连接，则结果为
定义。如果取消调用 pthread_join() 的线程，则目标线程将
保持可联接（即，它永远不会分离）。

Return Value
On success, pthread_join() returns 0; on error, it returns an error number.
成功返回0；失败返回错误码。

*/



/*
等待指定的线程退出
int pthread_join(pthread_t thread, void **value_ptr);
该函数是一个阻塞函数，一直等到参数 pthid 指定的线程返回；与多进程中的 wait 或 waitpid 类似

thread :要等待的线程ID
value_ptr :保存线程退出的状态 一般指定NULL


*/