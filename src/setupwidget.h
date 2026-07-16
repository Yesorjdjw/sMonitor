#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QScrollArea>

class ConfigManager;
class MqttManager;
namespace Ui { class SetupWidget; }

class SetupWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit SetupWidget(ConfigManager *cfg, QWidget *parent = nullptr);
    ~SetupWidget();

    void setMqttManager(MqttManager *mqtt);

private slots:
    void onSave();
    void onReset();

private:
    void loadConfig();
    void applyTtsLoopLinkage();

    Ui::SetupWidget *ui;
    ConfigManager *m_cfg;
    MqttManager *m_mqtt = nullptr;
};

#endif // SETUPWIDGET_H