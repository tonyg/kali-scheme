signal_count_is(38);
#ifdef SIGABRT
signal_map[0] = SIGABRT;
#endif
#ifdef SIGALRM
signal_map[1] = SIGALRM;
#endif
#ifdef SIGFPE
signal_map[2] = SIGFPE;
#endif
#ifdef SIGHUP
signal_map[3] = SIGHUP;
#endif
#ifdef SIGILL
signal_map[4] = SIGILL;
#endif
#ifdef SIGINT
signal_map[5] = SIGINT;
#endif
#ifdef SIGKILL
signal_map[6] = SIGKILL;
#endif
#ifdef SIGPIPE
signal_map[7] = SIGPIPE;
#endif
#ifdef SIGQUIT
signal_map[8] = SIGQUIT;
#endif
#ifdef SIGSEGV
signal_map[9] = SIGSEGV;
#endif
#ifdef SIGTERM
signal_map[10] = SIGTERM;
#endif
#ifdef SIGUSR1
signal_map[11] = SIGUSR1;
#endif
#ifdef SIGUSR2
signal_map[12] = SIGUSR2;
#endif
#ifdef SIGCHLD
signal_map[13] = SIGCHLD;
#endif
#ifdef SIGCONT
signal_map[14] = SIGCONT;
#endif
#ifdef SIGSTOP
signal_map[15] = SIGSTOP;
#endif
#ifdef SIGTSTP
signal_map[16] = SIGTSTP;
#endif
#ifdef SIGTTIN
signal_map[17] = SIGTTIN;
#endif
#ifdef SIGTTOU
signal_map[18] = SIGTTOU;
#endif
#ifdef SIGBUS
signal_map[19] = SIGBUS;
#endif
#ifdef SIGTRAP
signal_map[20] = SIGTRAP;
#endif
#ifdef SIGIOT
signal_map[21] = SIGIOT;
#endif
#ifdef SIGEMT
signal_map[22] = SIGEMT;
#endif
#ifdef SIGSYS
signal_map[23] = SIGSYS;
#endif
#ifdef SIGSTKFLT
signal_map[24] = SIGSTKFLT;
#endif
#ifdef SIGURG
signal_map[25] = SIGURG;
#endif
#ifdef SIGIO
signal_map[26] = SIGIO;
#endif
#ifdef SIGPOLL
signal_map[27] = SIGPOLL;
#endif
#ifdef SIGCLD
signal_map[28] = SIGCLD;
#endif
#ifdef SIGXCPU
signal_map[29] = SIGXCPU;
#endif
#ifdef SIGXFSZ
signal_map[30] = SIGXFSZ;
#endif
#ifdef SIGTALRM
signal_map[31] = SIGTALRM;
#endif
#ifdef SIGPROF
signal_map[32] = SIGPROF;
#endif
#ifdef SIGPWR
signal_map[33] = SIGPWR;
#endif
#ifdef SIGINFO
signal_map[34] = SIGINFO;
#endif
#ifdef SIGLOST
signal_map[35] = SIGLOST;
#endif
#ifdef SIGWINCH
signal_map[36] = SIGWINCH;
#endif
#ifdef SIGUNUSED
signal_map[37] = SIGUNUSED;
#endif
