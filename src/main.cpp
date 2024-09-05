#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

// Descomentar la siguiente línea para habilitar las trazas de depuración
// #define DEBUG_MODE

namespace fs = std::filesystem;

// Colores para terminales ANSI (Linux, macOS y algunos emuladores de Windows)
const std::string RESET_COLOR = "\033[0m";
const std::string GREEN = "\033[32m";   // LF (line feed)
const std::string RED = "\033[31m";     // CR (carriage return)
const std::string YELLOW = "\033[33m";  // CRLF

// Struct para guardar las combinaciones
struct EOLCount {
  int onlyLF = 0;
  int onlyCRLF = 0;
  int onlyCR = 0;
  int crlfLF = 0;
  int crlfCR = 0;
  int lfCR = 0;
  int all = 0;
};

// Variable global para llevar la cuenta
EOLCount eolCount;

// Función que analiza el tipo de EOL en un archivo
std::vector<std::string> analizar_eol(const std::string& file_path) {
  std::ifstream file(file_path, std::ios::binary);
  std::vector<std::string> eol_types;
  // bool hasCR = false, hasLF = false, hasCRLF = false;
  char prev = 0, current;
  int hasCR = 0, hasLF = 0, hasCRLF = 0;

  // Para rastrear en qué posición estamos analizando.
  int byte_idx = 1;

#ifdef DEBUG_MODE
  std::cout << "Analizando el archivo: " << file_path << std::endl;
#endif

  // Analizar el archivo byte por byte
  if (file.is_open()) {
    while (file.get(current)) {
      if (prev == '\r' && current == '\n') {
        hasCRLF++;
        hasCR--;  // Restar uno de CR si fue parte de CRLF
#ifdef DEBUG_MODE
        std::cout << "Pos " << byte_idx << ": Detectado CRLF (" << hasCRLF
                  << ")\n";
#endif
        prev = 0;  // Reiniciar el estado
      } else if (current == '\r') {
        hasCR++;
#ifdef DEBUG_MODE
        std::cout << "Pos " << byte_idx << ": Detectado CR (" << hasCR << ")\n";
#endif
        prev = current;  // Guardar el CR en prev para posible CRLF
      } else if (current == '\n') {
        if (prev != '\r')  // Detecta LF si no fue parte de CRLF
        {
          hasLF++;
#ifdef DEBUG_MODE
          std::cout << "Pos " << byte_idx << ": Detectado LF (" << hasLF
                    << ")\n";
#endif
        }
        prev = 0;  // Reiniciar el estado después de un LF
      } else {
        prev = current;
      }
      byte_idx++;
    }
  }

#ifdef DEBUG_MODE
  std::cout << "Resultados del análisis:\n";
#endif
  if (hasCRLF > 0) {
#ifdef DEBUG_MODE
    std::cout << "Total CRLF: " << hasCRLF << std::endl;
#endif
    eol_types.push_back(YELLOW + "CRLF" + RESET_COLOR);
  }
  if (hasLF > 0) {
#ifdef DEBUG_MODE
    std::cout << "Total LF: " << hasLF << std::endl;
#endif
    eol_types.push_back(GREEN + "LF" + RESET_COLOR);
  }
  if (hasCR > 0) {
#ifdef DEBUG_MODE
    std::cout << "Total CR: " << hasCR << std::endl;
#endif
    eol_types.push_back(RED + "CR" + RESET_COLOR);
  }

  // Increment counters based on the detected combinations
  if (hasLF > 0 && hasCR == 0 && hasCRLF == 0) {
    eolCount.onlyLF++;
  } else if (hasCRLF > 0 && hasLF == 0 && hasCR == 0) {
    eolCount.onlyCRLF++;
  } else if (hasCR > 0 && hasLF == 0 && hasCRLF == 0) {
    eolCount.onlyCR++;
  } else if (hasCRLF > 0 && hasLF > 0 && hasCR == 0) {
    eolCount.crlfLF++;
  } else if (hasCRLF > 0 && hasCR > 0 && hasLF == 0) {
    eolCount.crlfCR++;
  } else if (hasLF > 0 && hasCR > 0 && hasCRLF == 0) {
    eolCount.lfCR++;
  } else if (hasLF > 0 && hasCR > 0 && hasCRLF > 0) {
    eolCount.all++;
  }

  return eol_types;
}

// Función para determinar si un archivo es de texto (no binario)
bool es_archivo_de_texto(const std::string& file_path) {
  std::ifstream file(file_path, std::ios::binary);
  char ch;
  while (file.get(ch)) {
    if (ch == '\0')
      return false;  // Los archivos binarios suelen tener caracteres nulos
  }
  return true;
}

// Cargar las reglas desde un archivo .gitignore
std::vector<std::string> cargar_gitignore(const fs::path& dir_path) {
  std::vector<std::string> reglas;
  fs::path gitignore_path = dir_path / ".gitignore";

  if (fs::exists(gitignore_path)) {
    std::ifstream gitignore_file(gitignore_path);
    std::string linea;

    while (std::getline(gitignore_file, linea)) {
      // Ignorar comentarios o líneas vacías
      if (linea.empty() || linea[0] == '#') {
        continue;
      }
      reglas.push_back(linea);
    }
  }

  return reglas;
}

// Función para verificar si una ruta coincide con alguna regla de .gitignore
bool ignorar_archivo_o_directorio(const fs::path& path,
                                  const std::vector<std::string>& reglas) {
  for (const auto& regla : reglas) {
    std::regex pattern;

    // Convierte la regla en una expresión regular
    std::string regla_regex =
        std::regex_replace(regla, std::regex(R"(\*)"), ".*");
    regla_regex = std::regex_replace(regla_regex, std::regex(R"(\?)"), ".");

    // Si la regla termina en '/', es un directorio
    if (regla.back() == '/') {
      pattern = std::regex(regla_regex);
      if (fs::is_directory(path) &&
          std::regex_match(path.filename().string(), pattern)) {
        return true;  // Ignorar el directorio completo
      }
    } else {
      pattern = std::regex(regla_regex);
      if (std::regex_match(path.filename().string(), pattern)) {
        return true;  // Ignorar el archivo
      }
    }
  }

  return false;
}

// Función para formatear la ruta para que empiece con "./"
std::string formatear_ruta(const fs::path& dir_path, const fs::path& base_dir) {
  return "./" + fs::relative(dir_path, base_dir).string();
}

// Función para recorrer directorios recursivamente, ignorando los que estén en
// .gitignore y los directorios en la lista de exclusión
void recorrer_directorios(const fs::path& dir_path,
                          const std::vector<std::string>& reglas,
                          const std::set<std::string>& directorios_excluir,
                          const fs::path& base_dir,
                          bool& first_directory) {
  // Variable para el mensaje del nombre del directorio
  const std::string directory_label = "Directory: ";

  // Mostrar el directorio actual; no agregar línea en blanco si es el primer
  // directorio
  if (!first_directory) {
    std::cout << std::endl
              << directory_label << formatear_ruta(dir_path, base_dir) << ":"
              << std::endl;
  } else {
    std::cout << directory_label << "." << std::endl;
    first_directory = false;
  }

  for (const auto& entry : fs::directory_iterator(dir_path)) {
    // Ignorar directorios de primer nivel especificados en el array de
    // exclusión
    if (directorios_excluir.find(entry.path().filename().string()) !=
        directorios_excluir.end()) {
      continue;
    }

    // Ignorar archivos y directorios según las reglas de .gitignore
    if (ignorar_archivo_o_directorio(entry.path(), reglas)) {
      continue;
    }

    if (fs::is_directory(entry)) {
      recorrer_directorios(entry.path(), reglas, directorios_excluir, base_dir,
                           first_directory);  // Recursividad
    } else if (fs::is_regular_file(entry)) {
      std::string file_path = entry.path().string();
      if (es_archivo_de_texto(file_path)) {
        std::vector<std::string> eol_types = analizar_eol(file_path);

        std::cout << " " << entry.path().filename().string() << ":";

        // Espaciado dinámico para alinear la salida a partir de la columna 80
        const int output_column = 60;
        std::string file_name = entry.path().filename().string();
        int current_length = file_name.length() + 2;  // 2 es para ": "
        if (current_length < output_column) {
          std::cout << std::string(output_column - current_length, ' ');
        }

        // Mostrar los tipos de EOL encontrados, justificados a la derecha
        for (const auto& eol_type : eol_types) {
          std::cout << eol_type << " ";
        }
        std::cout << std::endl;
      }
    }
  }
}

int main(int argc, char* argv[]) {
  std::cout << "git-repo-eol-analyzer (Compiled with CLANG-17 - ";
#if __cplusplus == 199711L
  std::cout << "C++98 standard)" << std::endl;
#elif __cplusplus == 201103L
  std::cout << "C++11 standard)" << std::endl;
#elif __cplusplus == 201402L
  std::cout << "C++14 standard)" << std::endl;
#elif __cplusplus == 201703L
  std::cout << "C++17 standard)" << std::endl;
#elif __cplusplus == 202002L
  std::cout << "C++20 standard)" << std::endl;
#else
  std::cout << "Unknown C++ standard)" << std::endl;
#endif

  std::vector<int> v{};

  try {
    // Directorio actual desde el cual se iniciará la búsqueda recursiva
    fs::path current_dir = fs::current_path();

    // Cargar las reglas de .gitignore
    std::vector<std::string> reglas = cargar_gitignore(current_dir);

    // Array de directorios de primer nivel a excluir de la búsqueda
    std::set<std::string> directorios_excluir = {".git", "build", ".vscode",
                                                 "out"};

    // Variable para controlar si es el primer directorio
    bool first_directory = true;

    // Recorrer los directorios respetando las reglas de .gitignore y excluyendo
    // los directorios de primer nivel
    recorrer_directorios(current_dir, reglas, directorios_excluir, current_dir,
                         first_directory);

    // Print the final report
    std::cout << "\n\nNumber of files with:\n";
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
