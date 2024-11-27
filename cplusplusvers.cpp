#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include <string> 

#define MAX_COMMAND_LENGTH 2048 

void error(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    exit(EXIT_FAILURE);
} 

void compile_and_link_files(const std::vector<std::string>& filenames,
                           const std::string& output_executable,
                           const std::string& output_assembly,
                           const std::string& output_binary,
                           const std::string& output_hex) {
    std::string command; 

    // Compile source files
    command = "arm-none-eabi-gcc -c ";
    for (const auto& filename : filenames) {
        command += filename + " ";
    }
    command += "-specs=nosys.specs";
    std::cout << "Compiling with command: " << command << std::endl;
    int result = system(command.c_str());
    if (result != 0) {
        error("Compilation failed. Please check your code.");
    } 

    // Link object files to create executable
    command = "arm-none-eabi-gcc -o " + output_executable + " ";
    for (const auto& filename : filenames) {
        std::string object_file = filename.substr(0, filename.find_last_of('.')) + ".o";
        command += object_file + " ";
    }
    command += "-specs=nosys.specs";
    std::cout << "Linking with command: " << command << std::endl;
    result = system(command.c_str());
    if (result != 0) {
        error("Linking failed. Please check your code.");
    } 

    // Generate ARM assembly code
    command = "arm-none-eabi-gcc -S -o " + output_assembly + " ";
    for (const auto& filename : filenames) {
        std::string object_file = filename.substr(0, filename.find_last_of('.')) + ".o";
        command += object_file + " ";
    }
    std::cout << "Generating ARM assembly with command: " << command << std::endl;
    result = system(command.c_str());
    if (result != 0) {
        error("Failed to generate ARM assembly code.");
    } 

    // Generate binary file
    command = "arm-none-eabi-objcopy -O binary " + output_executable + " " + output_binary;
    std::cout << "Generating binary file with command: " << command << std::endl;
    result = system(command.c_str());
    if (result != 0) {
        error("Failed to generate binary file.");
    } 

    // Generate hex file
    command = "arm-none-eabi-objcopy -O ihex " + output_executable + " " + output_hex;
    std::cout << "Generating hex file with command: " << command << std::endl;
    result = system(command.c_str());
    if (result != 0) {
        error("Failed to generate hex file.");
    } 

    std::cout << "Compilation and linking successful! Executable: " << output_executable << std::endl;
    std::cout << "ARM assembly code generated: " << output_assembly << std::endl;
    std::cout << "Binary file generated: " << output_binary << std::endl;
    std::cout << "Hex file generated: " << output_hex << std::endl;
} 

int main() {
    std::vector<std::string> filenames;
    std::string filename; 

    std::cout << "Enter your C source and header file names (type 'END' to finish):\n"; 

    while (true) {
        std::cin >> filename;
        if (filename == "END") {
            break;
        }
        filenames.push_back(filename);
    } 

    if (filenames.empty()) {
        error("No files provided for compilation.");
    } 

    const std::string output_executable = "output_program";
    const std::string output_assembly = "output_program.s";
    const std::string output_binary = "output_program.bin";
    const std::string output_hex = "output_program.hex"; 

    compile_and_link_files(filenames, output_executable, output_assembly, output_binary, output_hex); 

    return 0;
}
