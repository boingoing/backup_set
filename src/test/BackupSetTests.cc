//-------------------------------------------------------------------------------------------------------
// Copyright (C) Taylor Woll. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "BackupSet.h"
#include "BackupSetReader.h"
#include "BackupSetWriter.h"
#include "test/TestCase.h"
#include "test/TestCaseData.h"

struct FileDescriptor {
  std::string sha1hash;
  std::string filename;

  friend std::ostream& operator<<(std::ostream& os, const FileDescriptor& fd) {
    return os << "[" << fd.sha1hash << " " << fd.filename << "]";
  }
};

struct BackupSetTestData : TestCaseData {
  std::vector<FileDescriptor> old_set;
  std::vector<FileDescriptor> new_set;
  std::vector<std::string> old_not_in_new;
  std::vector<std::string> new_not_in_old;

  BackupSetTestData(std::vector<FileDescriptor> old_set,
      std::vector<FileDescriptor> new_set,
      std::vector<std::string> old_not_in_new,
      std::vector<std::string> new_not_in_old) :
      old_set(old_set), new_set(new_set),
      old_not_in_new(old_not_in_new), new_not_in_old(new_not_in_old) {}
};

std::vector<BackupSetTestData> backup_set_tests = {
  {{{"11111", "c:\\temp\\something.pdf"},{"22222", "c:\\temp\\something else.pdf"}},
  {{"33333", "c:\\temp\\another thing.pdf"},{"11111", "c:\\temp\\something.pdf"}},
  {"c:\\temp\\something else.pdf"},
  {"c:\\temp\\another thing.pdf"}},
  {{{"11111", "c:\\temp\\something.pdf"},{"22222", "c:\\temp\\something else.pdf"}},
  {{"33333", "c:\\temp\\another thing.pdf"},{"11111", "c:\\temp\\not the same something.pdf"}},
  {"c:\\temp\\something else.pdf"},
  {"c:\\temp\\another thing.pdf"}},
};

class BackupSetTest : public TestCase {
 protected:

  template <bool should_validate = false>
  void roundtrip(const std::string& str, const std::string& expected) {
    trace << std::endl << "Attempting to roundtrip a BackupSet from this buffer:" << std::endl;
    trace << str << std::endl;

    BackupSet read_set;
    BackupSetReader reader(read_set);
    if (should_validate) {
      reader.enableValidation();
    }

    std::istringstream istr(str);
    reader.read(istr);

    BackupSetWriter writer(read_set);
    std::stringstream ostr;
    writer.write(ostr);

    trace << "Found: " << std::endl << ostr.str() << std::endl;
    trace << "Expected: " << std::endl << expected << std::endl;
    assert.equal(ostr.str(), expected);
  }
};

TEST_CASE_WITH_DATA(BackupSetTest, tests, BackupSetTestData, backup_set_tests) {
  trace << std::endl << "Finding the missing files between old and new backup sets." << std::endl;
  trace << "Old backup set:" << std::endl;
  trace.vector(data.old_set);
  trace << "New backup set:" << std::endl;
  trace.vector(data.new_set);

  BackupSet old_set;
  for (const auto& fd : data.old_set) {
    old_set.addFile(fd.sha1hash, fd.filename);
  }

  BackupSet new_set;
  for (const auto& fd : data.new_set) {
    new_set.addFile(fd.sha1hash, fd.filename);
  }

  const auto new_not_in_old = old_set.getMissingFiles(new_set);
  trace << "Files found in new but not present in old (NewNotInOld):" << std::endl;
  trace.vector(new_not_in_old);
  assert.equal(new_not_in_old, data.new_not_in_old);

  const auto old_not_in_new = new_set.getMissingFiles(old_set);
  trace << "Files found in old but not present in new (OldNotInNew):" << std::endl;
  trace.vector(old_not_in_new);
  assert.equal(old_not_in_new, data.old_not_in_new);
}

struct BackupSetWriterTestData : TestCaseDataWithExpectedResult<std::string> {
  std::vector<FileDescriptor> backup_set;
};

std::vector<BackupSetWriterTestData> backup_set_writer_tests = {
  {"11111 c:\\file 1.txt\n"
    "22222 c:\\file 2.txt\n"
    "33333 c:\\file 3.txt\n"
    "44444 c:\\file 4.txt\n", {
      {"11111", "c:\\file 1.txt"},
      {"22222", "c:\\file 2.txt"},
      {"33333", "c:\\file 3.txt"},
      {"44444", "c:\\file 4.txt"}}},
};

TEST_CASE_WITH_DATA(BackupSetTest, writer_buffer, BackupSetWriterTestData, backup_set_writer_tests) {
  trace << std::endl << "Attempting to write this BackupSet:" << std::endl;
  trace.vector(data.backup_set);

  BackupSet backup_set;
  for (const auto& fd : data.backup_set) {
    backup_set.addFile(fd.sha1hash, fd.filename);
  }
  
  BackupSetWriter writer(backup_set);
  std::stringstream str;
  writer.write(str);
  trace << "Found: " << std::endl << str.str() << std::endl;
  trace << "Expected: " << std::endl << data.expected << std::endl;
  assert.equal(str.str(), data.expected);
}

struct BackupSetReaderTestData : TestCaseDataWithExpectedResult<std::vector<FileDescriptor>> {
  std::string str;
};

std::vector<BackupSetReaderTestData> backup_set_reader_tests = {
  {std::vector<FileDescriptor>({
      {"1111111111111111111111111111111111111111", "c:\\file 1.txt"},
      {"2222222222222222222222222222222222222222", "c:\\file 2.txt"},
      {"3333333333333333333333333333333333333333", "c:\\file 3.txt"},
      {"4444444444444444444444444444444444444444", "c:\\file 4.txt"}}),
      "1111111111111111111111111111111111111111 c:\\file 1.txt\n"
      "2222222222222222222222222222222222222222 c:\\file 2.txt\n"
      "3333333333333333333333333333333333333333 c:\\file 3.txt\n"
      "4444444444444444444444444444444444444444 c:\\file 4.txt\n"},
};

TEST_CASE_WITH_DATA(BackupSetTest, reader_buffer, BackupSetReaderTestData, backup_set_reader_tests) {
  trace << std::endl << "Attempting to read a BackupSet from this buffer:" << std::endl;
  trace << data.str << std::endl;

  BackupSet backup_set;
  BackupSetReader reader(backup_set);
  std::istringstream str(data.str);
  reader.read(str);

  BackupSet expected;
  for (const auto& fd : data.expected) {
    expected.addFile(fd.sha1hash, fd.filename);
  }

  trace << "Expected BackupSet:" << std::endl;
  trace.vector(data.expected);

  const auto expected_not_found = backup_set.getMissingFiles(expected);
  trace << "Missing files expected:" << std::endl;
  trace.vector(expected_not_found);
  assert.equal(expected_not_found, {});

  const auto unexpected_found = expected.getMissingFiles(backup_set);
  trace << "Unexpected files found:" << std::endl;
  trace.vector(unexpected_found);
  assert.equal(unexpected_found, {});
}

struct BackupSetRoundtripTestData : TestCaseDataWithExpectedResult<std::string> {
  std::string str;
};

std::vector<BackupSetRoundtripTestData> backup_set_roundtrip_tests = {
  {"", "1111111111111111111111111111111111111111 c:\\file 1.txt\n"
      "2222222222222222222222222222222222222222 c:\\file 2.txt\n"
      "3333333333333333333333333333333333333333 c:\\file 3.txt\n"
      "4444444444444444444444444444444444444444 c:\\file 4.txt\n"},
};

TEST_CASE_WITH_DATA(BackupSetTest, roundtrip_buffer, BackupSetRoundtripTestData, backup_set_roundtrip_tests) {
  // Expected string should be equal to source string if there are no validation errors.
  roundtrip<false>(data.str, data.str);
}

std::vector<BackupSetRoundtripTestData> backup_set_roundtrip_validate_tests = {
  {"1111111111111111111111111111111111111111 c:\\file 1.txt\n"
      "4444f444F44444444a4444444444A44444e44E44 c:\\file 4.txt\n",
      "1111111111111111111111111111111111111111 c:\\file 1.txt\n"
      "222222222222222222222222222222222222222 c:\\file 2.txt\n"
      "3333333333333333333333333333333333333z33 c:\\file 3.txt\n"
      "4444f444F44444444a4444444444A44444e44E44 c:\\file 4.txt\n"},
};

TEST_CASE_WITH_DATA(BackupSetTest, roundtrip_buffer_validate, BackupSetRoundtripTestData, backup_set_roundtrip_validate_tests) {
  roundtrip<true>(data.str, data.expected);
}
