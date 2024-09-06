#ifndef EOL_ANALYZER_H
#define EOL_ANALYZER_H

#include <filesystem>
#include <map>
#include <set>
#include <string>
#include <vector>

// Descomentar la siguiente línea para habilitar las trazas de depuración
// #define DEBUG_MODE

// Tipos de EOL detectados en un archivo
struct EOLTypes {
  int hasLF = 0;
  int hasCRLF = 0;
  int hasCR = 0;
};

// Cantidad de cada tipo de EOL detectado
struct EOLCount {
  int onlyLF = 0;
  int onlyCRLF = 0;
  int onlyCR = 0;
  int crlfLF = 0;
  int crlfCR = 0;
  int lfCR = 0;
  int all = 0;
};

// Espacio de nombres para la biblioteca de archivos y directorios
namespace fs = std::filesystem;

// Colores para terminales ANSI (Linux, macOS y algunos emuladores de Windows)
const std::string RESET_COLOR = "\033[0m";
const std::string GREEN = "\033[32m";   // LF (line feed)
const std::string RED = "\033[31m";     // CR (carriage return)
const std::string YELLOW = "\033[33m";  // CRLF

// Variable global para llevar la cuenta de los tipos de EOL
extern EOLCount eolCount;

EOLTypes analizar_eol(const std::string& file_path);
bool es_archivo_de_texto(const std::string& file_path);
std::vector<std::string> cargar_gitignore(
    const std::filesystem::path& dir_path);
bool ignorar_archivo_o_directorio(const std::filesystem::path& path,
                                  const std::vector<std::string>& reglas);
std::string formatear_ruta(const fs::path& dir_path, const fs::path& base_dir);
void recorrer_directorios(const fs::path& dir_path,
                          const std::vector<std::string>& reglas,
                          const std::set<std::string>& directorios_excluir,
                          const fs::path& base_dir,
                          bool& first_directory);

#endif  // EOL_ANALYZER_H
