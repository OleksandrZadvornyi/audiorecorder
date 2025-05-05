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

public:
    explicit CommandInvoker(QObject* parent = nullptr)
        : QObject(parent)
    {}

    void executeCommand(std::shared_ptr<Command> command) {
        command->execute();

        m_history.push(command);
        emit commandExecuted(command->getName());
    }

    void clearHistory() {
        m_history.clear();
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
};

#endif // COMMANDINVOKER_H
