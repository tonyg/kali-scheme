#include <nlist.h>
main()
{
  struct nlist name_list;
  name_list.n_name = "foo";
  return 0;
}
