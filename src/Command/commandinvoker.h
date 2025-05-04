#ifndef COMMANDINVOKER_H
#define COMMANDINVOKER_H

#include "command.h"
#include <QObject>
#include <QStack>
#include <memory>

class CommandInvoker : public QObject
{
    Q_OBJECT

private:
    QStack<std::shared_ptr<Command>> m_history;
    QStack<std::shared_ptr<Command>> m_redoStack;
    bool m_recordCommands;

public:
    explicit CommandInvoker(QObject* parent = nullptr)
        : QObject(parent), m_recordCommands(true)
    {}

    void executeCommand(std::shared_ptr<Command> command) {
        command->execute();

        if (m_recordCommands) {
            m_history.push(command);
            // Clear redo stack when a new command is executed
            m_redoStack.clear();
            emit commandExecuted(command->getName());
        }
    }

    bool canUndo() const {
        return !m_history.isEmpty() && m_history.top()->canUndo();
    }

    void undo() {
        if (canUndo()) {
            auto command = m_history.pop();
            command->undo();
            m_redoStack.push(command);
            emit commandUndone(command->getName());
        }
    }

    bool canRedo() const {
        return !m_redoStack.isEmpty();
    }

    void redo() {
        if (canRedo()) {
            auto command = m_redoStack.pop();
            command->execute();
            m_history.push(command);
            emit commandRedone(command->getName());
        }
    }

    void clearHistory() {
        m_history.clear();
        m_redoStack.clear();
    }

    void setRecordCommands(bool record) {
        m_recordCommands = record;
    }

    QStringList getCommandHistory() const {
        QStringList history;
        for (const auto& command : m_history) {
            history.prepend(command->getName());
        }
        return history;
    }

signals:
    void commandExecuted(const QString& commandName);
    void commandUndone(const QString& commandName);
    void commandRedone(const QString& commandName);
};

#endif // COMMANDINVOKER_H
