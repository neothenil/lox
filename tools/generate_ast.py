#!/usr/bin/env python3
import sys
from pathlib import Path

VISIT_FUNC_TEMPLATE = \
    "    virtual any visit{sub}{base}({sub}* {lowerBase}) = 0;\n"
VISITOR_TEMPLATE = """class {baseName}Visitor
{{
public:
    virtual ~{baseName}Visitor() = default;

{functions}}};
"""
BASE_TEMPLATE = """class {base}
{{
public:
    virtual ~{base}() = default;

    virtual any accept({base}Visitor* visitor) = 0;
}};
"""
PARAM_TEMPLATE = "std::unique_ptr<{type}> {name}, "
FIELD_TEMPLATE = "    std::unique_ptr<{type}> {name};\n"
INIT_TEMPLATE = "{name}(std::move({name})), "
CTOR_TEMPLATE = "    {sub}({params}): {base}(), {initializer} {{}}"
SUB_TEMPLATE = """class {sub}: public {base}
{{
public:
{constructor}
    ~{sub}() override = default;

    any accept({base}Visitor* visitor) override
    {{ return visitor->visit{sub}{base}(this); }}

{fields}
}};

"""
FORWARD_TEMPLATE = "class {name};\n"
HEADER_TEMPLATE = """#pragma once

#include <memory>
#include <vector>
#include <any>

#include "Scanner.h"

namespace lox {{

using any = std::any;

{forwards}
{visitor}
{baseclass}
{subclasses}}}"""


def defineType(className: str, baseName: str,
               fields: list[tuple[str, ...]]) -> str:
    fieldLines = ""
    params = ""
    initializer = ""
    for (type, name) in fields:
        fieldLines += FIELD_TEMPLATE.format(type=type, name=name)
        params += PARAM_TEMPLATE.format(type=type, name=name)
        initializer += INIT_TEMPLATE.format(name=name)
    if fields:
        fieldLines = fieldLines[:-1]
        params = params[:-2]
        initializer = initializer[:-2]
    constructor = CTOR_TEMPLATE.format(sub=className, params=params,
                                       base=baseName, initializer=initializer)
    return SUB_TEMPLATE.format(sub=className, base=baseName,
                               constructor=constructor,
                               fields=fieldLines)


def defineAst(outputDir: Path, baseName: str, types: list[str]):
    header = outputDir / (baseName + ".h")
    baseclass = BASE_TEMPLATE.format(base=baseName)
    subclasses = ""
    visitFuncs = ""
    forwards = FORWARD_TEMPLATE.format(name=baseName)
    for type in types:
        className = type.split(":")[0].strip()
        fields = type.split(":")[1].split(",")
        fields = list(map(lambda x: tuple(x.strip().split()), fields))
        forwards += FORWARD_TEMPLATE.format(name=className)
        subclasses += defineType(className, baseName, fields)
        visitFuncs += VISIT_FUNC_TEMPLATE.format(
            base=baseName, sub=className, lowerBase=baseName.lower())
    visitor = VISITOR_TEMPLATE.format(baseName=baseName, functions=visitFuncs)
    text = HEADER_TEMPLATE.format(forwards=forwards, visitor=visitor,
                                  baseclass=baseclass, subclasses=subclasses)
    header.write_text(text, encoding="utf-8")


def main():
    if (len(sys.argv) != 2):
        print("Usage: ./generate_ast.py <output directory>", file=sys.stderr)
        sys.exit(64)
    outputDir = Path(sys.argv[1])
    defineAst(outputDir, "Expr", [
        "Binary   : Expr left, Token op, Expr right",
        "Grouping : Expr expression",
        "Literal  : any value",
        "Unary    : Token op, Expr right"
    ])


if __name__ == "__main__":
    main()
