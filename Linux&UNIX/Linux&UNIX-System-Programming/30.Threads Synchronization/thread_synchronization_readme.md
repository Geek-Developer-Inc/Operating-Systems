��30�� �߳�:�߳�ͬ��
         THREADS:THREAD SYNCHRONIZATION

˵��: (1)���¶�Ӧ Linux/Unix ϵͳ����ֲ�(�ϲ�) �� 30 �� �߳�:�߳�ͬ��
         <<The Linux Programming Interface-A Linux and UNIX System Programming 
		      Handbook>> 30 Threads:Thread Synchronization 
      (2)����ϵͳ����(Operating Systems) �� 27 �� ����:�߳�API
	     <<Operating Systems: Three Easy Pieces>> 27 Interlude:Thread API

30.1 ������-�����Թ�������ķ���
     Protecting Accesses to Shared Variables:Mutexes

    Ϊ�����̸߳��¹������ʱ�����ֵ����⣬����ʹ�û�����(Mutex �� mutual exc-
lusion ����д)����ȷ��ͬʱ����һ���߳̿��Է���ĳ�����Դ��
    To avoid the problems that can occur when threads try to update a shared 
variable,we must use a mutex(short for mutual exclusion)to ensure that only 
one thread at atime can access the variable.
    
	����ʹ�û���������֤�����⹲����Դ��ԭ�ӷ��ʣ���������������ǳ����ķ�����
	Mutexes can be used to ensure atomic access to any shared resource,but pr-
otecting shared variable is the most common use.

    ������������״̬��������(locked)��δ����(unlocked)���κ�ʱ������ֻ��һ
���߳̿��������û�������
    A mutex han two states:locked and unlocked,At any moment,at most one thre-
ad may hold the lock on a mutex.

    һ���߳��������������漴��Ϊ�û������������ߡ�ֻ�������߲��ܸ�������������
	When a thread locks a mutex,it becomes the owner of that mutex.Only the
mutex owner can unlock the mutex.