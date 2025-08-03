#include "LoxCallable.h"

namespace lox {

int LoxFunction::arity()
{
    return declaration->params->size();
}

any LoxFunction::call(Interpreter* interpreter, std::vector<any>& arguments)
{
    auto environment = std::make_shared<Environment>(interpreter->globals);
    for (int i = 0; i < declaration->params->size(); ++i) {
        environment->define(declaration->params->at(i).lexeme, arguments.at(i));
    }
    std::swap(environment, interpreter->environment);
    EnvironmentSwapGuard guard(interpreter->environment, environment);
    try {
        interpreter->executeBlock(declaration->body.get());
    }
    catch (const ReturnValue& returnValue) {
        return returnValue.value;
    }
    return any();
}

}