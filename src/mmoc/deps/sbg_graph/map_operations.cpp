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

#include <deps/sbg_graph/container_def.h>
#include <deps/sbg_graph/graph.h>
#include <deps/sbg_graph/interval.h>
#include <deps/sbg_graph/map_operations.h>

namespace SB {

template <typename T>
size_t hash_value(SetVertex<T> v) { return v.hash(); }

// This operations are implemented here to simplificate the implementation.

PWLMap minAtomPW(AtomSet &dom, LMap &lm1, LMap &lm2)
{
  OrdCT<Real> g1 = lm1.slopes();
  OrdCT<Real> o1 = lm1.constants();
  OrdCT<Real>::iterator ito1 = o1.begin();
  OrdCT<Real> g2 = lm2.slopes();
  OrdCT<Real>::iterator itg2 = g2.begin();
  OrdCT<Real> o2 = lm2.constants();
  OrdCT<Real>::iterator ito2 = o2.begin();
  OrdCT<Interval> ints = dom.atomicSets().intervals();
  OrdCT<Interval>::iterator itints = ints.begin();

  AtomSet asAux = dom;
  LMap lmAux = lm1;
  OrdCT<Real> resg = g1;
  OrdCT<Real> reso = o1;
  int count = 1;

  OrdCT<Set> domRes;
  OrdCT<LMap> lmRes;

  if (lm1.dims() == lm2.dims()) {
    for (Real g1i : g1) {
      if (g1i != *itg2) {
        Real xinter = (*ito2 - *ito1) / (g1i - *itg2);

        // Intersection before domain
        if (xinter <= (*itints).lo()) {
          if (*itg2 < g1i) lmAux = lm2;

          Set sAux;
          sAux.addAtomSet(asAux);

          domRes.insert(domRes.begin(), sAux);
          lmRes.insert(lmRes.begin(), lmAux);
        } else if (xinter >= (*itints).hi()) { // Intersection after domain
          if (*itg2 > g1i) lmAux = lm2;

          Set sAux;
          sAux.addAtomSet(asAux);

          domRes.insert(domRes.begin(), sAux);
          lmRes.insert(lmRes.begin(), lmAux);
        } else { // Intersection in domain
          Interval i1((*itints).lo(), (*itints).step(), floor(xinter));
          Interval i2(i1.hi() + i1.step(), (*itints).step(), (*itints).hi());

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
          } else {
            lmRes.insert(lmRes.end(), lm2);
            lmRes.insert(lmRes.end(), lm1);
          }
        }

        PWLMap auxRes(domRes, lmRes);
        return auxRes;
      } else if (*ito1 != *ito2) {
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

  UnordCT<AtomSet> asets = dom.atomicSets();
  UnordCT<AtomSet>::iterator itas = asets.begin();

  if (!dom.empty()) {
    PWLMap aux;
    AtomSet asAux = *itas;
    aux = minAtomPW(asAux, lm1, lm2);
    if (!aux.empty()) {
      sres1 = *(aux.dom().begin());
      lres1 = *(aux.linearMap().begin());
      ++itas;

      OrdCT<Set> d;
      OrdCT<Set>::iterator itd;
      OrdCT<LMap> l;
      OrdCT<LMap>::iterator itl;
      while (itas != asets.end()) {
        asAux = *itas;
        aux = minAtomPW(asAux, lm1, lm2);
        d = aux.dom();
        itd = d.begin();
        l = aux.linearMap();
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

  OrdCT<LMap> lm1 = pw1.linearMap();
  OrdCT<LMap>::iterator itl1 = lm1.begin();
  OrdCT<LMap> lm2 = pw2.linearMap();

  if (!pw1.empty() && !pw2.empty()) {
    for (Set s1i : pw1.dom()) {
      OrdCT<LMap>::iterator itl2 = lm2.begin();

      for (Set s2j : pw2.dom()) {
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
    OrdCT<Set> auxdom = pw.dom();
    OrdCT<Set>::iterator itdom = auxdom.begin();
    OrdCT<LMap> auxlm = pw.linearMap();
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
  OrdCT<Set> sres = pw.dom();
  OrdCT<Set>::iterator itsres = sres.end();
  OrdCT<LMap> lres = pw.linearMap();
  OrdCT<LMap>::iterator itlres = lres.end();

  OrdCT<LMap> lm = pw.linearMap();
  OrdCT<LMap>::iterator itlm = lm.begin();

  unsigned int i = 1;
  for (Set di : pw.dom()) {
    int count1 = 1;

    OrdCT<Real> g = (*itlm).slopes();
    OrdCT<Real>::iterator itg = g.begin();
    OrdCT<Real> o = (*itlm).constants();
    OrdCT<Real>::iterator ito = o.begin();
    // Get the dim-th gain and offset
    while (count1 < dim) {
      ++itg;
      ++ito;
      ++count1;
    }

    if (*itg == 1 && *ito < 0) {
      Real off = -(*ito);

      for (AtomSet adom : di.atomicSets()) {
        MultiInterval mi = adom.atomicSets();
        OrdCT<Interval> inters = mi.intervals();
        OrdCT<Interval>::iterator itints = inters.begin();

        int count2 = 1;
        while (count2 < dim) {
          ++itints;
          ++count2;
        }

        Integer loint = (*itints).lo();
        Integer hiint = (*itints).hi();

        if ((hiint - loint) > (off * off)) {
          OrdCT<Set> news;
          OrdCT<Set>::iterator itnews = news.begin();
          OrdCT<LMap> newl;
          OrdCT<LMap>::iterator itnewl = newl.begin();

          for (int k = 1; k <= off; k++) {
            OrdCT<Real> newo = (*itlm).constants();
            OrdCT<Real>::iterator itnewo = newo.begin();

            OrdCT<Real> resg;
            OrdCT<Real>::iterator itresg = resg.begin();
            OrdCT<Real> reso;
            OrdCT<Real>::iterator itreso = reso.begin();

            int count3 = 1;
            for (Real gi : (*itlm).slopes()) {
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
          for (AtomSet auxasi : di.atomicSets()) {
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
              for (Set si : sres) {
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
              for (Set si : sres) {
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

          for (Set newi : newmap.dom()) {
            itsres = sres.insert(itsres, newi);
            ++itsres;
          }

          for (LMap newi : newmap.linearMap()) {
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

    for (int i = 2; i <= res.dims(); ++i) res = reduceMapN(res, i);

    int maxit = 0;

    OrdCT<Set> doms = res.dom();
    OrdCT<Set>::iterator itdoms = doms.begin();
    for (LMap lm : res.linearMap()) {
      OrdCT<Real> o = lm.constants();
      OrdCT<Real>::iterator ito = o.begin();

      Real a = 0;
      Real b = *(lm.slopes().begin());

      for (Real gi : lm.slopes()) {
        a = max(a, gi * abs(*ito));
        b = min(b, gi);

        ++ito;
      }

      ito = o.begin();
      if (a > 0) {
        Real its = 0;

        OrdCT<Real> g = lm.slopes();
        OrdCT<Real>::iterator itg = g.begin();
        // For intervals in which size <= off ^ 2 (check reduceMapN, this intervals are not "reduced")
        for (int dim = 0; dim < res.dims(); ++dim) {
          if (*itg == 1 && *ito != 0) {
            for (AtomSet asi : (*itdoms).atomicSets()) {
              MultiInterval mii = asi.atomicSets();
              OrdCT<Interval> ii = mii.intervals();
              OrdCT<Interval>::iterator itii = ii.begin();

              for (int count = 0; count < dim; ++count) ++itii;

              its = max(its, ceil(((*itii).hi() - (*itii).lo()) / abs(*ito)));
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

  OrdCT<Set> auxd = pw2.dom();
  int auxsize = auxd.size();
  if (auxsize == 1) {
    Set dominv = pw2.image(*(pw2.dom().begin()));
    LMap lminv = (*(pw2.linearMap().begin())).invlinearMap();

    PWLMap invpw;
    invpw.addSetLM(dominv, lminv);

    Real maxg = *(lminv.slopes().begin());
    Real ming = maxg;
    for (Real gi : lminv.slopes()) {
      maxg = max(maxg, gi);
      ming = min(ming, gi);
    }

    if (maxg < Inf) {
      res = pw1.compPW(invpw);
    }

    else if (ming == Inf) {
      if (!pw2.empty()) {
        Set aux = pw1.image(*(pw2.dom().begin()));
        OrdCT<Integer> minaux = aux.minElem();
        OrdCT<Integer>::iterator itminaux = minaux.begin();
        OrdCT<Real> minaux2;
        OrdCT<Real>::iterator itminaux2 = minaux2.begin();

        OrdCT<Real> resg;
        OrdCT<Real>::iterator itresg = resg.begin();
        for (unsigned int i = 0; i < minaux.size(); ++i) {
          itresg = resg.insert(itresg, 0);
          ++itresg;
          itminaux2 = minaux2.insert(itminaux2, (Real)(*itminaux));
          ++itminaux2;

          ++itminaux;
        }

        LMap auxlm(resg, minaux2);
        res.addSetLM(dominv, auxlm);
      }
    }

    else {
      Set aux1 = pw1.image(*(pw2.dom().begin()));
      OrdCT<Integer> minaux1 = aux1.minElem();
      OrdCT<Integer> minaux2 = (*(pw2.dom().begin())).minElem();
      OrdCT<Integer>::iterator it2 = minaux2.begin();

      OrdCT<Real> oi = lminv.constants();
      OrdCT<Real>::iterator itoi = oi.begin();

      OrdCT<Real> resg;
      OrdCT<Real>::iterator itresg = resg.begin();
      OrdCT<Real> reso;
      OrdCT<Real>::iterator itreso = reso.begin();
      for (Real gi : lminv.slopes()) {
        if (gi == Inf) {
          itresg = resg.insert(itresg, 0);
          itreso = reso.insert(itreso, (Real)(*it2));
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
        Set domres = *(auxres.dom().begin());
        LMap lmres = *(auxres.linearMap().begin());
        OrdCT<Real> gres = lmres.slopes();
        OrdCT<Real>::iterator itgres = gres.begin();
        oi = lmres.constants();
        itoi = oi.begin();

        OrdCT<Real> resg2;
        OrdCT<Real>::iterator itresg2 = resg2.begin();
        OrdCT<Real> reso2;
        OrdCT<Real>::iterator itreso2 = reso2.begin();
        OrdCT<Integer>::iterator it1 = minaux1.begin();
        for (Real gi : lminv.slopes()) {
          if (gi == Inf) {
            itresg2 = resg2.insert(itresg2, 0);
            itreso2 = reso2.insert(itreso2, (Real)(*it1));
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
  return res;
}

PWLMap minAdjMap(PWLMap pw2, PWLMap pw1)
{
  PWLMap res;

  if (!pw2.empty()) {
    OrdCT<Set> dom2 = pw2.dom();
    OrdCT<Set>::iterator itdom2 = dom2.begin();
    OrdCT<LMap> lm2 = pw2.linearMap();
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

template<typename E>
SetEdge<E> restrictEdge(SetEdge<E> e, Set dom)
{
  PWLMap es1 = e.mapF();
  PWLMap es2 = e.mapU();

  PWLMap res1 = es1.restrictMap(dom);
  PWLMap res2 = es2.restrictMap(dom);

  SetEdge res(e.name(), e.id(), res1, res2, 0);
  return res;
}

} // namespace SB
