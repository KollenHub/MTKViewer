#pragma once
#include <functional>
#include <QObject>
#include <QKeyEvent>

class KeyCommand
{
private:
    std::function<void()> m_Function;
    std::function<bool()> m_Condition;
    Qt::Key m_Key;
    Qt::KeyboardModifiers m_Modifiers;

public:
    KeyCommand(Qt::Key key, std::function<void()> function, std::function<bool()> condition = nullptr);

    KeyCommand(Qt::Key key, Qt::KeyboardModifiers modifiers, std::function<void()> function, std::function<bool()> condition = nullptr);

    bool Execute(const QKeyEvent &event);

    ~KeyCommand();
};
