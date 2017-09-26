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

#ifndef EDITOR_H_
#define EDITOR_H_

#include <QtGui>
#include <modeleditor.h>

/**
 *
 */
class Editor
{
  public:
    /**
     *
     * @param parent
     * @param name
     * @return
     */
    static ModelEditor*
    instance(QWidget *parent = NULL, QString name = QString())
    {
      if(!_instance)
      {
        _instance = new ModelEditor(parent, name);
      }
      return _instance;
    }
    /**
     *
     */
    static void
    drop()
    {
      if(!_instance)
        delete _instance;
      _instance = NULL;
    }
  private:
    Editor()
    {
    }
    Editor(const Editor &); // hide copy constructor
    Editor&
    operator=(const Editor &); // hide assign op
    static ModelEditor* _instance;
};
#endif  /* EDITOR_H_ */
