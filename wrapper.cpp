#include "wrapper.h"
#include<iostream>

class cFunctionError : public std::runtime_error {
public:
    cFunctionError(const std::string& msg) : std::runtime_error(msg) {
    }

    char const* what() const noexcept override {
        return runtime_error::what();
    }
};

void throwError(const std::string& msg) {
    throw cFunctionError(msg);
}

void throwError(int errorNum, const std::string& msg) {
    throw cFunctionError("error number(return code or errno) is " + std::to_string(errorNum) + "," + msg);
}

pid_t Fork(void) {
    pid_t pid;

    if ((pid = fork()) < 0)
        throwError("Fork error");
    return pid;
}


void Execve(const char* filename, char* const argv[], char* const envp[]) {
    if (execve(filename, argv, envp) < 0)
        throwError("Execve error");
}

pid_t Wait(int* status) {
    pid_t pid;

    if ((pid = wait(status)) < 0)
        throwError("Wait error");
    return pid;
}

pid_t Waitpid(pid_t pid, int* iptr, int options) {
    pid_t retpid;

    if ((retpid = waitpid(pid, iptr, options)) < 0)
        throwError("Waitpid error");
    return retpid;
}

void Kill(pid_t pid, int signum) {
    int rc;

    if ((rc = kill(pid, signum)) < 0)
        throwError(rc, "Kill error");
}

void Pause() {
    (void) pause();
    return;
}

unsigned int Sleep(unsigned int secs) {
    unsigned int rc;

    if ((rc = sleep(secs)) < 0)
        throwError("Sleep error");
    return rc;
}

unsigned int Alarm(unsigned int seconds) {
    return alarm(seconds);
}

void Setpgid(pid_t pid, pid_t pgid) {
    int rc;

    if ((rc = setpgid(pid, pgid)) < 0)
        throwError(rc, "Setpgid error");
}

pid_t Getpgrp(void) {
    return getpgrp();
}

/************************************
 * Wrappers for Unix signal functions 
 ***********************************/

/* $begin sigaction */
handler_t* Signal(int signum, handler_t* handler) {
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        throwError("Signal error");
    return (old_action.sa_handler);
}

/* $end sigaction */

void Sigprocmask(int how, const sigset_t* set, sigset_t* oldset) {
    if (sigprocmask(how, set, oldset) < 0)
        throwError("Sigprocmask error");
    return;
}

void Sigemptyset(sigset_t* set) {
    if (sigemptyset(set) < 0)
        throwError("Sigemptyset error");
    return;
}

void Sigfillset(sigset_t* set) {
    if (sigfillset(set) < 0)
        throwError("Sigfillset error");
    return;
}

void Sigaddset(sigset_t* set, int signum) {
    if (sigaddset(set, signum) < 0)
        throwError("Sigaddset error");
    return;
}

void Sigdelset(sigset_t* set, int signum) {
    if (sigdelset(set, signum) < 0)
        throwError("Sigdelset error");
    return;
}

int Sigismember(const sigset_t* set, int signum) {
    int rc;
    if ((rc = sigismember(set, signum)) < 0)
        throwError("Sigismember error");
    return rc;
}

int Sigsuspend(const sigset_t* set) {
    int rc = sigsuspend(set); /* always returns -1 */
    if (errno != EINTR)
        throwError("Sigsuspend error");
    return rc;
}


/********************************
 * Wrappers for Unix I/O routines
 ********************************/
int Open(const char* pathname, int flags, mode_t mode) {
    int rc;

    if ((rc = open(pathname, flags, mode)) < 0)
        throwError("Open error");
    return rc;
}

ssize_t Read(int fd, void* buf, size_t count) {
    ssize_t rc;

    if ((rc = read(fd, buf, count)) < 0)
        throwError(errno,"Read error");
    return rc;
}

ssize_t Write(int fd, const void* buf, size_t count) {
    ssize_t rc;

    if ((rc = write(fd, buf, count)) < 0)
        throwError("Write error");
    return rc;
}

off_t Lseek(int fildes, off_t offset, int whence) {
    off_t rc;

    if ((rc = lseek(fildes, offset, whence)) < 0)
        throwError("Lseek error");
    return rc;
}

void Close(int fd) {
    int rc;

    if ((rc = close(fd)) < 0)
        throwError(rc, "Close error");
}

int Select(int n, fd_set* readfds, fd_set* writefds,
           fd_set* exceptfds, struct timeval* timeout) {
    int rc;

    if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0)
        throwError("Select error");
    return rc;
}

int Dup2(int fd1, int fd2) {
    int rc;

    if ((rc = dup2(fd1, fd2)) < 0)
        throwError("Dup2 error");
    return rc;
}

void Stat(const char* filename, struct stat* buf) {
    if (stat(filename, buf) < 0)
        throwError("Stat error");
}

void Fstat(int fd, struct stat* buf) {
    if (fstat(fd, buf) < 0)
        throwError("Fstat error");
}

/*********************************
 * Wrappers for directory function
 *********************************/

DIR* Opendir(const char* name) {
    DIR* dirp = opendir(name);

    if (!dirp)
        throwError("opendir error");
    return dirp;
}

struct dirent* Readdir(DIR* dirp) {
    struct dirent* dep;

    errno = 0;
    dep = readdir(dirp);
    if ((dep == NULL) && (errno != 0))
        throwError("readdir error");
    return dep;
}

int Closedir(DIR* dirp) {
    int rc;

    if ((rc = closedir(dirp)) < 0)
        throwError("closedir error");
    return rc;
}

/***************************************
 * Wrappers for memory mapping functions
 ***************************************/
void* Mmap(void* addr, size_t len, int prot, int flags, int fd, off_t offset) {
    void* ptr;

    if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void*) -1))
        throwError("mmap error");
    return (ptr);
}

void Munmap(void* start, size_t length) {
    if (munmap(start, length) < 0)
        throwError("munmap error");
}

/***************************************************
 * Wrappers for dynamic storage allocation functions
 ***************************************************/

void* Malloc(size_t size) {
    void* p;

    if ((p = malloc(size)) == NULL)
        throwError("Malloc error");
    return p;
}

void* Realloc(void* ptr, size_t size) {
    void* p;

    if ((p = realloc(ptr, size)) == NULL)
        throwError("Realloc error");
    return p;
}

void* Calloc(size_t nmemb, size_t size) {
    void* p;

    if ((p = calloc(nmemb, size)) == NULL)
        throwError("Calloc error");
    return p;
}

void Free(void* ptr) {
    free(ptr);
}

/******************************************
 * Wrappers for the Standard I/O functions.
 ******************************************/
void Fclose(FILE* fp) {
    if (fclose(fp) != 0)
        throwError("Fclose error");
}

FILE* Fdopen(int fd, const char* type) {
    FILE* fp;

    if ((fp = fdopen(fd, type)) == NULL)
        throwError("Fdopen error");

    return fp;
}

char* Fgets(char* ptr, int n, FILE* stream) {
    char* rptr;

    if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
        throwError("Fgets error");

    return rptr;
}

FILE* Fopen(const char* filename, const char* mode) {
    FILE* fp;

    if ((fp = fopen(filename, mode)) == NULL)
        throwError("Fopen error");

    return fp;
}

void Fputs(const char* ptr, FILE* stream) {
    if (fputs(ptr, stream) == EOF)
        throwError("Fputs error");
}

size_t Fread(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t n;

    if (((n = fread(ptr, size, nmemb, stream)) < nmemb) && ferror(stream))
        throwError("Fread error");
    return n;
}

void Fwrite(const void* ptr, size_t size, size_t nmemb, FILE* stream) {
    if (fwrite(ptr, size, nmemb, stream) < nmemb)
        throwError("Fwrite error");
}


/**************************** 
 * Sockets interface wrappers
 ****************************/

int Socket(int domain, int type, int protocol) {
    int rc;

    if ((rc = socket(domain, type, protocol)) < 0)
        throwError("Socket error");
    return rc;
}

void Setsockopt(int s, int level, int optname, const void* optval, int optlen) {
    int rc;

    if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
        throwError(rc, "Setsockopt error");
}

void Bind(int sockfd, struct sockaddr* my_addr, socklen_t addrlen) {
    int rc;

    if ((rc = bind(sockfd, my_addr, addrlen)) < 0)
        throwError(rc, "Bind error");
}

void Listen(int s, int backlog) {
    int rc;

    if ((rc = listen(s, backlog)) < 0)
        throwError(rc, "Listen error");
}

int Accept(int s, struct sockaddr* addr, socklen_t* addrlen) {
    int rc;

    if ((rc = accept(s, addr, addrlen)) < 0)
        throwError("Accept error");
    return rc;
}

void Connect(int sockfd, struct sockaddr* serv_addr, int addrlen) {
    int rc;

    if ((rc = connect(sockfd, serv_addr, addrlen)) < 0)
        throwError(rc, "Connect error");
}

/*******************************
 * Protocol-independent wrappers
 *******************************/
/* $begin getaddrinfo */
void Getaddrinfo(const char* node, const char* service,
                 const struct addrinfo* hints, struct addrinfo** res) {
    int rc;

    if ((rc = getaddrinfo(node, service, hints, res)) != 0)
        throwError(rc, "Getaddrinfo failed (from " + std::string(service) + "): " + gai_strerror(rc));
}

/* $end getaddrinfo */

void Getnameinfo(const struct sockaddr* sa, socklen_t salen, char* host,
                 size_t hostlen, char* serv, size_t servlen, int flags) {
    int rc;

    if ((rc = getnameinfo(sa, salen, host, hostlen, serv,
                          servlen, flags)) != 0)
        throwError(rc, "Getnameinfo error");
}

void Freeaddrinfo(struct addrinfo* res) {
    freeaddrinfo(res);
}

void Inet_ntop(int af, const void* src, char* dst, socklen_t size) {
    if (!inet_ntop(af, src, dst, size))
        throwError("Inet_ntop error");
}

void Inet_pton(int af, const char* src, void* dst) {
    int rc;

    rc = inet_pton(af, src, dst);
    if (rc == 0)
        throwError("inet_pton error: invalid dotted-decimal address");
    else if (rc < 0)
        throwError("Inet_pton error");
}

/*******************************************
 * DNS interface wrappers. 
 *
 * NOTE: These are obsolete because they are not thread safe. Use
 * getaddrinfo and getnameinfo instead
 ***********************************/

/* $begin gethostbyname */
struct hostent* Gethostbyname(const char* name) {
    struct hostent* p;

    if ((p = gethostbyname(name)) == NULL)
        throwError("Gethostbyname error");
    return p;
}

/* $end gethostbyname */

struct hostent* Gethostbyaddr(const char* addr, int len, int type) {
    struct hostent* p;

    if ((p = gethostbyaddr(addr, len, type)) == NULL)
        throwError("Gethostbyaddr error");
    return p;
}

/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/

void Pthread_create(pthread_t* tidp, pthread_attr_t* attrp,
                    void* (* routine)(void*), void* argp) {
    int rc;

    if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0)
        throwError(rc, "Pthread_create error");
}

void Pthread_cancel(pthread_t tid) {
    int rc;

    if ((rc = pthread_cancel(tid)) != 0)
        throwError(rc, "Pthread_cancel error");
}

void Pthread_join(pthread_t tid, void** thread_return) {
    int rc;

    if ((rc = pthread_join(tid, thread_return)) != 0)
        throwError(rc, "Pthread_join error");
}

/* $begin detach */
void Pthread_detach(pthread_t tid) {
    int rc;

    if ((rc = pthread_detach(tid)) != 0)
        throwError(rc, "Pthread_detach error");
}

/* $end detach */

void Pthread_exit(void* retval) {
    pthread_exit(retval);
}

pthread_t Pthread_self(void) {
    return pthread_self();
}

void Pthread_once(pthread_once_t* once_control, void (* init_function)()) {
    pthread_once(once_control, init_function);
}

/*******************************
 * Wrappers for Posix semaphores
 *******************************/

void Sem_init(sem_t* sem, int pshared, unsigned int value) {
    if (sem_init(sem, pshared, value) < 0)
        throwError("Sem_init error");
}

void P(sem_t* sem) {
    if (sem_wait(sem) < 0)
        throwError("P error");
}

void V(sem_t* sem) {
    if (sem_post(sem) < 0)
        throwError("V error");
}

/* IPC wrappers*/
int Pipe(int pipefd[2]) {
    int rc = 0;
    if ((rc = pipe(pipefd)) == -1)
        throwError("Pipe error");
    return rc;
}
