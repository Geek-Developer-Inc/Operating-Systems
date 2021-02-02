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
  * LINUX/UNIX ϵͳ����ֲ� & ����ϵͳ����
  * THE LINUX PROGRAMMING INTERFACE & Operating Systems Three Easy Pieces
  *
  * Compile and link with -pthread
  * sudo gcc -Wall -g -std=c2x pthread_create_example.c -pthread
  * 
  * �̴߳�������
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
  *     @Returns     ��None
  *     @Note        ��None
  *     @Version     ��V1.2.1
  *     @Date        ��2020.08.08
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
	/*   Return Value:�ɹ�����0��ʧ�ܷ��ش����� 
	On success, pthread_create() returns 0; on error,
	it returns an error number, and the contents of *thread are undefined.*/

	assert(0 == ret);   /* ���Ժ����������ڵ��Թ����в�׽����Ĵ��� */
	/* void assert (int expression);      #include <assert.h>
	assert()  �޷���ֵ ,����ֻҪ����һ�����ʽ
	������ʽ�Ľ��Ϊ���١���assert() ���ӡ������ʧ�ܵ���Ϣ�������� abort()
	������ֹ�����ִ�У�������ʽ�Ľ��Ϊ���桱��assert() ��ʲôҲ������
	�����������ִ�С�

	*/

	ret = pthread_create(&pthread_id2, NULL, thread_enter, "pthread id2");
	assert(0 == ret);

	/* pthread_join - join with a terminated thread  */
	/* #include <pthread.h>
	   int pthread_join(pthread_t thread, void **retval);  */
	/* pthread_join���� �����ȴ��߳�ָ�����߳���ֹ��������߳�����ֹ��
	  ��pthread_join���� �������ء��߳�ָ�����̱߳����ǿ����ӵġ�
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

����һ���߳�

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);

#include<pthread.h>

thread :�߳�ID  //unsigned long
attr : �߳����� // NULL
start_routine :�߳���ں���
arg :���ݸ��߳���ں����Ĳ���

�ɹ�: 0
ʧ��:���ش����

*/

