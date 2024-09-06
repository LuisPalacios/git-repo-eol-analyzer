#include <gtest/gtest.h>
#include "../src/eol_analyzer.h"

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
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
