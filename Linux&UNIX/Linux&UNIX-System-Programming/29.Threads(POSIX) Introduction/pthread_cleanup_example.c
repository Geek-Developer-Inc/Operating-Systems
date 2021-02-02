/*
 ***********************************************************************************************************************
 *
 *                                                    PTHREAD_CLEANUP
 *                                         LINUX/UNIX 系统编程手册 & 操作系统导论
 *                        THE LINUX PROGRAMMING INTERFACE & Operating Systems Three Easy Pieces
 *
 *                                  File    : pthread_cleanup_example.c
 *                                  By      : Ying Wang Team
 *                                  Version : V1.02.04
 *
 *                                  Compile and link with -pthread
 *                                  sudo gcc -Wall -g -std=c2x pthread_cleanup_example.c -pthread
 *
 *                         线程清理函数
 *                                  void pthread_cleanup_pop(int execute); 是执行函数，只有里面的参数 execute=1才会执行
 *                                  void pthread_cleanup_push(void (*routine)(void*), void *arg); 是清理注册函数
 *
 ***********************************************************************************************************************
 */

#include <stdio.h>
#include <pthread.h>

void clean1(void *p)
{

	printf("clean1 %d\n", (int)p);

}
void clean2(void *p)
{

	printf("clean2 %d\n", (int)p);

}
void clean3(void *p)
{

	printf("clean3 %d\n", (int)p);

}
void *fun(void *data)
{

	int i = 1;
	pthread_cleanup_push(clean1, (void *)i++);//先注册的  最后执行
	pthread_cleanup_push(clean2, (void *)i++);
	pthread_cleanup_push(clean3, (void *)i++);
	printf("hello fun!!!\n");
	pthread_cleanup_pop(1);//决定最后注册的是否执行 1：执行   0：不执行
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(0);//决定第一个注册的是否执行  1：执行  0：不执行
	return NULL;

}
//main线程   子线程  
int main(void)
{

	int num = 100;

	pthread_t id;//unsigned long

	pthread_create(&id, NULL, fun, (void *)&num);//创建一个线程

	pthread_join(id, NULL);//等待指定的线程退出

	return 0;

}

/*

PTHREAD_CLEANUP_PUSH(3)                       Linux Programmer's Manual                       PTHREAD_CLEANUP_PUSH(3)

NAME
	   pthread_cleanup_push, pthread_cleanup_pop - push and pop thread cancellation clean-up handlers

SYNOPSIS
	   #include <pthread.h>

	   void pthread_cleanup_push(void (*routine)(void *),
								 void *arg);
	   void pthread_cleanup_pop(int execute);

	   Compile and link with -pthread.

DESCRIPTION
	   These  functions  manipulate  the calling thread's stack of thread-cancellation clean-up handlers.  A clean-up
	   handler is a function that is automatically executed when a thread is canceled (or in  various  other  circum‐
	   stances  described below); it might, for example, unlock a mutex so that it becomes available to other threads
	   in the process.

	   The pthread_cleanup_push() function pushes routine onto the top of the stack of clean-up handlers.  When  rou‐
	   tine is later invoked, it will be given arg as its argument.

	   The  pthread_cleanup_pop()  function removes the routine at the top of the stack of clean-up handlers, and op‐
	   tionally executes it if execute is nonzero.

	   A cancellation clean-up handler is popped from the stack and executed in the following circumstances:

	   1. When a thread is canceled, all of the stacked clean-up handlers are popped and executed in the  reverse  of
		  the order in which they were pushed onto the stack.

	   2. When a thread terminates by calling pthread_exit(3), all clean-up handlers are executed as described in the
		  preceding point.  (Clean-up handlers are not called if the thread terminates by performing  a  return  from
		  the thread start function.)

	   3. When a thread calls pthread_cleanup_pop() with a nonzero execute argument, the top-most clean-up handler is
		  popped and executed.

	   POSIX.1 permits pthread_cleanup_push() and pthread_cleanup_pop() to be implemented as macros  that  expand  to
	   text  containing '{' and '}', respectively.  For this reason, the caller must ensure that calls to these func‐
	   tions are paired within the same function, and at the same lexical nesting level.  (In other words, a clean-up
	   handler is established only during the execution of a specified section of code.)

	   Calling   longjmp(3)   (siglongjmp(3))   produces   undefined   results   if   any   call  has  been  made  to
	   pthread_cleanup_push() or pthread_cleanup_pop() without the matching call of the pair since  the  jump  buffer
	   was  filled  by setjmp(3) (sigsetjmp(3)).  Likewise, calling longjmp(3) (siglongjmp(3)) from inside a clean-up
	   handler produces undefined results unless the jump buffer was also filled by setjmp(3)  (sigsetjmp(3))  inside
	   the handler.

*/
