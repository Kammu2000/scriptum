#include <fstream>
#include <print>
#include <sstream>
#include <stdexcept>

#include <scriptum/frontend/parser/program_parser.hpp>
#include <scriptum/runtime/environment.hpp>
#include <scriptum/runtime/interpreter.hpp>
#include <scriptum/stdlib/globals.hpp>

namespace {

std::string readFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::println(stderr, "Usage: scriptum <script-file>");
        return 1;
    }

    try
    {
        const std::string code = readFile(argv[1]);
        scriptum::ProgramParser parser;
        const scriptum::Program ast = parser.buildAST(code);

        scriptum::Environment env(nullptr);
        scriptum::loadStdlib(env);
        scriptum::run(ast, &env);
    }
    catch (const std::exception& ex)
    {
        std::println(stderr, "Error: {}", ex.what());
        return 1;
    }

    return 0;
}
