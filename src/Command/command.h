#ifndef COMMAND_H
#define COMMAND_H

#include <QString>

// Command interface
class Command
{
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual bool canUndo() const { return false; }
    virtual void undo() {}
    virtual QString getName() const = 0;
};

#endif // COMMAND_H
