#ifndef SETUP_H
#define SETUP_H

#include <QWidget>
#include "steng.h"
#include <QTimer>

namespace Ui { class setup; }

class setup : public QWidget
{
    Q_OBJECT
public:
    explicit setup(QWidget *parent = 0);
    ~setup();
    void setEngPtr(setEng *p) { m_eng = p; }
    void setRecordingState(bool recording);

signals:
    void autoRecordToggled(bool on);

private slots:
    void on_backBt_clicked();
    void on_nav1_clicked(); void on_nav2_clicked(); void on_nav3_clicked();
    void on_nav4_clicked(); void on_nav5_clicked();
    void on_saveBt_clicked(); void on_resetBt_clicked();
    void on_speedChanged(int val);
    void on_ckDoor_clicked(); void on_ckWarehouse_clicked();
    void on_ckParking_clicked(); void on_ckWall_clicked();
    void onToggleCruise(bool checked); void onCruiseTick();
    /* 录像存储设置 */
    void on_browseBt_clicked();
    void on_autoToggle_clicked(bool checked);
    void on_saveBtRec_clicked();
    void on_resetBtRec_clicked();
    void loadStorageConfig();

private:
    Ui::setup *ui;
    setEng *m_eng;
    struct Preset { int lr, ud; bool enabled; } m_presets[4];
    int  m_cruiseIndex, m_cruiseCount, m_cruiseSpeed;
    QTimer *m_cruiseTimer;
    bool m_cruiseRunning;
    void startCruise(); void stopCruise();
    void selectCategory(int index); void clearSelection();

    /* 存储设置 */
    QString m_storagePath;
    bool    m_autoRecord;
    int     m_retainDays;
    int     m_diskFullPolicy;

    void saveStorageConfig();
    void refreshStorageInfo();
    static QString fmtSize(qint64 bytes);
    static qint64 dirSize(const QString &path);
};

#endif // SETUP_H
