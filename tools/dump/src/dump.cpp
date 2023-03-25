/****************************************************************************
MIT License

Copyright (c) 2023 Juan Medina

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
****************************************************************************/

#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>

auto main(int argc, char *argv[]) -> int {
    // get arguments
    auto args = std::span(argv, static_cast<size_t>(argc));

    // get executable path
    auto full_path = std::filesystem::path{args[0]};
    auto executable_path = full_path.parent_path();
    std::cout << "Executable path: " << executable_path.string() << std::endl;
    auto name = full_path.filename().string();

    if(args.size() != 3) {
        std::cout << "Usage: " << name << " <file1> <file2>" << std::endl;
        return EXIT_FAILURE;
    }

    // get relative path
    auto file1 = std::filesystem::path{args[1]};
    auto origin_path = executable_path / file1;

    // check if file1 exists
    if(!std::filesystem::exists(origin_path)) {
        std::cout << "File " << args[1] << " does not exist" << std::endl;
        return EXIT_FAILURE;
    }

    auto file2 = std::filesystem::path{args[2]};
    auto destination_path = executable_path / file2;

    std::cout << "Origin path: " << origin_path.string() << std::endl;
    std::cout << "Destination path: " << destination_path.string() << std::endl;

    // dump file1 in hex into file2
    std::cout << "Dumping " << args[1] << " into " << args[2] << std::endl;

    auto output_file = std::ofstream{destination_path, std::ios::binary};
    if(!output_file.is_open()) {
        std::cout << "Could not open " << args[2] << std::endl;
        return EXIT_FAILURE;
    }

    auto input_file = std::ifstream{origin_path, std::ios::binary};
    if(!input_file.is_open()) {
        std::cout << "Could not open " << args[1] << std::endl;
        return EXIT_FAILURE;
    }

    // license string literal
    const auto *const license = R"(/****************************************************************************
The MIT License (MIT)

Copyright (c) 2023 Juan Medina

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
****************************************************************************/)";

    // output license
    output_file << license << std::endl;
    output_file << std::endl;

    output_file << "#pragma once" << std::endl;
    output_file << std::endl;

    output_file << "namespace sneze::embedded {" << std::endl;
    output_file << std::endl;

    // output file
    output_file << "#include <array>" << std::endl;
    output_file << std::endl;
    auto count = 0;

    if(input_file.is_open()) {
        const auto total_bytes = std::filesystem::file_size(origin_path);
        const auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto buffer = std::array<char, 80>{};
        std::strftime(buffer.data(), buffer.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&time));

        output_file << "// " << file1.filename().string() << " size: " << total_bytes / 1024 << " kb "
                    << "generated on: " << buffer.data() << std::endl;
        output_file << "static constexpr std::array<unsigned char," << total_bytes << "> "
                    << file2.filename().stem().string() << " {" << std::endl;
        output_file << "    ";
        while(!input_file.eof()) {
            auto byte = input_file.get();
            if(!input_file.eof()) {
                output_file << "0x" << std::hex << std::setw(2) << std::setfill('0') << byte << ", ";
                count++;
                if(count % 16 == 0) {
                    output_file << std::endl << "    ";
                }
            }
        }
    }
    output_file << std::endl << "};" << std::endl;
    output_file << std::endl;
    output_file << "}// namespace sneze::embedded" << std::endl;

    return EXIT_SUCCESS;
}
