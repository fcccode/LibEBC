#include "ebc/BitcodeContainer.h"
#include "ebc/EmbeddedFile.h"

#include "catch.hpp"

#include <string>
#include <vector>

TEST_CASE("Bitcode Container Commands", "[BitcodeContainer]") {
  auto bitcodeContainer = ebc::BitcodeContainer(nullptr, 0);
  std::vector<std::string> commands = {"a", "b", "c"};
  bitcodeContainer.SetCommands(commands);
  REQUIRE(commands == bitcodeContainer.GetCommands());
}

TEST_CASE("Bitcode Container Files", "[BitcodeContainer]") {
  using ebc::EmbeddedFile;
  char data[8];
  data[0] = 0x42;
  data[1] = 0x43;
  data[2] = 0xC0;
  data[3] = 0xDE;
  data[4] = 0x42;
  data[5] = 0x43;
  data[6] = 0xC0;
  data[7] = 0xDE;
  auto bitcodeContainer = ebc::BitcodeContainer(data, 8);
  bitcodeContainer.GetBinaryMetadata().SetFileFormatName("name");

  auto actualFiles = bitcodeContainer.GetEmbeddedFiles();
  REQUIRE(actualFiles.size() == 2);

  for (auto& file : actualFiles) {
    file->Remove();
  }
}

TEST_CASE("Bitcode Container Marker Only", "[BitcodeContainer]") {
  using ebc::EmbeddedFile;
  char data[3];
  data[0] = 0x42;
  data[1] = 0x43;
  data[2] = 0xC0;

  auto bitcodeContainer = ebc::BitcodeContainer(data, 3);
  bitcodeContainer.GetBinaryMetadata().SetFileFormatName("name");

  auto actualFiles = bitcodeContainer.GetEmbeddedFiles();
  REQUIRE(actualFiles.empty());
}

TEST_CASE("Bitcode Container Prefix", "[BitcodeContainer]") {
  using ebc::EmbeddedFile;
  char data[8];
  data[0] = 0x42;
  data[1] = 0x43;
  data[2] = 0xC0;
  data[3] = 0xDE;
  data[4] = 0x42;
  data[5] = 0x43;
  data[6] = 0xC0;
  data[7] = 0xDE;

  auto bitcodeContainer = ebc::BitcodeContainer(data, 8);
  bitcodeContainer.GetBinaryMetadata().SetFileFormatName("name");

  REQUIRE(bitcodeContainer.GetPrefix() == "");
  bitcodeContainer.SetPrefix("prefix");
  REQUIRE(bitcodeContainer.GetPrefix() == "prefix");

  for (auto& file : bitcodeContainer.GetEmbeddedFiles()) {
    REQUIRE(file->GetName().find("prefix") == 0);
    file->Remove();
  }
}
