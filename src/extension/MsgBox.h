#pragma once
#include <QMessageBox>
#include <QAbstractButton>
#include <QMap>
#include <QString>
class MsgBox
{
public:
    static QMap<QMessageBox::StandardButton, QString> s_ButtonTextMap;

    static QMessageBox::StandardButton Show(QMessageBox::Icon icon, const QString &title, const QString &text,
                                            QMessageBox::StandardButtons buttons = QMessageBox::NoButton, QWidget *parent = nullptr)
    {
        QMessageBox msgBox(icon, title, text, buttons, parent);

        static QMap<QMessageBox::StandardButton, QString> buttonTexts = {
            {QMessageBox::Ok, "确定"},
            {QMessageBox::Cancel, "取消"},
            {QMessageBox::Close, "关闭"},
            {QMessageBox::Abort, "终止"},
            {QMessageBox::Ignore, "忽略"},
            {QMessageBox::No, "否"},
            {QMessageBox::NoToAll, "全部否"},
            {QMessageBox::Open, "打开"},
            {QMessageBox::Reset, "重置"},
            {QMessageBox::RestoreDefaults, "恢复默认"},
            {QMessageBox::Save, "保存"},
            {QMessageBox::SaveAll, "全部保存"},
            {QMessageBox::Yes, "是"},
            {QMessageBox::YesToAll, "全部是"},
            {QMessageBox::Apply, "应用"},
            {QMessageBox::Discard, "放弃"},
            {QMessageBox::Retry, "重试"}};

        // 循环设置按钮文本，自动处理空指针
        auto it = buttonTexts.constBegin();
        while (it != buttonTexts.constEnd())
        {
            if (QAbstractButton *button = msgBox.button(it.key()))
            {
                button->setText(QObject::tr(it.value().toUtf8().constData()));
            }
            ++it;
        }
        msgBox.exec();

        return msgBox.standardButton(msgBox.clickedButton());
    }

    static QMessageBox::StandardButton information(QWidget *parent, const QString &title, const QString &text,
                                                   QMessageBox::StandardButtons buttons = QMessageBox::Ok)
    {
        return Show(QMessageBox::Information, title, text, buttons, parent);
    }

    static QMessageBox::StandardButton warning(QWidget *parent, const QString &title, const QString &text,
                                               QMessageBox::StandardButtons buttons = QMessageBox::Ok)
    {
        return Show(QMessageBox::Warning, title, text, buttons, parent);
    }

    static QMessageBox::StandardButton critical(QWidget *parent, const QString &title, const QString &text,
                                                QMessageBox::StandardButtons buttons = QMessageBox::Ok)
    {
        return Show(QMessageBox::Critical, title, text, buttons, parent);
    }

    static QMessageBox::StandardButton question(QWidget *parent, const QString &title, const QString &text,
                                                QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No)
    {
        return Show(QMessageBox::Question, title, text, buttons, parent);
    }

    static QMessageBox::StandardButton confirm(QWidget *parent, const QString &title, const QString &text,
                                               QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No)
    {
        return Show(QMessageBox::Question, title, text, buttons, parent);
    }
};