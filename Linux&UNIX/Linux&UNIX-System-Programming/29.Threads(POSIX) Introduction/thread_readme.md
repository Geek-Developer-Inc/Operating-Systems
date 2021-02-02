��29��  �߳�(thread)

˵����(1)���¶�Ӧ Linux/Unix ϵͳ����ֲ�(�ϲ�) �� 29~33 �� �߳�
      (2)����ϵͳ����(Operating Systems) ��2���� ����

һ���߳̽���

    (1)�߳�(POSIX�߳�)���༴ pthreads��
    (2)�߳�(thread)������Ӧ�ó��򲢷�ִ�ж�������һ�ֻ��ơ�һ�����̿��԰�������̡߳�
    (3)ͬһ�����е������߳̾������ִ����ͬ�ĳ����ҹ���ͬһ��ȫ���ڴ��������а���
��ʼ�����ݶ�(initialized data)��δ��ʼ�����ݶ�(uninitialized data)���Լ����ڴ��(heap
segment)�� 
    (4)ͬһ�����еĶ���߳̿��Բ���ִ�С�
    (5)���߳�(multi-threaded)������ж��ִ�е�(��������������ÿ��������ȡֵ���ִ��)��
    (6)ÿ���߳������ڶ����Ľ��̡��̼߳乲���ַ�ռ䣬�Ӷ��ܹ�������ͬ�����ݡ�
    (7)�����̵߳�״̬����̵�״̬�ǳ����ơ�
    (8)�߳�֮����������л�(context switch)�����ڽ��̼���������л����������л�ʱ���̵�
״̬�ɽ��̿��ƿ�(Process Control Block,PCB)���棬�߳��򱣴����߳̿��ƿ�(Thread 
Control Block,TCB)��
    (9)�������ȣ��߳�֮����������л���ַ�ռ䱣�ֲ���(������Ҫ�л���ǰʹ�õ�ҳ��)����
�̺ͽ���֮�����һ����Ҫ��������ջ��
    (10)�߳���һ���������Ľ��̣�����߳�������ͬһ�����̿ռ����档
    (11)�߳̾��ǽ��̵�һ����Сִ�е�Ԫ���߳̾����������Ľ��̡���������Դ����Ļ�����λ��
�߳��ǵ��ȵĻ�����λ��
    (12)�̹߳�������ڴ�ռ��в����Ķ��ִ��·����������̵���Դ��
    (13) ��������Դ�ļ��ϣ���Щ��Դ�����ڴ��ַ�ռ䣬�ļ��������ȵȣ�һ�������е�
����̹߳�����Щ��Դ��
    (14) CPU��������е���ʱ���ɵ��ȵĻ�����λ (dispatchable entity)���̡߳����
һ��������û�������̣߳������������������ֻ��һ�����̣߳���������̶��������
��������Դ��
    (15) ���̵ĸ��������ȫ�����ģ����̼߳��Ǳ˴����棬���ҹ�����Դ������̻����У�
�κ�һ�����̵���ֹ������Ӱ�쵽�������ӽ��̡������̻߳����У����߳���ֹ��ȫ������
�̱�����ֹ(û������Դ)��

�����̺߳ͽ���

    (1)���̼����Ϣ���Թ�����Ҫ���ý��̼�ͨ��(inter-process communication,���IPC)��
ʽ��
    (2)���� fork() ���������̵Ĵ�����Ըߡ���������дʱ����(copy-on-write)��������Ȼ��Ҫ
���������ڴ�ҳ��(page table)���ļ���������(file descriptor table)֮��Ľ������ԡ�  
    (3)�߳�֮���ܹ�������ٵع�����Ϣ��ֻ�轫���ݸ��Ƶ�����(ȫ�ֻ��)�����м��ɡ�Ҫ����
���ֶ���߳���ͼͬʱ�޸�ͬһ����Ϣ�������
    (4)���е��߳�ջ��פ����ͬһ�����ַ�ռ䡣
    (5)��ҪƵ���������ٵ��������߳�,��Ϊ�Խ�����˵����������һ�����̴����Ǻܴ�ġ�
    (6)�̵߳��л��ٶȿ죬��������Ҫ�������㣬�л�Ƶ��ʱ���̣߳����к�ʱ�Ĳ���ʹ���߳�
�����Ӧ�ó������Ӧ��
    (7)��Ҫ���ȶ���ȫʱ���ʺ�ѡ����̣���Ҫ�ٶ�ʱ��ѡ���̸߳��á�

�����̲߳���API(Pthreads API)

  1.�߳���������(Pthreads data type)
        
                  ��������                         ����
               pthread_t                        �߳�ID
               pthread_mutex_t                  �������(Mutex)
               pthread_mutexattr_t              �������Զ���
               pthread_cond_t                   ��������(condition variable)
               pthread_condattr_t               �������������Զ���
               pthread_key_t                    �߳��������ݵļ�(Key)
               pthread_once_t                   һ���Գ�ʼ������������(control context)
               pthread_attr_t                   �̵߳����Զ���

  2.Linux �̲߳���API

  (1)�����߳� pthread_create()
    pthread_create - create a new thread.
    SYNOPSIS
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
    �� ͷ�ļ�
    #include <pthread.h>
    �� ����
    thread���߳�ID�����磺thread_t thread_id;/*ID returned by pthread_create()*/
��ӡ�߳�ID printf("thread ID = %ld\n",(unsigned long)thread_id);	
    pthread_t���ͱ���(typedef  unsigned long int  pthread_t)����Linux��Ϊ�޷���
����(%lu)��
    attr���������̵߳����ԣ�һ������ΪNULL��ʾʹ��Ĭ�����ԡ�
    start_routine���߳���ں������Ǹ�������ַ���߳�������Ҫִ�еĺ���,����ָ�룬
ָ���߳�������(�߳���)���ú������н��������߳̽�����
    arg�������߳����������Ĳ���,�߳�������ִ���ڼ���ʹ�õĲ�������Ҫ���������, 
�����ýṹ��װ���� arg ����Ϊ void* ���ͣ���ζ�ſ��Խ�ָ����������ָ�����Ͳ���
���ݸ� start_routine() ������
    �� ����ֵ(RETURN VALUE)
    �ɹ�����0��ʧ�ܷ��ش����롣
    On success, pthread_create() returns 0; on error, it returns an error number.
    �� ����
    ���벢���� -pthread��
    Compile and link with -pthread.

  (2) �߳���ֹ pthread_exit()
    pthread_exit - terminate calling thread.
    SYNOPSIS
    void pthread_exit(void *retval);
    �� ͷ�ļ�
    #include <pthread.h>
    �� ����
    retval��ָ����̵߳ķ���ֵ��pthread_exit()������ֹ�����̲߳�ͨ��retval����һ��
ֵ��ע�⣺retval ����ָ��һ���ֲ�������
    �� ����ֵ(RETURN VALUE)
    �޷���ֵ��
    �� ����
    ���벢���� -pthread��
    Compile and link with -pthread.
	pthread_exit() ��������ֹ�����̣߳����䷵��ֵ������һ�߳�ͨ������ pthread_join()
����ȡ������ pthread_exit() �൱�������̵߳�ִ�к�����ִ�� return����֮ͬ�����ڿ���
�߳� start_routine() ���������õ����⺯���е���	pthread_exit()��

  (3) �ȴ�(��)�߳̽���(��������) pthread_join()
    pthread_join - join with a terminated thread.
    SYNOPSIS
    int pthread_join(pthread_t thread, void **retval);
    �� ͷ�ļ�
    #include <pthread.h>
    �� ����
    thread���̱߳�ʶ���������߳�id��Ψһ�ı�ʶ�̡߳�
    retval���û������ָ�룬�����洢���ȴ����̵߳ķ���ֵ��
    �� ����ֵ(RETURN VALUE)
    �ɹ�����0��ʧ�ܷ��ش����롣
	�� ����
	pthread_join() �����ȴ��� thread ��ʶ���߳���ֹ������߳��Ѿ���ֹ���ú������������ء�
pthread_join() �����Ĳ�������Ϊ����(joining)������� pthread_join() ��������һ���Ѿ���
���ӹ����߳�ID���������޷�Ԥ֪����Ϊ�� 
	
  (4) ��ȡ�����̵߳�ID pthread_self()
    pthread_self - obtain ID of the calling thread.
    SYNOPSIS
    pthread_t pthread_self(void);
    �� ͷ�ļ�
    #include <pthread.h>
    �� ����ֵ(RETURN VALUE)
    �˺���ʼ�ճɹ������ص����̵߳�ID,�߳�ID�ǽ����ڲ���ʶ���־,��ͬ���̼䣬
�߳�ID������ͬ��
    his function always succeeds, returning the calling thread's ID.
    �� ����
    ��Ӧʹ��ȫ�ֱ��� pthread_t tid�������߳���ͨ��pthread_create����������
��ȡ�߳�ID����Ӧʹ�� pthread_self��

  (5) ȡ��һ��ִ���е��� pthread_cancel()
    pthread_cancel - send a cancellation request to a thread.
    SYNOPSIS
    int pthread_cancel(pthread_t thread);
    �� ͷ�ļ�
    #include <pthread.h>
    �� ����
    thread���߳�ID
    �� ����ֵ(RETURN VALUE)
    �ɹ�����0, ʧ�ܷ��ش����롣

  (6) �̷߳��� pthread_detach()
    pthread_detach - detach a thread.
    SYNOPSIS
    int pthread_detach(pthread_t thread);
    �� ����
    thread�������߳�ID��
    �� ����
    �������̱߳�ʶ���̱߳��Ϊ�ѷ��롣��������߳���ֹʱ������Դ���Զ��ͷŻ�ϵͳ��
��������һ���߳�����ֹ���߳����ӡ� ���Է����Ѿ�������̻߳ᵼ��δָ������Ϊ��
    The pthread_detach() function marks the thread identified by thread as  
detached.When a detached thread terminates, its resources are automatically 
released back to the system without the need for another thread to join with 
the terminated thread.Attempting to detach an already detached thread results in 
unspecified behavior.
    Ĭ������£��߳��ǿ����ӵ�(joinable)�����߳��˳�ʱ�������߳̿���ͨ�� ����
pthread_join() ������ȡ�䷵��״̬��pthread_detach() ֻ�ǿ����߳���ֹ֮��������
�����飬���Ǻ�ʱ�������ֹ�̡߳�	

  (7) �Ƚ������߳�(ID)�Ƿ���� pthread_equal
    pthread_equal - compare thread IDs.
    SYNOPSIS
    int pthread_equal(pthread_t t1, pthread_t t2);
    �� ����
    t1��Ҫ�Ƚϵ��߳�1���߳�ID��
    t1��Ҫ�Ƚϵ��߳�2���߳�ID��
    �� ����ֵ(RETURN VALUE)
    ��������߳�ID��ȣ���pthread_equal() ���ط���ֵ������ȣ�����0��
    If the two thread IDs are equal, pthread_equal() returns a nonzero value; 
otherwise, it returns 0.
    �� ����
    �˺���ʼ�ճɹ�
    This function always succeeds
    pthread_equal() �����Ƚ������̱߳�ʶ����
    The pthread_equal() function compares two thread identifiers.
	
  (8) �߳����� pthread_equal
    ��ʱ��ϣ���߳��˳�ʱ�ܹ��Զ���ִ��ĳЩ������
    void pthread_cleanup_pop(int execute);
��ִ�к�����ֻ������Ĳ��� execute = 1 �Ż�ִ�С�
    void pthread_cleanup_push(void (*routine)(void*), void *arg);
������ע�ắ����
    �� ����
    routine��ע������ĺ�����ָ�롣
    arg�����ݸ��������Ĳ�����
    execute����������������Ƿ񱻵��á�
	�� ����
	����������߳��������ᱻ���ã�
    �̻߳�δִ�� pthread_cleanup_pop ǰ���� pthread_cancel ȡ����
    �̻߳�δִ�� pthread_cleanup_pop ǰ������ִ�� pthread_exit ��ֹ��
    �߳�ִ�� pthread_cleanup_pop���� pthread_cleanup_pop �Ĳ�����Ϊ 0��
	
	