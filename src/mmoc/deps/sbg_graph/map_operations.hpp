
#pragma once

#include <deps/sbg_graph/set.h>
#include <deps/sbg_graph/linear_map.h>
#include <deps/sbg_graph/pw_lmap.h>

namespace SB {

PWLMap minAtomPW(AtomSet &dom, LMap &lm1, LMap &lm2);
PWLMap minPW(Set &dom, LMap &lm1, LMap &lm2);
PWLMap minMap(PWLMap &pw1, PWLMap &pw2);
PWLMap minInv(PWLMap &pw, Set &s);

PWLMap reduceMapN(PWLMap pw, int dim);

PWLMap mapInf(PWLMap pw);

PWLMap minAdjCompMap(PWLMap pw2, PWLMap pw1);

PWLMap minAdjMap(PWLMap pw2, PWLMap pw1);
} // namespace SB
