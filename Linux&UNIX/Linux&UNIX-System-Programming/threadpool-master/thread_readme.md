/********************************************************************************/

Linux 的进程和线程
    (1) 进程是资源分配的基本单位，线程是调度的基本单位。
    (2) 进程是资源的集合，这些资源包括内存地址空间，文件描述符等等，一个进程中的
多个线程共享这些资源。
    (3) CPU对任务进行调度时，可调度的基本单位 (dispatchable entity)是线程。如果
一个进程中没有其他线程，可以理解成这个进程中只有一个主线程，这个主进程独享进程中
的所有资源。
    (4) 进程的个体间是完全独立的，而线程间是彼此依存，并且共享资源。多进程环境中，
任何一个进程的终止，不会影响到其他非子进程。而多线程环境中，父线程终止，全部子线
程被迫终止(没有了资源)。

Linux 线程操作API
  (1) 创建线程 pthread_create
    pthread_create - create a new thread.
    SYNOPSIS
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
    ① 头文件
    #include <pthread.h>
    ② 参数
    thread：线程ID。例如：thread_t thread_id;/*ID returned by pthread_create()*/
    pthread_t类型本质(typedef  unsigned long int  pthread_t)：在Linux下为无符号
整数(%lu)。
    attr：设置线程的属性，一般设置为NULL表示使用默认属性。
    start_routine：线程入口函数。是个函数地址，线程启动后要执行的函数,函数指针，
指向线程主函数(线程体)，该函数运行结束，则线程结束。
    arg：传给线程启动函数的参数,线程主函数执行期间所使用的参数，如要传多个参数, 
可以用结构封装。
    ③ 返回值(RETURN VALUE)
    成功返回0；失败返回错误码。
    On success, pthread_create() returns 0; on error, it returns an error number.
    ④ 其它
    编译并链接 -pthread。
    Compile and link with -pthread.

  (2) 线程终止 pthread_exit
    pthread_exit - terminate calling thread.
    SYNOPSIS
    void pthread_exit(void *retval);
    ① 头文件
    #include <pthread.h>
    ② 参数
    retval：指向该线程的返回值。pthread_exit()函数终止调用线程并通过retval返回一个
值。注意：retval 不能指向一个局部变量。
    ③ 返回值(RETURN VALUE)
    无返回值。
    ④ 其它
    编译并链接 -pthread。
    Compile and link with -pthread.

  (3) 等待(子)线程结束(阻塞函数) pthread_join
    pthread_join - join with a terminated thread.
    SYNOPSIS
    int pthread_join(pthread_t thread, void **retval);
    ① 头文件
    #include <pthread.h>
    ② 参数
    thread：线程标识符，就是线程id，唯一的标识线程。
    retval：用户定义的指针，用来存储被等待的线程的返回值。
    ③ 返回值(RETURN VALUE)
    成功返回0；失败返回错误码。

  (4) 获取调用线程的ID pthread_self
    pthread_self - obtain ID of the calling thread.
    SYNOPSIS
    pthread_t pthread_self(void);
    ① 头文件
    #include <pthread.h>
    ② 返回值(RETURN VALUE)
    此函数始终成功，返回调用线程的ID,线程ID是进程内部，识别标志,不同进程间，
线程ID允许相同。
    his function always succeeds, returning the calling thread's ID.
    ③ 其它
    不应使用全局变量 pthread_t tid，在子线程中通过pthread_create传出参数来
获取线程ID，而应使用 pthread_self。

  (5) 取消一个执行中的线 pthread_cancel
    pthread_cancel - send a cancellation request to a thread.
    SYNOPSIS
    int pthread_cancel(pthread_t thread);
    ① 头文件
    #include <pthread.h>
    ② 参数
    thread：线程ID
    ③ 返回值(RETURN VALUE)
    成功返回0, 失败返回错误码。

  (6) 设置线程为分离态 pthread_detach
    pthread_detach - detach a thread.
    SYNOPSIS
    int pthread_detach(pthread_t thread);
    ① 参数
    thread：分离线程ID。
    ② 其它
    函数将线程标识的线程标记为已分离。当分离的线程终止时，其资源会自动释放回系统，
而无需另一个线程与终止的线程联接。 尝试分离已经分离的线程会导致未指定的行为。
     The pthread_detach() function marks the thread identified by thread as  
detached.When a detached thread terminates, its resources are automatically 
released back to the system without the need for another thread to join with 
the terminated thread.Attempting to detach an already detached thread results in 
unspecified behavior.

  (7) 比较两个线程(ID)是否相等 pthread_equal
    pthread_equal - compare thread IDs.
    SYNOPSIS
    int pthread_equal(pthread_t t1, pthread_t t2);
    ① 参数
    t1：要比较的线程1的线程ID。
    t1：要比较的线程2的线程ID。
    ② 返回值(RETURN VALUE)
    如果两个线程ID相等，则pthread_equal() 返回非零值；不相等，返回0。
    If the two thread IDs are equal, pthread_equal() returns a nonzero value; 
otherwise, it returns 0.
    ③ 其它
    此函数始终成功
    This function always succeeds
    pthread_equal() 函数比较两个线程标识符。
    The pthread_equal() function compares two thread identifiers.