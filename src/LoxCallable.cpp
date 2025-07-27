#include "LoxCallable.h"

namespace lox {

int LoxFunction::arity()
{
    return declaration->params->size();
}

any LoxFunction::call(Interpreter* interpreter, std::vector<any>& arguments)
{
    interpreter->environment = std::make_unique<Environment>(
        std::move(interpreter->environment));
    for (int i = 0; i < declaration->params->size(); ++i) {
        interpreter->environment->define(
            declaration->params->at(i).lexeme, arguments.at(i));
    }
    EnvironmentGuard guard(interpreter->environment);
    interpreter->executeBlock(declaration->body.get());
    return any();
}

}