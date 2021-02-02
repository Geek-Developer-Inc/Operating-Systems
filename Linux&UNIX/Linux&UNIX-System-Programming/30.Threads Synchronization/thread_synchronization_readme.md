第30章 线程:线程同步
         THREADS:THREAD SYNCHRONIZATION

说明: (1)本章对应 Linux/Unix 系统编程手册(上册) 第 30 章 线程:线程同步
         <<The Linux Programming Interface-A Linux and UNIX System Programming 
		      Handbook>> 30 Threads:Thread Synchronization 
      (2)操作系统导论(Operating Systems) 第 27 章 插叙:线程API
	     <<Operating Systems: Three Easy Pieces>> 27 Interlude:Thread API

30.1 互斥量-保护对共享变量的访问
     Protecting Accesses to Shared Variables:Mutexes

    为避免线程更新共享变量时所出现的问题，必须使用互斥量(Mutex 是 mutual exc-
lusion 的缩写)，来确保同时仅有一个线程可以访问某项共享资源。
    To avoid the problems that can occur when threads try to update a shared 
variable,we must use a mutex(short for mutual exclusion)to ensure that only 
one thread at atime can access the variable.
    
	可以使用互斥量来保证对任意共享资源的原子访问，而保护共享变量是常见的方法。
	Mutexes can be used to ensure atomic access to any shared resource,but pr-
otecting shared variable is the most common use.

    互斥量有两种状态：已锁定(locked)和未锁定(unlocked)。任何时候，至多只有一
个线程可以锁定该互斥量。
    A mutex han two states:locked and unlocked,At any moment,at most one thre-
ad may hold the lock on a mutex.

    一旦线程锁定互斥量，随即成为该互斥量的所有者。只有所有者才能给互斥量解锁。
	When a thread locks a mutex,it becomes the owner of that mutex.Only the
mutex owner can unlock the mutex.