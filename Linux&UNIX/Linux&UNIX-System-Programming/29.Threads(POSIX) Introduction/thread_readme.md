第29章  线程(thread)

说明：(1)本章对应 Linux/Unix 系统编程手册(上册) 第 29~33 章 线程
      (2)操作系统导论(Operating Systems) 第2部分 并发

一、线程介绍

    (1)线程(POSIX线程)，亦即 pthreads。
    (2)线程(thread)是允许应用程序并发执行多个任务的一种机制。一个进程可以包含多个线程。
    (3)同一程序中的所有线程均会独立执行相同的程序，且共享同一份全局内存区域，其中包括
初始化数据段(initialized data)、未初始化数据段(uninitialized data)，以及堆内存段(heap
segment)。 
    (4)同一进程中的多个线程可以并发执行。
    (5)多线程(multi-threaded)程序会有多个执行点(多个程序计数器，每个都用于取值令和执行)。
    (6)每个线程类似于独立的进程。线程间共享地址空间，从而能够访问相同的数据。
    (7)单个线程的状态与进程的状态非常类似。
    (8)线程之间的上下文切换(context switch)类似于进程间的上下文切换。上下文切换时进程的
状态由进程控制块(Process Control Block,PCB)保存，线程则保存在线程控制块(Thread 
Control Block,TCB)，
    (9)与进程相比，线程之间的上下文切换地址空间保持不变(即不需要切换当前使用的页表)。线
程和进程之间的另一个主要区别在于栈。
    (10)线程是一个轻量级的进程，多个线程运行在同一个进程空间里面。
    (11)线程就是进程的一个最小执行单元。线程就是轻量级的进程。进程是资源分配的基本单位，
线程是调度的基本单位。
    (12)线程共享进程内存空间中并发的多道执行路径，共享进程的资源。
    (13) 进程是资源的集合，这些资源包括内存地址空间，文件描述符等等，一个进程中的
多个线程共享这些资源。
    (14) CPU对任务进行调度时，可调度的基本单位 (dispatchable entity)是线程。如果
一个进程中没有其他线程，可以理解成这个进程中只有一个主线程，这个主进程独享进程中
的所有资源。
    (15) 进程的个体间是完全独立的，而线程间是彼此依存，并且共享资源。多进程环境中，
任何一个进程的终止，不会影响到其他非子进程。而多线程环境中，父线程终止，全部子线
程被迫终止(没有了资源)。

二、线程和进程

    (1)进程间的信息难以共享。需要采用进程间通信(inter-process communication,简称IPC)方
式。
    (2)调用 fork() 来创建进程的代价相对高。即便利用写时复制(copy-on-write)技术，任然需要
复制诸如内存页表(page table)和文件描述符表(file descriptor table)之类的进程属性。  
    (3)线程之间能够方便快速地共享信息。只需将数据复制到共享(全局或堆)变量中即可。要避免
出现多个线程试图同时修改同一份信息的情况。
    (4)所有的线程栈均驻留于同一虚拟地址空间。
    (5)需要频繁创建销毁的优先用线程,因为对进程来说创建和销毁一个进程代价是很大的。
    (6)线程的切换速度快，所以在需要大量计算，切换频繁时用线程，还有耗时的操作使用线程
可提高应用程序的响应。
    (7)需要更稳定安全时，适合选择进程；需要速度时，选择线程更好。

三、线程操作API(Pthreads API)

  1.线程数据类型(Pthreads data type)
        
                  数据类型                         描述
               pthread_t                        线程ID
               pthread_mutex_t                  互斥对象(Mutex)
               pthread_mutexattr_t              互斥属性对象
               pthread_cond_t                   条件变量(condition variable)
               pthread_condattr_t               条件变量的属性对象
               pthread_key_t                    线程特有数据的键(Key)
               pthread_once_t                   一次性初始化控制上下文(control context)
               pthread_attr_t                   线程的属性对象

  2.Linux 线程操作API

  (1)创建线程 pthread_create()
    pthread_create - create a new thread.
    SYNOPSIS
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
    ① 头文件
    #include <pthread.h>
    ② 参数
    thread：线程ID。例如：thread_t thread_id;/*ID returned by pthread_create()*/
打印线程ID printf("thread ID = %ld\n",(unsigned long)thread_id);	
    pthread_t类型本质(typedef  unsigned long int  pthread_t)：在Linux下为无符号
整数(%lu)。
    attr：设置新线程的属性，一般设置为NULL表示使用默认属性。
    start_routine：线程入口函数。是个函数地址，线程启动后要执行的函数,函数指针，
指向线程主函数(线程体)，该函数运行结束，则线程结束。
    arg：传给线程启动函数的参数,线程主函数执行期间所使用的参数，如要传多个参数, 
可以用结构封装。将 arg 声明为 void* 类型，意味着可以将指向任意对象的指针类型参数
传递给 start_routine() 函数。
    ③ 返回值(RETURN VALUE)
    成功返回0；失败返回错误码。
    On success, pthread_create() returns 0; on error, it returns an error number.
    ④ 其它
    编译并链接 -pthread。
    Compile and link with -pthread.

  (2) 线程终止 pthread_exit()
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
	pthread_exit() 函数将终止调用线程，且其返回值可由另一线程通过调用 pthread_join()
来获取。调用 pthread_exit() 相当于在新线程的执行函数中执行 return，不同之处在于可在
线程 start_routine() 函数所调用的任意函数中调用	pthread_exit()。

  (3) 等待(子)线程结束(阻塞函数) pthread_join()
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
	④ 其它
	pthread_join() 函数等待由 thread 标识的线程终止。如果线程已经终止，该函数会立即返回。
pthread_join() 函数的操作被称为连接(joining)。如果向 pthread_join() 函数传入一个已经被
连接过的线程ID，将导致无法预知的行为。 
	
  (4) 获取调用线程的ID pthread_self()
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

  (5) 取消一个执行中的线 pthread_cancel()
    pthread_cancel - send a cancellation request to a thread.
    SYNOPSIS
    int pthread_cancel(pthread_t thread);
    ① 头文件
    #include <pthread.h>
    ② 参数
    thread：线程ID
    ③ 返回值(RETURN VALUE)
    成功返回0, 失败返回错误码。

  (6) 线程分离 pthread_detach()
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
    默认情况下，线程是可连接的(joinable)，当线程退出时，其它线程可以通过 调用
pthread_join() 函数获取其返回状态。pthread_detach() 只是控制线程终止之后所发生
的事情，而非何时或如何终止线程。	

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
	
  (8) 线程清理 pthread_equal
    有时候希望线程退出时能够自动的执行某些函数。
    void pthread_cleanup_pop(int execute);
是执行函数，只有里面的参数 execute = 1 才会执行。
    void pthread_cleanup_push(void (*routine)(void*), void *arg);
是清理注册函数。
    ① 参数
    routine：注册清理的函数的指针。
    arg：传递给清理函数的参数。
    execute：决定这个清理函数是否被调用。
	② 其它
	有三种情况线程清理函数会被调用：
    线程还未执行 pthread_cleanup_pop 前，被 pthread_cancel 取消。
    线程还未执行 pthread_cleanup_pop 前，主动执行 pthread_exit 终止。
    线程执行 pthread_cleanup_pop，且 pthread_cleanup_pop 的参数不为 0。
	
	