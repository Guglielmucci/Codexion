*This project has been created as part of the 42 curriculum by fedegugl*

# Codexion — Master the race for resources before the deadline

*Summary: concurrent simulation where multiple coders compete for two shared USB dongles. The system handles concurrent access, request prioritization, resource cooldown, and burnout detection.*

## Description

Codexion is a concurrency simulator representing a group of coders sitting around a circular table. Each coder must acquire two adjacent USB dongles to compile, then proceeds with debugging and refactoring. The project uses POSIX threads to model each coder, mutexes and condition variables to protect shared dongles, and a monitor thread to stop the simulation when coders have completed the required number of compilations or a burnout occurs.

Core behavior:
- `num_coders` threads represent coders adjacent on a ring.
- Each coder requests the `left` and `right` dongles, globally ordered to avoid deadlock.
- Dongles have a cooldown period after release.
- Waiting on dongles can follow two schedulers: `fifo` and `edf`.
- A monitor checks for burnout and simulation completion.

## Instructions

Build:
```bash
make
```
run:
```bash
./codexion <num_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <compiles_required> <dongle_cooldown> <scheduler>
```

### Arguments:

- `num_coders`: number of coders and total dongles.
- `time_to_burnout`: maximum time allowed to complete a compilation before burnout.
- `time_to_compile`: duration of the compilation phase (ms).
- `time_to_debug`: debug phase duration (ms).
- `time_to_refactor`: refactoring phase duration (ms).
- `compiles_required`: number of compilations each coder must complete.
- `dongle_cooldown`: cooldown time in milliseconds after a dongle is released.
- `scheduler`: fifo or edf.

## Blocking cases handled

- **Deadlock prevention (Coffman conditions)**  
  Coders always acquire the lower‑index dongle first, then the higher‑index one.  
  This breaks the circular wait condition – one of the four necessary Coffman conditions – and eliminates deadlocks entirely.

- **Starvation reduction**  
  Each dongle maintains a request queue with FIFO or EDF priority, depending on the `scheduler` argument.

- **Cooldown handling**  
  After release, a dongle remains blocked until `cooldown_until` expires and notifies waiting threads via `pthread_cond_broadcast`.

- **Burnout detection**  
  If a coder fails to complete compilation within `time_to_burnout`, the simulation signals burnout and terminates.

- **Log serialization**  
  `stdout` access is serialized with `print_mutex` to prevent interleaved output.


## Thread synchronization mechanisms

- **`pthread_mutex_t` per `t_dongle`**  
  Protects the dongle’s internal state (`in_use`, `cooldown_until`, request heap, `holder_id`, `stop`).

- **`pthread_cond_t` per `t_dongle`**  
  Allows waiting coders to block until the dongle becomes available or cooldown ends.

- **`pthread_mutex_t stop_mutex`**  
  Protects the global `sim->stop` flag used to safely terminate the simulation.

- **`pthread_mutex_t print_mutex`**  
  Serialises log output.

### How synchronization works

- `try_take_dongle()` locks the dongle, attempts to acquire immediately, or inserts a request into a priority heap.
- If the dongle is unavailable, the request stays queued and the thread waits on `pthread_cond_timedwait` until deadline timeout or cooldown end.
- When a dongle is released, `release_dongle()` updates `in_use`, sets `cooldown_until`, and broadcasts on `cond` to wake waiting threads.
- `signal_stop()` sets `dongle->stop = 1` on all dongles and wakes condition variables, allowing threads to exit even on forced stop.
- `interruptible_sleep()` suspends the thread in 1 ms intervals and periodically checks `sim->stop`, enabling quick termination if the simulation is halted.

This structure ensures thread‑safe communication between coders, dongles, and the monitor, avoiding race conditions and inconsistent shutdowns.


## Resources

### Core documentation and concepts

- POSIX Threads (`pthread_create`, `pthread_mutex_t`, `pthread_cond_t`, `pthread_cond_timedwait`)
- `gettimeofday` and millisecond time management
- Heap structures for priority queues
- Deadlock prevention and starvation in concurrent systems

### External tutorials and discussions

- [How to create a simple thread in C](https://www.educative.io/answers/how-to-create-a-simple-thread-in-c)
- [Understanding Queue Data Structures in C: The First In, First Out Principle](https://medium.com/@noransaber685/understanding-queue-data-structures-in-c-the-first-in-first-out-principle-fbd1f89d40dc)
- [Understanding threads in C](https://medium.com/@akshatarhabib/understanding-threads-in-c-c9feb5e9372a)
- [How do I start threads in plain C?](https://stackoverflow.com/questions/56810/how-do-i-start-threads-in-plain-c)
- [Reddit discussion on C threads](https://reddit.com)


## AI usage

Claude (Anthropic) was used throughout the project for **code review, debugging, and documentation**.  
All algorithmic decisions were made and implemented by the author.  
AI assistance was limited to identifying bugs, suggesting fixes, verifying subject compliance, and drafting this README.