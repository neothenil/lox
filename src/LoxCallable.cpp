#include "LoxCallable.h"

namespace lox {

int LoxFunction::arity()
{
    return declaration->params->size();
}

any LoxFunction::call(Interpreter* interpreter, std::vector<any>& arguments)
{
    auto environment = std::make_unique<Environment>(Interpreter::globals());
    for (int i = 0; i < declaration->params->size(); ++i) {
        environment->define(declaration->params->at(i).lexeme, arguments.at(i));
    }
    std::swap(interpreter->environment, environment);
    // make sure the environment will be restored even if exception is raised.
    EnvironmentSwapGuard guard(interpreter->environment, environment);
    interpreter->executeBlock(declaration->body.get());
    return any();
}

}