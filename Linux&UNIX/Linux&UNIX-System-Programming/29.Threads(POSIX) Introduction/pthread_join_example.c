/*
 ***********************************************************************************************************************
 *
 *                                                      PTHREAD_JOIN
 *                                         LINUX/UNIX ϵͳ����ֲ� & ����ϵͳ����
 *                        THE LINUX PROGRAMMING INTERFACE & Operating Systems Three Easy Pieces
 *
 *                                  File    : pthread_join_example.c
 *                                  By      : Ying Wang Team
 *                                  Version : V1.02.02
 *
 *                                  Compile and link with -pthread
 *                                  sudo gcc -Wall -g -std=c2x pthread_join_example.c -lpthread
 *
 *                        �̵߳ȴ�����
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

	printf("thread_id = %lu\n", thread_id);   /*   ��ӡ�߳�ID*/

	
	ret = pthread_join(thread_id, NULL);
	assert(0 == ret);

	printf("main thread end\n");

	return 0;

}

/*

Name
pthread_join - join with a terminated thread
�ȴ�(��)�߳̽���(��������),һֱ�ȵ����� pthid ָ�����̷߳���
Synopsis
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);
  thread������Ҫָ�����̵߳�id
   retval�����߳��˳���״̬ һ��ָ��NULL
Compile and link with -pthread.

Description
The pthread_join() function waits for the thread specified by thread to terminate.
If that thread has already terminated, then pthread_join() returns immediately. 
The thread specified by thread must be joinable.
If retval is not NULL, then pthread_join() copies the exit status of the target
thread (i.e., the value that the target thread supplied to pthread_exit(3)) into the 
location pointed to by *retval. If the target thread was canceled, then PTHREAD_CANCELED 
is placed in *retval.
pthread_join() �����ȴ��߳�ָ�����߳���ֹ��
������߳�����ֹ����pthread_join() �������ء�
�߳�ָ�����̱߳����ǿ����ӵġ�
��� retval ���� NULL��pthread_join() ����Ŀ����˳�״̬
�̣߳�����Ŀ���߳��ṩ��pthread_exit��3������ֵ��
λ��ָ�� *retval�����Ŀ���߳���ȡ������PTHREAD_CANCELED
������ *retval �С�
If multiple threads simultaneously try to join with the same thread, the results are 
undefined. If the thread calling pthread_join() is canceled, then the target thread will 
remain joinable (i.e., it will not be detached).
�������߳�ͬʱ������ͬһ�߳����ӣ�����Ϊ
���塣���ȡ������ pthread_join() ���̣߳���Ŀ���߳̽�
���ֿ����ӣ���������Զ������룩��

Return Value
On success, pthread_join() returns 0; on error, it returns an error number.
�ɹ�����0��ʧ�ܷ��ش����롣

*/



/*
�ȴ�ָ�����߳��˳�
int pthread_join(pthread_t thread, void **value_ptr);
�ú�����һ������������һֱ�ȵ����� pthid ָ�����̷߳��أ��������е� wait �� waitpid ����

thread :Ҫ�ȴ����߳�ID
value_ptr :�����߳��˳���״̬ һ��ָ��NULL


*/