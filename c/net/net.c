/* Copyright (c) 1993-2007 by Richard Kelsey and Jonathan Rees.
   See file COPYING. */

extern void s48_init_net_addresses(void);
extern void s48_init_net_sockets(void);

void
s48_init_net(void)
{
  s48_init_net_addresses();
  s48_init_net_sockets();
}
