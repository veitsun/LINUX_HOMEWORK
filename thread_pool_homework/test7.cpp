// 三个线程共同访问count变量
// thread 2 和 thread 3 竞争地对 count 进行加 1 操作
// thread 1 等 count 达到 12 的时候，count 一次性加 125
// thread 2 和 thread 3 再去竞争 count 的控制权
// 直到thread 2和 thread 3 完成对 count 一共加 10 次的操作

#include <climits>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 3
// 一共 3 个线程

#define COUNT_LIMIT 12
#define TCOUNT 10

int count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *watch_count(void *t) {
  long my_id = (long)t;
  printf("starting watch_count(): thread %ld\n", my_id);

  pthread_mutex_lock(&count_mutex);
  while (count < COUNT_LIMIT) {
    printf("watch_count(): thread %ld Count = %d . Going into wait...\n", my_id,
           count);
    pthread_cond_wait(&count_threshold_cv, &count_mutex);
    printf("watch_count(): thread %ld Condition signal received. Count = %d \n",
           my_id, count);
    printf("watch_count(): thread %ld Updating the value of count ...\n",
           my_id);
    count += 125;
    printf("watch_count(): thread %ld count now = %d. \n", my_id, count);
  }
  printf("watch_count(): thread %ld Unlocking mutex.\n", my_id);
  pthread_mutex_unlock(&count_mutex);
  pthread_exit(nullptr);
}

void *inc_count(void *t) {
  // int i;
  long my_id = (long)t;

  for (int i = 0; i < TCOUNT; i++) {
    pthread_mutex_lock(&count_mutex);
    count++;

    if (count == COUNT_LIMIT) {
      printf("inc_count(): thread %ld, count = %d Threshold reached.", my_id,
             count);
      pthread_cond_signal(&count_threshold_cv);
      printf("just sent signal");
    }
    printf("inc_count(): thread %ld, count = %d, unlocking mutex\n", my_id,
           count);
    pthread_mutex_unlock(&count_mutex);
    sleep(1);
  }
  pthread_exit(nullptr);
}

int main(int argc, char *argv[]) {
  long t1 = 1, t2 = 2, t3 = 3;
  pthread_t threads[3];
  pthread_attr_t attr;
  pthread_mutex_init(&count_mutex, nullptr);
  pthread_cond_init(&count_threshold_cv, nullptr);

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&threads[0], &attr, watch_count, (void *)t1);
  pthread_create(&threads[1], &attr, inc_count, (void *)t2);
  pthread_create(&threads[3], &attr, inc_count, (void *)t3);

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], nullptr);
  }
  printf("Main(): Waited and joined with %d threads. Final value of count = "
         "%d. Done.\n",
         NUM_THREADS, count);

  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&count_mutex);
  pthread_cond_destroy(&count_threshold_cv);
  pthread_exit(NULL);
}
