#include "KeyCommand.h"
#include "core/Logger.h"
KeyCommand::KeyCommand(Qt::Key key, std::function<void()> function, std::function<bool()> condition) : m_Key(key), m_Modifiers(Qt::NoModifier), m_Function(function), m_Condition(condition)
{
}

KeyCommand::KeyCommand(Qt::Key key, Qt::KeyboardModifiers modifiers, std::function<void()> function, std::function<bool()> condition) : m_Key(key), m_Modifiers(modifiers), m_Function(function), m_Condition(condition)
{
}

bool KeyCommand::Execute(const QKeyEvent &event)
{
    Logger::debug("KeyCommand::Execute：{}=={} {}=={}", event.key(), (int)m_Key, (int)event.modifiers(), (int)m_Modifiers);

    if (event.key() == m_Key && event.modifiers() == m_Modifiers)
    {
        if ((!m_Condition || m_Condition()) && m_Function)
        {
            m_Function();
            return true;
        }
    }
    return false;
}

KeyCommand::~KeyCommand()
{
    m_Function = nullptr;
}
