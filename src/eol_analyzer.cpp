#include "eol_analyzer.h"
#include <fstream>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;

EOLCount eolCount;  // Definición global de la estructura EOLCount

// Función que analiza el tipo de EOL en un archivo
// std::vector<std::string> analizar_eol(const std::string& file_path) {
EOLTypes analizar_eol(const std::string& file_path) {
  std::ifstream file(file_path, std::ios::binary);

  // bool hasCR = false, hasLF = false, hasCRLF = false;
  char prev = 0, current;
  int hasCR = 0, hasLF = 0, hasCRLF = 0;
  EOLTypes eolTypes;  // Estructura para almacenar los tipos de EOL detectados

  // Para rastrear en qué posición estamos analizando.
  int byte_idx = 1;

#ifdef DEBUG_MODE
  std::cout << "Analizando el archivo: " << file_path << std::endl;
#endif

  // Analizar el archivo byte por byte
  if (file.is_open()) {
    while (file.get(current)) {
      if (prev == '\r' && current == '\n') {
        eolTypes.hasCRLF++;  // Incrementar uno de CRLF si se detecta
        eolTypes.hasCR--;    // Restar uno de CR si fue parte de CRLF
#ifdef DEBUG_MODE
        std::cout << "Pos " << byte_idx << ": Detectado CRLF ("
                  << eolTypes.hasCRLF << ")\n";
#endif
        prev = 0;  // Reiniciar el estado
      } else if (current == '\r') {
        eolTypes.hasCR++;
#ifdef DEBUG_MODE
        std::cout << "Pos " << byte_idx << ": Detectado CR (" << eolTypes.hasCR
                  << ")\n";
#endif
        prev = current;  // Guardar el CR en prev para posible CRLF
      } else if (current == '\n') {
        if (prev != '\r')  // Detecta LF si no fue parte de CRLF
        {
          eolTypes.hasLF++;
#ifdef DEBUG_MODE
          std::cout << "Pos " << byte_idx << ": Detectado LF ("
                    << eolTypes.hasLF << ")\n";
#endif
        }
        prev = 0;  // Reiniciar el estado después de un LF
      } else {
        prev = current;
      }
      byte_idx++;
    }
  }

  // Increment counters based on the detected combinations
  if (eolTypes.hasLF > 0 && eolTypes.hasCR == 0 && eolTypes.hasCRLF == 0) {
    eolCount.onlyLF++;
  } else if (eolTypes.hasCRLF > 0 && eolTypes.hasLF == 0 &&
             eolTypes.hasCR == 0) {
    eolCount.onlyCRLF++;
  } else if (eolTypes.hasCR > 0 && eolTypes.hasLF == 0 &&
             eolTypes.hasCRLF == 0) {
    eolCount.onlyCR++;
  } else if (eolTypes.hasCRLF > 0 && eolTypes.hasLF > 0 &&
             eolTypes.hasCR == 0) {
    eolCount.crlfLF++;
  } else if (eolTypes.hasCRLF > 0 && eolTypes.hasCR > 0 &&
             eolTypes.hasLF == 0) {
    eolCount.crlfCR++;
  } else if (eolTypes.hasLF > 0 && eolTypes.hasCR > 0 &&
             eolTypes.hasCRLF == 0) {
    eolCount.lfCR++;
  } else if (eolTypes.hasLF > 0 && eolTypes.hasCR > 0 && eolTypes.hasCRLF > 0) {
    eolCount.all++;
  }

  return eolTypes;
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
  // Variable para almacenar los tipos de EOL detectados
  EOLTypes eolTypes;
  // Variable para almacenar los mensajes en pantalla de tipos de EOL
  // encontrados en cada archivo
  std::vector<std::string> eol_types;

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
        eolTypes = analizar_eol(file_path);
        eol_types.clear();  // Limpiar el vector de tipos de EOL
#ifdef DEBUG_MODE
        std::cout << "Resultados del análisis:\n";
#endif
        if (eolTypes.hasCRLF > 0) {
#ifdef DEBUG_MODE
          std::cout << "Total CRLF: " << eolTypes.hasCRLF << std::endl;
#endif
          eol_types.push_back(YELLOW + "CRLF" + RESET_COLOR);
        }
        if (eolTypes.hasLF > 0) {
#ifdef DEBUG_MODE
          std::cout << "Total LF: " << eolTypes.hasLF << std::endl;
#endif
          eol_types.push_back(GREEN + "LF" + RESET_COLOR);
        }
        if (eolTypes.hasCR > 0) {
#ifdef DEBUG_MODE
          std::cout << "Total CR: " << eolTypes.hasCR << std::endl;
#endif
          eol_types.push_back(RED + "CR" + RESET_COLOR);
        }

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
