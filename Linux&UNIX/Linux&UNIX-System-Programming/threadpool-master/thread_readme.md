/********************************************************************************/

Linux �Ľ��̺��߳�
    (1) ��������Դ����Ļ�����λ���߳��ǵ��ȵĻ�����λ��
    (2) ��������Դ�ļ��ϣ���Щ��Դ�����ڴ��ַ�ռ䣬�ļ��������ȵȣ�һ�������е�
����̹߳�����Щ��Դ��
    (3) CPU��������е���ʱ���ɵ��ȵĻ�����λ (dispatchable entity)���̡߳����
һ��������û�������̣߳������������������ֻ��һ�����̣߳���������̶��������
��������Դ��
    (4) ���̵ĸ��������ȫ�����ģ����̼߳��Ǳ˴����棬���ҹ�����Դ������̻����У�
�κ�һ�����̵���ֹ������Ӱ�쵽�������ӽ��̡������̻߳����У����߳���ֹ��ȫ������
�̱�����ֹ(û������Դ)��

Linux �̲߳���API
  (1) �����߳� pthread_create
    pthread_create - create a new thread.
    SYNOPSIS
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
    �� ͷ�ļ�
    #include <pthread.h>
    �� ����
    thread���߳�ID�����磺thread_t thread_id;/*ID returned by pthread_create()*/
    pthread_t���ͱ���(typedef  unsigned long int  pthread_t)����Linux��Ϊ�޷���
����(%lu)��
    attr�������̵߳����ԣ�һ������ΪNULL��ʾʹ��Ĭ�����ԡ�
    start_routine���߳���ں������Ǹ�������ַ���߳�������Ҫִ�еĺ���,����ָ�룬
ָ���߳�������(�߳���)���ú������н��������߳̽�����
    arg�������߳����������Ĳ���,�߳�������ִ���ڼ���ʹ�õĲ�������Ҫ���������, 
�����ýṹ��װ��
    �� ����ֵ(RETURN VALUE)
    �ɹ�����0��ʧ�ܷ��ش����롣
    On success, pthread_create() returns 0; on error, it returns an error number.
    �� ����
    ���벢���� -pthread��
    Compile and link with -pthread.

  (2) �߳���ֹ pthread_exit
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

  (3) �ȴ�(��)�߳̽���(��������) pthread_join
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

  (4) ��ȡ�����̵߳�ID pthread_self
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

  (5) ȡ��һ��ִ���е��� pthread_cancel
    pthread_cancel - send a cancellation request to a thread.
    SYNOPSIS
    int pthread_cancel(pthread_t thread);
    �� ͷ�ļ�
    #include <pthread.h>
    �� ����
    thread���߳�ID
    �� ����ֵ(RETURN VALUE)
    �ɹ�����0, ʧ�ܷ��ش����롣

  (6) �����߳�Ϊ����̬ pthread_detach
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