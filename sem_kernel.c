#include <linux/nmi.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/highmem.h>
#include <linux/smp_lock.h>
#include <asm/mmu_context.h>
#include <linux/interrupt.h>
#include <linux/capability.h>
#include <linux/completion.h>
#include <linux/kernel_stat.h>
#include <linux/debug_locks.h>
#include <linux/security.h>
#include <linux/notifier.h>
#include <linux/profile.h>
#include <linux/freezer.h>
#include <linux/vmalloc.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/pid_namespace.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/timer.h>
#include <linux/rcupdate.h>
#include <linux/cpu.h>
#include <linux/cpuset.h>
#include <linux/percpu.h>
#include <linux/kthread.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sysctl.h>
#include <linux/syscalls.h>
#include <linux/times.h>
#include <linux/tsacct_kern.h>
#include <linux/kprobes.h>
#include <linux/delayacct.h>
#include <linux/reciprocal_div.h>
#include <linux/unistd.h>
#include <linux/pagemap.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
#include <linux/bootmem.h>
#include <linux/debugfs.h>
#include <linux/ctype.h>
#include <linux/ftrace.h>
#include <trace/sched.h>

#include "sem.h"

#define  MAX_SEM 15

typedef struct sem_t {
    char nom[ MAX_NOM];
    uid_t proprietaire;
    int value;
    struct task_struct* att [MAX_PID];
    int att_nb;
    struct task_struct* wait [MAX_PID];
    int wait_nb;
} sem_t ;
 
#define SEM_LIBRE(s) ((s)->att_nb == 0)
sem_t sems[MAX_SEM];
int   sems_nb;
  
SYSCALL_DEFINE2(sem_list,Tinfo *, t, int, tnb)
{ 
    int j,i,r;
    /* verifier tnb */

    /* vérifier que t jusqua t+tnb est une zone user valide (getuser..) */

    /* transfert des sems[i] dans t[r] */
    for(i =0,r=0 ; i<MAX_SEM && r<tnb;i++)
    {
        sem_t* s = sems +i; 
        if  (SEM_LIBRE(s) ) continue ;
        
        /* vérifier que strcpy existe dans le noyau, la coder si c'est pas le cas */
        strcpy (t[r].nom, s->nom ); // copie s-> nom dans t[r].nom
        t[r].prop = s->prop ; // copie sprop dans la Rième case de t 
        
        for(j=0; j<s->wait_nb;j++)
          t[r].pids_wait[j] = task_tgid_vnr(s->wait[j]);
        t[r].pid_wait_nb = s->wait_nb ;
	}
    return r;
}
