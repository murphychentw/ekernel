// File: Config.h

/*
 Copyright (c) 1998-1999 Murphy Cheng-Che Chen. All Rights Reserved.

 This software is provided "AS IS," without a warranty of any kind. ALL
 EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. MURPHY CHENG-CHE CHEN AND HIS
 LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A
 RESULT OF USING, MODIFYING OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES.
 IN NO EVENT WILL MURPHY CHENG-CHE CHEN OR HIS LICENSORS BE LIABLE FOR ANY
 LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL,
 CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS
 OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE
 SOFTWARE, EVEN IF MURPHY CHENG-CHE CHEN HAS BEEN ADVISED OF THE POSSIBILITY
 OF SUCH DAMAGES.

 murphy@hpc.ee.ntu.edu.tw
 http://hpc.ee.ntu.edu.tw/~murphy/
*/


/*
1999/02/04: Murphy Cheng-Che Chen
	MAX_NO_SEMAPHORES
1999/02/28: Murphy
	MAX_NO_MESSAGEQUEUES
*/

#ifndef __EKERNEL_CONFIG_H_
#define __EKERNEL_CONFIG_H_



#define	MAX_NO_PROCESSES	1024	// maximal number of processes
#define	MAX_NO_THREADS		1024	// maximal number of threads
#define MAX_NO_EVENTS		1024	// maximal number of events
#define	MAX_NO_MESSAGEQUEUES	1024	// maximal number of message queues
#define	MAX_NO_MUTEXES		1024	// maximal number of mutexes
#define	MAX_NO_SEMAPHORES	1024	// maximal number of semaphores
#define MAX_NO_INTERRUPTS	256	// maximal number of interrupts

#endif
