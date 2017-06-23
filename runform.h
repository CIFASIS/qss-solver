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

#ifndef RUNFORM_H_
#define RUNFORM_H_

#include <QtGui>
#include "./ui/ui_run.h"
#include "utils.h"

/**
 *
 */
class RunDlg : public QDialog, public Ui::RunForm
{
    Q_OBJECT
public:
    /**
     *
     * @param parent
     */
    RunDlg (QWidget *parent = NULL);
    /**
     *
     */
    ~RunDlg ();
    /**
     *
     * @return
     */
    QString
    absTolerance ()
    {
        return (_absTolerance->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    derDelta ()
    {
        return (_derDelta->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    DT ()
    {
        return (_dt->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    description ()
    {
        return (_description->toPlainText ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    minStep ()
    {
        return (_minStep->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    output ()
    {
        return (_output->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    outputType ()
    {
        return (_getOutputTypeString (_commInterval->currentIndex ()));
    }
    ;
    /**
     *
     * @return
     */
    QString
    period ()
    {
        return (_period->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    scheduler ()
    {
        return (_getSchedulerString (_scheduler->currentIndex ()));
    }
    ;
    /**
     *
     * @return
     */
    QString
    solver ()
    {
        return (_getSolverString (_solver->currentIndex ()));
    }
    ;
    /**
     *
     * @return
     */
    QString
    startTime ()
    {
        return (_startTime->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    stopTime ()
    {
        return (_stopTime->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    symDiff ()
    {
        return (_getSymDiffString (_symDiff->currentIndex ()));
    }
    ;
    /**
     *
     * @return
     */
    QString
    jacobian ()
    {
        return (_getJacobianString (_jacobian->currentIndex ()));
    }
    ;
    /**
     *
     * @return
     */
    QString
    tolerance ()
    {
        return (_tolerance->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    zcHyst ()
    {
        return (_zcHyst->text ());
    }
    ;
    /**
     *
     * @return
     */
    QString
    LPS ()
    {
        return (_lps->text ());
    }
    ;
    /**
     *
     * @param str
     */
    void
    setAbsTolerance (QString str)
    {
        _absTolerance->setText (str.remove ("{").remove ("}"));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setDerDelta (QString str)
    {
        _derDelta->setText (str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setDescription (QString str)
    {
        _description->setText (str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setMinStep (QString str)
    {
        _minStep->setText (str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setDT (QString str)
    {
        _dt->setText (str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setOutput (QString str)
    {
        _output->setText (str.remove ("{").remove ("}"));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setOutputType (QString str)
    {
        _commInterval->setCurrentIndex (_getOutputTypeIdx (str));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setPeriod (QString str)
    {
        _period->setText (str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setScheduler (QString str)
    {
        _scheduler->setCurrentIndex (_getSchedulerIdx (str));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setSolver (QString str)
    {
        _solver->setCurrentIndex (_getSolverIdx (str));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setStartTime (QString str)
    {
        _startTime->setText (str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setStopTime (QString str)
    {
        _stopTime->setText (str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setSymDiff (QString str)
    {
        _symDiff->setCurrentIndex (_getSymDiffIdx (str));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setTolerance (QString str)
    {
        _tolerance->setText (str.remove ("{").remove ("}"));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setZcHyst (QString str)
    {
        _zcHyst->setText (str);
    }
    ;
    /**
     *
     * @param str
     */
    void
    setLPS (QString str)
    {
        _lps->setText (str);
    }
    ;
    void
    setJacobian (QString str)
    {
        _jacobian->setCurrentIndex (str == "Sparse" ? 0 : 1);
    }
    ;

    /**
     *
     * @return
     */
    QString
    parallel ()
    {
        return (_getParallelString (_parallel->currentIndex ()));
    }
    ;
    /**
     *
     * @return
     */
    QString
    partitionMethod ()
    {
        return (_getPartitionMethodString (_partitionMethod->currentIndex ()));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setParallel (QString str)
    {
        _parallel->setCurrentIndex (_getParallelIdx (str));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setPartitionMethod (QString str)
    {
        _partitionMethod->setCurrentIndex (_getPartitionMethodIdx (str));
    }
    ;
    /**
     *
     * @return
     */
    QString
    dtSynch ()
    {
        return (_getDtSynchString (_dtSynch->currentIndex ()));
    }
    ;
    /**
     *
     * @param str
     */
    void
    setDtSynch (QString str)
    {
        _dtSynch->setCurrentIndex (_getDtSynchIdx (str));
    }
    ;
    /**
     *
     * @return
     */
    bool
    enableDebug ();
    QString
    patohSettings ()
    {
        return (_patohSettings->text ());
    }
    ;
    void
    setPatohSettings (QString str)
    {
        _patohSettings->setText (str.remove ("{").remove ("}"));
    }
    ;
    QString
    scotchSettings ()
    {
        return (_scotchSettings->text ());
    }
    ;
    void
    setScotchSettings (QString str)
    {
        _scotchSettings->setText (str.remove ("{").remove ("}"));
    }
    ;
    QString
    metisSettings ()
    {
        return (_metisSettings->text ());
    }
    ;
    void
    setMetisSettings (QString str)
    {
        _metisSettings->setText (str.remove ("{").remove ("}"));
    }
    ;
    bool
    semiStaticPartitioning ();
    void
    setSemiStaticPartitioning (bool st);private slots:
    void
    on__commInterval_currentIndexChanged (int index);
    void
    on__parallel_currentIndexChanged (int index);
    void
    on__showAll_stateChanged (int state);
    void
    on__dtSynch_currentIndexChanged (int index);
private:
    int
    _getOutputTypeIdx (QString str);
    QString
    _getOutputTypeString (int idx);
    int
    _getSchedulerIdx (QString str);
    QString
    _getSchedulerString (int idx);
    int
    _getSolverIdx (QString str);
    QString
    _getSolverString (int idx);
    int
    _getSymDiffIdx (QString str);
    QString
    _getSymDiffString (int idx);
    QString
    _getJacobianString (int idx);
    int
    _getParallelIdx (QString str);
    QString
    _getParallelString (int idx);
    int
    _getPartitionMethodIdx (QString str);
    QString
    _getPartitionMethodString (int idx);
    int
    _getDtSynchIdx (QString str);
    int
    _getJacobianIdx (QString str);
    QString
    _getDtSynchString (int idx);
    Utils *_utils;
    QDoubleValidator *_validate;
    QIntValidator *_validateInt;
};

#endif  /* RUNFORM_H_ */
