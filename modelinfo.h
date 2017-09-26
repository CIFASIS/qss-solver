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

#ifndef MODELINFO_H_
#define MODELINFO_H_

#include <QtGui>

/**
 *
 */
class ModelInfo
{
  public:
    /**
     *
     * @param fullname
     */
    ModelInfo(QString fullname = QString());
    /**
     *
     */
    ~ModelInfo();
    /**
     *
     * @return
     */
    QString
    baseName();
    /**
     *
     * @return
     */
    bool
    dirty()
    {
      return _dirty;
    }
    ;
    /**
     *
     * @return
     */
    QString
    fullname()
    {
      return _fullname;
    }
    ;
    /**
     *
     * @return
     */
    bool
    init()
    {
      return _init;
    }
    ;
    /**
     *
     * @return
     */
    QString
    name();
    /**
     *
     * @return
     */
    QString
    path();
    /**
     *
     * @param d
     */
    void
    setDirty(bool d)
    {
      _dirty = d;
    }
    ;
    /**
     *
     * @param fn
     */
    void
    setFullname(QString fn)
    {
      _fullname = fn;
    }
    ;
    /**
     *
     * @param d
     */
    void
    setInit(bool d)
    {
      _init = d;
    }
    ;
    private:
    bool _dirty;
    QString _fullname;
    bool _init;
};

#endif  /* MODELINFO_H_ */
