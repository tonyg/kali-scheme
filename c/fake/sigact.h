/*
 * If we don't have sigaction, we fake it using signal.
 */
#if	! defined(HAVE_SIGACTION)

struct	sigaction {
	void	(*sa_handler)();
	int	sa_mask;
	int	sa_flags;
};

#define	sigaction(sig, act, oact)	signal((sig), (act)->sa_handler)
#define	sigemptyset(ign)		0

#endif
