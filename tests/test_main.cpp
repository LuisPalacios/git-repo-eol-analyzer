#include <gtest/gtest.h>
#include <iostream>
#include "../src/eol_analyzer.h"

// Para mostrar el path desde donde se esta ejecutando el test
#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

// Test para analizar EOL en un archivo con solo LF
TEST(EOLAnalyzerTest, OnlyLF) {
  std::string test_file = "test_files/test_only_lf.test";
  auto result = analizar_eol(test_file);
  ASSERT_GT(result.hasLF, 0);
  ASSERT_EQ(result.hasCR, 0);
  ASSERT_EQ(result.hasCRLF, 0);
}

// Test para analizar EOL en un archivo con solo CRLF
TEST(EOLAnalyzerTest, OnlyCRLF) {
  std::string test_file = "test_files/test_only_crlf.test";
  auto result = analizar_eol(test_file);
  ASSERT_EQ(result.hasLF, 0);
  ASSERT_EQ(result.hasCR, 0);
  ASSERT_GT(result.hasCRLF, 0);
}

// Test para analizar un archivo con CR, LF y CRLF
TEST(EOLAnalyzerTest, CRLF_LF_CR) {
  std::string test_file = "test_files/test_CRLF_LF_CR.test";
  auto result = analizar_eol(test_file);
  ASSERT_GT(result.hasLF, 0);
  ASSERT_GT(result.hasCR, 0);
  ASSERT_GT(result.hasCRLF, 0);
}

int main(int argc, char** argv) {
  // buffer para el path
  char buffer[PATH_MAX];

  // Mostrar el path desde donde se esta ejecutando el test
#ifdef _WIN32
  if (GetCurrentDirectory(sizeof(buffer), buffer)) {
    std::cout << "Current Working Directory: " << buffer << std::endl;
  } else {
    std::cerr << "Error retrieving current working directory" << std::endl;
  }
#else
  if (getcwd(buffer, sizeof(buffer)) != nullptr) {
    std::cout << "Current Working Directory: " << buffer << std::endl;
  } else {
    std::cerr << "Error retrieving current working directory" << std::endl;
  }
#endif

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
