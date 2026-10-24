#include <print>
#include <string>
#include <vector>

#include "arg_parser.hpp"

namespace {

using lovebird::tools::ArgParser;

struct LbcArgs {
    bool help = false;
    std::vector<std::string> files;
};

ArgParser make_arg_parser(LbcArgs& args) {
    ArgParser arg_parser{"lbc", "The Lovebird compiler"};
    arg_parser.positional("file", args.files);
    arg_parser.flag("--help", "-h", "show the help", args.help);
    return arg_parser;
}

}  // namespace

int main(int argc, char* argv[]) {
    LbcArgs args;

    auto parser = make_arg_parser(args);
    auto res = parser.parse(argc, argv);

    if (args.help || argc == 1 || !res) {
        std::println("{}", parser.help());
    }

    if (!res) {
        std::println("invalid argument: {}", res.error().arg);
        return 1;
    }

    return 0;
}
