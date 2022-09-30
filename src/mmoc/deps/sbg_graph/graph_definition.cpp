/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include <iostream>
#include <list>
#include <map>
#include <math.h>
#include <utility>

#include <boost/config.hpp>
#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/unordered_set.hpp>

#include <util/debug.h>
#include <util/graph/graph_definition.h>
#include <util/table.h>

namespace SBG {
size_t hash_value(SetVertex v) { return v.hash(); }

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
// Printing instances
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

ostream &operator<<(ostream &out, Interval &i)
{
  out << "[" << i.lo_() << ":" << i.step_() << ":" << i.hi_() << "]";
  return out;
}

ostream &operator<<(ostream &out, MultiInterval &mi)
{
  OrdCT<Interval> is = mi.inters_();
  OrdCT<Interval>::iterator it = is.begin();

  if (is.size() == 0) return out;

  if (is.size() == 1) {
    out << *it;
    return out;
  }

  out << *it;
  ++it;
  while (next(it, 1) != is.end()) {
    out << "x" << *it;
    ++it;
  }
  out << "x" << *it;

  return out;
}

ostream &operator<<(ostream &out, AtomSet &as)
{
  MultiInterval mi = as.aset_();

  out << "{" << mi << "}";

  return out;
}

ostream &operator<<(ostream &out, Set &s)
{
  UnordCT<AtomSet> as = s.asets_();
  UnordCT<AtomSet>::iterator it = as.begin();
  AtomSet aux;

  if (as.size() == 0) {
    out << "{}";
    return out;
  }

  if (as.size() == 1) {
    aux = *it;
    out << "{" << aux << "}";
    return out;
  }

  aux = *it;
  out << "{" << aux;
  ++it;
  while (next(it, 1) != as.end()) {
    aux = *it;
    out << ", " << aux;
    ++it;
  }
  aux = *it;
  out << ", " << aux << "}";

  return out;
}

std::string mapOper(NI2 cte) { return (cte >= 0) ? "+ " : ""; }

ostream &operator<<(ostream &out, LMap &lm)
{
  OrdCT<NI2> g = lm.gain_();
  OrdCT<NI2>::iterator itg = g.begin();
  OrdCT<NI2> o = lm.off_();
  OrdCT<NI2>::iterator ito = o.begin();

  if (g.size() == 0) return out;

  if (g.size() == 1) {
    out << "[" << *itg << " * x " << mapOper(*ito) << *ito << "]";
    return out;
  }

  out << "[" << *itg << " * x " << mapOper(*ito) << *ito;
  ++itg;
  ++ito;
  while (next(itg, 1) != g.end()) {
    out << ", " << *itg << " * x " << mapOper(*ito) << *ito;

    ++itg;
    ++ito;
  }
  out << ", " << *itg << " * x " << mapOper(*ito) << *ito << "]";

  return out;
}

ostream &operator<<(ostream &out, PWAtomLMap &pwatom)
{
  AtomSet d = pwatom.dom_();
  LMap lm = pwatom.lmap_();

  out << "(" << d << ", " << lm << ")";
  return out;
}

ostream &operator<<(ostream &out, PWLMap &pw)
{
  OrdCT<Set> d = pw.dom_();
  OrdCT<Set>::iterator itd = d.begin();
  OrdCT<LMap> l = pw.lmap_();
  OrdCT<LMap>::iterator itl = l.begin();

  if (d.size() == 0) {
    out << "[]";
    return out;
  }

  if (d.size() == 1) {
    out << "[(" << *itd << ", " << *itl << ")]";
    return out;
  }

  out << "[(" << *itd << ", " << *itl << ") ";
  ++itd;
  ++itl;
  while (next(itd, 1) != d.end()) {
    out << ", (" << *itd << ", " << *itl << ")";

    ++itd;
    ++itl;
  }
  out << ", (" << *itd << ", " << *itl << ")]";

  return out;
}

/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
// Map operations
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/

// This operations are implemented here to simplificate the implementation.

PWLMap minAtomPW(AtomSet &dom, LMap &lm1, LMap &lm2)
{
  OrdCT<NI2> g1 = lm1.gain_();
  OrdCT<NI2> o1 = lm1.off_();
  OrdCT<NI2>::iterator ito1 = o1.begin();
  OrdCT<NI2> g2 = lm2.gain_();
  OrdCT<NI2>::iterator itg2 = g2.begin();
  OrdCT<NI2> o2 = lm2.off_();
  OrdCT<NI2>::iterator ito2 = o2.begin();
  OrdCT<Interval> ints = dom.aset_().inters_();
  OrdCT<Interval>::iterator itints = ints.begin();

  AtomSet asAux = dom;
  LMap lmAux = lm1;
  OrdCT<NI2> resg = g1;
  OrdCT<NI2> reso = o1;
  int count = 1;

  OrdCT<Set> domRes;
  OrdCT<LMap> lmRes;

  if (lm1.ndim_() == lm2.ndim_()) {
    BOOST_FOREACH (NI2 g1i, g1) {
      if (g1i != *itg2) {
        NI2 xinter = (*ito2 - *ito1) / (g1i - *itg2);

        // Intersection before domain
        if (xinter <= (*itints).lo_()) {
          if (*itg2 < g1i) lmAux = lm2;

          Set sAux;
          sAux.addAtomSet(asAux);

          domRes.insert(domRes.begin(), sAux);
          lmRes.insert(lmRes.begin(), lmAux);
        }

        // Intersection after domain
        else if (xinter >= (*itints).hi_()) {
          if (*itg2 > g1i) lmAux = lm2;

          Set sAux;
          sAux.addAtomSet(asAux);

          domRes.insert(domRes.begin(), sAux);
          lmRes.insert(lmRes.begin(), lmAux);
        }

        // Intersection in domain
        else {
          Interval i1((*itints).lo_(), (*itints).step_(), floor(xinter));
          Interval i2(i1.hi_() + i1.step_(), (*itints).step_(), (*itints).hi_());

          AtomSet as1 = asAux.replace(i1, count);
          AtomSet as2 = asAux.replace(i2, count);

          Set d1;
          d1.addAtomSet(as1);

          Set d2;
          d2.addAtomSet(as2);

          domRes.insert(domRes.end(), d1);
          domRes.insert(domRes.end(), d2);

          if (g1i > *itg2) {
            lmRes.insert(lmRes.end(), lm1);
            lmRes.insert(lmRes.end(), lm2);
          }

          else {
            lmRes.insert(lmRes.end(), lm2);
            lmRes.insert(lmRes.end(), lm1);
          }
        }

        PWLMap auxRes(domRes, lmRes);
        return auxRes;
      }

      else if (*ito1 != *ito2) {
        if (*ito2 < *ito1) lmAux = lm2;

        Set sAux;
        sAux.addAtomSet(asAux);
        domRes.insert(domRes.begin(), sAux);
        lmRes.insert(lmRes.begin(), lmAux);

        PWLMap auxRes(domRes, lmRes);
        return auxRes;
      }

      ++ito1;
      ++itg2;
      ++ito2;
      ++itints;
      ++count;
    }
  }

  Set sAux;
  sAux.addAtomSet(dom);
  domRes.insert(domRes.begin(), sAux);
  lmRes.insert(lmRes.begin(), lm1);
  PWLMap auxRes(domRes, lmRes);
  return auxRes;
}

PWLMap minPW(Set &dom, LMap &lm1, LMap &lm2)
{
  OrdCT<Set> sres;
  OrdCT<LMap> lres;

  Set sres1;
  Set sres2;
  LMap lres1;
  LMap lres2;

  UnordCT<AtomSet> asets = dom.asets_();
  UnordCT<AtomSet>::iterator itas = asets.begin();

  if (!dom.empty()) {
    PWLMap aux;
    AtomSet asAux = *itas;
    aux = minAtomPW(asAux, lm1, lm2);
    if (!aux.empty()) {
      sres1 = *(aux.dom_().begin());
      lres1 = *(aux.lmap_().begin());
      ++itas;

      OrdCT<Set> d;
      OrdCT<Set>::iterator itd;
      OrdCT<LMap> l;
      OrdCT<LMap>::iterator itl;
      while (itas != asets.end()) {
        asAux = *itas;
        aux = minAtomPW(asAux, lm1, lm2);
        d = aux.dom_();
        itd = d.begin();
        l = aux.lmap_();
        itl = l.begin();

        while (itd != d.end()) {
          if (*itl == lres1)
            sres1 = sres1.cup(*itd);

          else {
            if (sres2.empty()) {
              sres2 = *itd;
              lres2 = *itl;
            }

            else
              sres2 = sres2.cup(*itd);
          }

          ++itd;
          ++itl;
        }

        ++itas;
      }
    }
  }

  if (!sres1.empty() && !lres1.empty()) {
    sres.insert(sres.end(), sres1);
    lres.insert(lres.end(), lres1);
  }

  if (!sres2.empty() && !lres2.empty()) {
    sres.insert(sres.end(), sres2);
    lres.insert(lres.end(), lres2);
  }

  PWLMap res(sres, lres);
  return res;
}

PWLMap minMap(PWLMap &pw1, PWLMap &pw2)
{
  PWLMap res;

  OrdCT<LMap> lm1 = pw1.lmap_();
  OrdCT<LMap>::iterator itl1 = lm1.begin();
  OrdCT<LMap> lm2 = pw2.lmap_();

  if (!pw1.empty() && !pw2.empty()) {
    BOOST_FOREACH (Set s1i, pw1.dom_()) {
      OrdCT<LMap>::iterator itl2 = lm2.begin();

      BOOST_FOREACH (Set s2j, pw2.dom_()) {
        Set dom = s1i.cap(s2j);

        if (!dom.empty()) {
          PWLMap aux = minPW(dom, *itl1, *itl2);

          if (res.empty())
            res = aux;

          else
            res = aux.combine(res);
        }

        ++itl2;
      }

      ++itl1;
    }
  }

  return res;
}

PWLMap minInv(PWLMap &pw, Set &s)
{
  if (!pw.empty()) {
    // Initialization
    OrdCT<Set> auxdom = pw.dom_();
    OrdCT<Set>::iterator itdom = auxdom.begin();
    OrdCT<LMap> auxlm = pw.lmap_();
    OrdCT<LMap>::iterator itlm = auxlm.begin();

    Set dom1 = *itdom;
    ++itdom;
    LMap lm1 = *itlm;
    ++itlm;

    OrdCT<Set> pw2dom;
    pw2dom.insert(pw2dom.begin(), dom1);
    OrdCT<LMap> pw2lm;
    pw2lm.insert(pw2lm.begin(), lm1);
    PWLMap pw2(pw2dom, pw2lm);

    PWLMap invpw2 = pw2.minInvCompact(s);
    pw2 = invpw2;

    for (; itdom != auxdom.end(); ++itdom) {
      OrdCT<Set> pwidom;
      pwidom.insert(pwidom.begin(), *itdom);
      OrdCT<LMap> pwilm;
      pwilm.insert(pwilm.begin(), *itlm);
      PWLMap pwi(pwidom, pwilm);

      PWLMap invpwi = pwi.minInvCompact(s);

      PWLMap minmap = minMap(pw2, invpwi);
      PWLMap combmap = invpwi.combine(pw2);
      pw2 = combmap;

      if (!minmap.empty()) {
        PWLMap combmap2 = minmap.combine(pw2);
        pw2 = combmap2;
      }

      ++itlm;
    }

    return pw2;
  }

  PWLMap res;
  return res;
}

PWLMap reduceMapN(PWLMap pw, int dim)
{
  OrdCT<Set> sres = pw.dom_();
  OrdCT<Set>::iterator itsres = sres.end();
  OrdCT<LMap> lres = pw.lmap_();
  OrdCT<LMap>::iterator itlres = lres.end();

  OrdCT<LMap> lm = pw.lmap_();
  OrdCT<LMap>::iterator itlm = lm.begin();

  unsigned int i = 1;
  BOOST_FOREACH (Set di, pw.dom_()) {
    int count1 = 1;

    OrdCT<NI2> g = (*itlm).gain_();
    OrdCT<NI2>::iterator itg = g.begin();
    OrdCT<NI2> o = (*itlm).off_();
    OrdCT<NI2>::iterator ito = o.begin();
    // Get the dim-th gain and offset
    while (count1 < dim) {
      ++itg;
      ++ito;
      ++count1;
    }

    if (*itg == 1 && *ito < 0) {
      NI2 off = -(*ito);

      BOOST_FOREACH (AtomSet adom, di.asets_()) {
        MultiInterval mi = adom.aset_();
        OrdCT<Interval> inters = mi.inters_();
        OrdCT<Interval>::iterator itints = inters.begin();

        int count2 = 1;
        while (count2 < dim) {
          ++itints;
          ++count2;
        }

        NI1 loint = (*itints).lo_();
        NI1 hiint = (*itints).hi_();

        if ((hiint - loint) > (off * off)) {
          OrdCT<Set> news;
          OrdCT<Set>::iterator itnews = news.begin();
          OrdCT<LMap> newl;
          OrdCT<LMap>::iterator itnewl = newl.begin();

          for (int k = 1; k <= off; k++) {
            OrdCT<NI2> newo = (*itlm).off_();
            OrdCT<NI2>::iterator itnewo = newo.begin();

            OrdCT<NI2> resg;
            OrdCT<NI2>::iterator itresg = resg.begin();
            OrdCT<NI2> reso;
            OrdCT<NI2>::iterator itreso = reso.begin();

            int count3 = 1;
            BOOST_FOREACH (NI2 gi, (*itlm).gain_()) {
              if (count3 == dim) {
                itresg = resg.insert(itresg, 0);
                itreso = reso.insert(itreso, loint + k - off - 1);
              }

              else {
                itresg = resg.insert(itresg, gi);
                itreso = reso.insert(itreso, *itnewo);
              }

              ++itresg;
              ++itreso;
              ++itnewo;
              ++count3;
            }

            LMap newlmap(resg, reso);
            Interval newinter(loint + k - 1, off, hiint);
            AtomSet auxas = adom.replace(newinter, dim);
            Set newset;
            newset.addAtomSet(auxas);

            itnews = news.insert(itnews, newset);
            ++itnews;
            itnewl = newl.insert(itnewl, newlmap);
            ++itnewl;
          }

          PWLMap newmap(news, newl);

          UnordCT<AtomSet> auxnewd;
          BOOST_FOREACH (AtomSet auxasi, di.asets_()) {
            if (auxasi != adom) auxnewd.insert(auxasi);
          }

          Set newdomi(auxnewd);

          if (newdomi.empty()) {
            itlres = lres.begin();

            if (i < sres.size()) {
              OrdCT<Set> auxs;
              OrdCT<Set>::iterator itauxs = auxs.begin();
              OrdCT<LMap> auxl;
              OrdCT<LMap>::iterator itauxl = auxl.begin();

              unsigned int count4 = 1;
              BOOST_FOREACH (Set si, sres) {
                if (count4 != i) {
                  itauxs = auxs.insert(itauxs, si);
                  ++itauxs;
                  itauxl = auxl.insert(itauxl, *itlres);
                  ++itauxl;
                }

                ++count4;
                ++itlres;
              }

              sres = auxs;
              lres = auxl;
            }

            else {
              OrdCT<Set> auxs;
              OrdCT<Set>::iterator itauxs = auxs.begin();
              OrdCT<LMap> auxl;
              OrdCT<LMap>::iterator itauxl = auxl.begin();

              unsigned int count4 = 1;
              BOOST_FOREACH (Set si, sres) {
                if (count4 < i) {
                  itauxs = auxs.insert(itauxs, si);
                  ++itauxs;
                  itauxl = auxl.insert(itauxl, *itlres);
                  ++itauxl;
                }

                ++count4;
                ++itlres;
              }

              sres = auxs;
              lres = auxl;
            }
          }

          else {
            OrdCT<Set> auxs;
            OrdCT<Set>::iterator itauxs = auxs.begin();
            OrdCT<Set>::iterator itauxsres = sres.begin();
            unsigned int count5 = 1;
            while (itauxsres != sres.end()) {
              if (count5 == i)
                itauxs = auxs.insert(itauxs, newdomi);

              else
                itauxs = auxs.insert(itauxs, *itauxsres);

              ++itauxs;
              ++itauxsres;
              ++count5;
            }

            sres = auxs;
          }

          BOOST_FOREACH (Set newi, newmap.dom_()) {
            itsres = sres.insert(itsres, newi);
            ++itsres;
          }

          BOOST_FOREACH (LMap newi, newmap.lmap_()) {
            itlres = lres.insert(itlres, newi);
            ++itlres;
          }
        }
      }
    }

    ++itlm;
    ++i;
  }

  PWLMap res(sres, lres);
  return res;
}

PWLMap mapInf(PWLMap pw)
{
  PWLMap res;
  if (!pw.empty()) {
    res = reduceMapN(pw, 1);

    for (int i = 2; i <= res.ndim_(); ++i) res = reduceMapN(res, i);

    int maxit = 0;

    OrdCT<Set> doms = res.dom_();
    OrdCT<Set>::iterator itdoms = doms.begin();
    BOOST_FOREACH (LMap lm, res.lmap_()) {
      OrdCT<NI2> o = lm.off_();
      OrdCT<NI2>::iterator ito = o.begin();

      NI2 a = 0;
      NI2 b = *(lm.gain_().begin());

      BOOST_FOREACH (NI2 gi, lm.gain_()) {
        a = max(a, gi * abs(*ito));
        b = min(b, gi);

        ++ito;
      }

      ito = o.begin();
      if (a > 0) {
        NI2 its = 0;

        OrdCT<NI2> g = lm.gain_();
        OrdCT<NI2>::iterator itg = g.begin();
        // For intervals in which size <= off ^ 2 (check reduceMapN, this intervals are not "reduced")
        for (int dim = 0; dim < res.ndim_(); ++dim) {
          if (*itg == 1 && *ito != 0) {
            BOOST_FOREACH (AtomSet asi, (*itdoms).asets_()) {
              MultiInterval mii = asi.aset_();
              OrdCT<Interval> ii = mii.inters_();
              OrdCT<Interval>::iterator itii = ii.begin();

              for (int count = 0; count < dim; ++count) ++itii;

              its = max(its, ceil(((*itii).hi_() - (*itii).lo_()) / abs(*ito)));
            }
          }

          // else
          //  ++maxit;

          ++itg;
          ++ito;
        }

        maxit += its;
      }

      else if (b == 0)
        ++maxit;

      ++itdoms;
    }

    if (maxit == 0)
      return res;

    else
      maxit = floor(log2(maxit)) + 1;

    for (int j = 0; j < maxit; ++j) res = res.compPW(res);
  }

  return res;
}

PWLMap minAdjCompMap(PWLMap pw2, PWLMap pw1)
{
  PWLMap res;

  OrdCT<Set> auxd = pw2.dom_();
  int auxsize = auxd.size();
  if (auxsize == 1) {
    Set dominv = pw2.image(*(pw2.dom_().begin()));
    LMap lminv = (*(pw2.lmap_().begin())).invlinearMap();

    PWLMap invpw;
    invpw.addSetLM(dominv, lminv);

    NI2 maxg = *(lminv.gain_().begin());
    NI2 ming = maxg;
    BOOST_FOREACH (NI2 gi, lminv.gain_()) {
      maxg = max(maxg, gi);
      ming = min(ming, gi);
    }

    if (maxg < Inf) {
      res = pw1.compPW(invpw);
    }

    else if (ming == Inf) {
      if (!pw2.empty()) {
        Set aux = pw1.image(*(pw2.dom_().begin()));
        OrdCT<NI1> minaux = aux.minElem();
        OrdCT<NI1>::iterator itminaux = minaux.begin();
        OrdCT<NI2> minaux2;
        OrdCT<NI2>::iterator itminaux2 = minaux2.begin();

        OrdCT<NI2> resg;
        OrdCT<NI2>::iterator itresg = resg.begin();
        for (unsigned int i = 0; i < minaux.size(); ++i) {
          itresg = resg.insert(itresg, 0);
          ++itresg;
          itminaux2 = minaux2.insert(itminaux2, (NI2)(*itminaux));
          ++itminaux2;

          ++itminaux;
        }

        LMap auxlm(resg, minaux2);
        res.addSetLM(dominv, auxlm);
      }
    }

    else {
      Set aux1 = pw1.image(*(pw2.dom_().begin()));
      OrdCT<NI1> minaux1 = aux1.minElem();
      OrdCT<NI1> minaux2 = (*(pw2.dom_().begin())).minElem();
      OrdCT<NI1>::iterator it2 = minaux2.begin();

      OrdCT<NI2> oi = lminv.off_();
      OrdCT<NI2>::iterator itoi = oi.begin();

      OrdCT<NI2> resg;
      OrdCT<NI2>::iterator itresg = resg.begin();
      OrdCT<NI2> reso;
      OrdCT<NI2>::iterator itreso = reso.begin();
      BOOST_FOREACH (NI2 gi, lminv.gain_()) {
        if (gi == Inf) {
          itresg = resg.insert(itresg, 0);
          itreso = reso.insert(itreso, (NI2)(*it2));
        }

        else {
          itresg = resg.insert(itresg, gi);
          itreso = reso.insert(itreso, *itoi);
        }

        ++itresg;
        ++itreso;
        ++it2;
        ++itoi;
      }

      LMap auxlm1(resg, reso);
      PWLMap auxinv;
      auxinv.addSetLM(dominv, auxlm1);

      PWLMap auxres = pw1.compPW(auxinv);
      if (!auxres.empty()) {
        Set domres = *(auxres.dom_().begin());
        LMap lmres = *(auxres.lmap_().begin());
        OrdCT<NI2> gres = lmres.gain_();
        OrdCT<NI2>::iterator itgres = gres.begin();
        oi = lmres.off_();
        itoi = oi.begin();

        OrdCT<NI2> resg2;
        OrdCT<NI2>::iterator itresg2 = resg2.begin();
        OrdCT<NI2> reso2;
        OrdCT<NI2>::iterator itreso2 = reso2.begin();
        OrdCT<NI1>::iterator it1 = minaux1.begin();
        BOOST_FOREACH (NI2 gi, lminv.gain_()) {
          if (gi == Inf) {
            itresg2 = resg2.insert(itresg2, 0);
            itreso2 = reso2.insert(itreso2, (NI2)(*it1));
          }

          else {
            itresg2 = resg2.insert(itresg2, *itgres);
            itreso2 = reso2.insert(itreso2, *itoi);
          }

          ++itresg2;
          ++itreso2;
          ++it1;
          ++itgres;
          ++itoi;
        }

        LMap auxlm2(resg2, reso2);
        res.addSetLM(domres, auxlm2);
      }
    }
  }

  // else
  // WARNING("There should be only one pair in the map");

  return res;
}

PWLMap minAdjMap(PWLMap pw2, PWLMap pw1)
{
  PWLMap res;

  if (!pw2.empty()) {
    OrdCT<Set> dom2 = pw2.dom_();
    OrdCT<Set>::iterator itdom2 = dom2.begin();
    OrdCT<LMap> lm2 = pw2.lmap_();
    OrdCT<LMap>::iterator itlm2 = lm2.begin();

    Set auxdom = *itdom2;
    LMap auxlm = *itlm2;

    PWLMap map1;
    map1.addSetLM(auxdom, auxlm);

    res = minAdjCompMap(map1, pw1);

    PWLMap minAdj;
    PWLMap minM;
    while (itdom2 != dom2.end()) {
      PWLMap mapi;
      mapi.addSetLM(*itdom2, *itlm2);
      minAdj = minAdjCompMap(mapi, pw1);
      minM = minMap(res, minAdj);

      res = minAdj.combine(res);

      if (!minM.empty()) res = minM.combine(res);

      ++itdom2;
      ++itlm2;
    }
  }

  return res;
}

SetEdge restrictEdge(SetEdge e, Set dom)
{
  PWLMap es1 = e.es1_();
  PWLMap es2 = e.es2_();

  PWLMap res1 = es1.restrictMap(dom);
  PWLMap res2 = es2.restrictMap(dom);

  SetEdge res(e.name, e.id_(), res1, res2, 0);
  return res;
}

}  // namespace SBG
