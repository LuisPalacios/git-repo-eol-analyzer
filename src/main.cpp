#include <filesystem>
#include <iostream>
#include "eol_analyzer.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
  std::cout << "git-repo-eol-analyzer (Compiled w/ CLANG-17 - ";
#if __cplusplus == 199711L
  std::cout << "standard C++98)" << std::endl;
#elif __cplusplus == 201103L
  std::cout << "standard C++11)" << std::endl;
#elif __cplusplus == 201402L
  std::cout << "standard C++14)" << std::endl;
#elif __cplusplus == 201703L
  std::cout << "standard C++17)" << std::endl;
#elif __cplusplus == 202002L
  std::cout << "standard C++20)" << std::endl;
#else
  std::cout << "(standard de C++ desconocido)" << std::endl;
#endif

  try {
    fs::path current_dir = fs::current_path();
    std::vector<std::string> reglas = cargar_gitignore(current_dir);
    std::set<std::string> directorios_excluir = {".git", "build", ".vscode",
                                                 "out"};
    bool first_directory = true;
    recorrer_directorios(current_dir, reglas, directorios_excluir, current_dir,
                         first_directory);

    std::cout << "\n\Número de ficheros con:\n";
    std::cout << "Only LF        : " << eolCount.onlyLF << "\n";
    std::cout << "Only CRLF      : " << eolCount.onlyCRLF << "\n";
    std::cout << "Only CR        : " << eolCount.onlyCR << "\n";
    std::cout << "CRLF & LF      : " << eolCount.crlfLF << "\n";
    std::cout << "CRLF & CR      : " << eolCount.crlfCR << "\n";
    std::cout << "LF & CR        : " << eolCount.lfCR << "\n";
    std::cout << "LF & CRLF & CR : " << eolCount.all << "\n";

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
