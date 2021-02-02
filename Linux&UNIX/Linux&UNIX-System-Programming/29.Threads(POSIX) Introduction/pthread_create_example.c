/**
  ******************************************************************************
  *
  *     @File          : pthread_create_example.c
  *     @Brief         : pthread_create - create a new thread
  *     @Attention     : Compile and link with -pthread
  *     @Version       : V1.2.1
  *     @Date          : 2020.08.08
  *
  ******************************************************************************
  *
  * @attention
  *
  * PTHREAD_CREATE
  * LINUX/UNIX 系统编程手册 & 操作系统导论
  * THE LINUX PROGRAMMING INTERFACE & Operating Systems Three Easy Pieces
  *
  * Compile and link with -pthread
  * sudo gcc -Wall -g -std=c2x pthread_create_example.c -pthread
  * 
  * 线程创建函数
  * pthread_create - create a new thread
  * int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
  *                        void *(*start_routine) (void *), void *arg);
  *
  ******************************************************************************
  */


/************************|  Include Header File Area  |************************/

#include <stdio.h>
#include <assert.h>                 /*  void assert (int expression);     */
#include <pthread.h>                /*  pthread_create()                  */

/************************|    Header File Area End    |************************/


/************************|  Function Definition Area  |************************/

/**
  ******************************************************************************
  *
  *     @Function    : void *thread_enter(void *arg)
  *     @Description : The thread entry function
  *     @Arguments   : Parameters passed to the new thread
  *     @Returns     : Return pointer of null type
  *     @Note        : None
  *     @Version     : V1.2.1
  *     @Date        : 2020.08.08
  *
  ******************************************************************************
  */

void *thread_enter(void *arg)
{

	printf("new thread enter thread: %s\n", (char*)arg);
	return NULL;

}


/**
  ******************************************************************************
  *
  *     @Function    : int main(int argc, char**argv)
  *     @Description : Main thread
  *     @Arguments   : None
  *     @Returns     ；None
  *     @Note        ：None
  *     @Version     ：V1.2.1
  *     @Date        ：2020.08.08
  *
  ******************************************************************************
  *
  * @note(s)
  *
  * int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
  *                              void *(*start_routine) (void *), void *arg);
  * thread : thread ID
  *
  *
  *
  *
  ******************************************************************************
  */
int main(int argc, char**argv)
{

	pthread_t pthread_id1;
	pthread_t pthread_id2;

	int ret = 0;

	printf("main thread begin\n");
	
	/*     pthread_create - create a new thread     */
	/*     #include <pthread.h>     */
	/*     int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine) (void *), void *arg);     */
	ret = pthread_create(&pthread_id1, NULL, thread_enter, "pthread id1");
	/*   Return Value:成功返回0，失败返回错误码 
	On success, pthread_create() returns 0; on error,
	it returns an error number, and the contents of *thread are undefined.*/

	assert(0 == ret);   /* 断言函数，用于在调试过程中捕捉程序的错误。 */
	/* void assert (int expression);      #include <assert.h>
	assert()  无返回值 ,我们只要传入一个表达式
	如果表达式的结果为“假”，assert() 会打印出断言失败的信息，并调用 abort()
	函数终止程序的执行；如果表达式的结果为“真”，assert() 就什么也不做，
	程序继续往后执行。

	*/

	ret = pthread_create(&pthread_id2, NULL, thread_enter, "pthread id2");
	assert(0 == ret);

	/* pthread_join - join with a terminated thread  */
	/* #include <pthread.h>
	   int pthread_join(pthread_t thread, void **retval);  */
	/* pthread_join（） 函数等待线程指定的线程终止。如果该线程已终止，
	  则pthread_join（） 立即返回。线程指定的线程必须是可联接的。
	  The pthread_join() function waits for the thread specified by 
	  thread to terminate. If that thread has already terminated, then pthread_join() 
	  returns immediately. The thread specified by thread must be joinable.
	*/
	/*  join waits for the threads to finish  */
	ret = pthread_join(pthread_id1, NULL);
	assert(0 == ret);

	ret = pthread_join(pthread_id2, NULL);
	assert(0 == ret);

	printf("main thread end\n");

	return 0;

}

/*

创建一个线程

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);

#include<pthread.h>

thread :线程ID  //unsigned long
attr : 线程属性 // NULL
start_routine :线程入口函数
arg :传递给线程入口函数的参数

成功: 0
失败:返回错误号

*/

