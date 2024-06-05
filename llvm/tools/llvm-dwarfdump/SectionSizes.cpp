//===-- SectionSizes.cpp - Debug section sizes ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm-dwarfdump.h"
#include "math.h"

#define DEBUG_TYPE "dwarfdump"

using namespace llvm;
using namespace llvm::dwarfdump;
using namespace llvm::object;

static size_t getNameColumnWidth(const SectionSizes &Sizes,
                                 const StringRef SectionNameTitle) {
  // The minimum column width should be the size of "SECTION".
  size_t Width = SectionNameTitle.size();
  for (const auto &It : Sizes.DebugSectionSizes)
    Width = std::max(Width, It.first.size());
  return Width;
}

static size_t getSizeColumnWidth(const SectionSizes &Sizes,
                                 const StringRef SectionSizeTitle) {
  // The minimum column width should be the size of the column title.
  size_t Width = SectionSizeTitle.size();
  for (const auto &It : Sizes.DebugSectionSizes) {
    size_t NumWidth = std::to_string(It.second).size();
    Width = std::max(Width, NumWidth);
  }
  return Width;
}

static void prettyPrintSectionSizes(const ObjectFile &Obj,
                                    const SectionSizes &Sizes,
                                    raw_ostream &OS) {
  const StringRef SectionNameTitle = "SECTION";
  const StringRef SectionSizeTitle = "SIZE (b)";

  size_t NameColWidth = getNameColumnWidth(Sizes, SectionNameTitle);
  size_t SizeColWidth = getSizeColumnWidth(Sizes, SectionSizeTitle);

  OS << "----------------------------------------------------" << '\n';
  OS << SectionNameTitle;
  size_t SectionNameTitleWidth = SectionNameTitle.size();
  for (unsigned i = 0; i < (NameColWidth - SectionNameTitleWidth) + 2; i++)
    OS << " ";
  OS << SectionSizeTitle << '\n';
  for (unsigned i = 0; i < NameColWidth; i++)
    OS << "-";
  OS << "  ";

  for (unsigned i = 0; i < SizeColWidth; i++)
    OS << "-";
  OS << '\n';

  for (const auto &It : Sizes.DebugSectionSizes) {
    OS << left_justify(It.first, NameColWidth) << "  ";

    std::string NumBytes = std::to_string(It.second);
    OS << right_justify(NumBytes, SizeColWidth) << " ("
       << format("%0.2f",
                 It.second / static_cast<double>(Sizes.TotalObjectSize) * 100)
       << "%)\n";
  }

  OS << '\n';
  OS << " Total Size: " << Sizes.TotalDebugSectionsSize << "  ("
     << format("%0.2f", Sizes.TotalDebugSectionsSize /
                            static_cast<double>(Sizes.TotalObjectSize) * 100)
     << "%)\n";
  OS << " Total File Size: " << Sizes.TotalObjectSize << '\n';
  OS << "----------------------------------------------------" << '\n';
}

void dwarfdump::calculateSectionSizes(const ObjectFile &Obj,
                                      SectionSizes &Sizes,
                                      const Twine &Filename) {
  // Get total size.
  Sizes.TotalObjectSize = Obj.getData().size();

  for (const SectionRef &Section : Obj.sections()) {
    StringRef SectionName;
    if (Expected<StringRef> NameOrErr = Section.getName())
      SectionName = *NameOrErr;
    else
      WithColor::defaultWarningHandler(
          createFileError(Filename, NameOrErr.takeError()));

    LLVM_DEBUG(dbgs() << SectionName.str() << ": " << Section.getSize()
                      << '\n');

    if (!Section.isDebugSection())
      continue;

    Sizes.TotalDebugSectionsSize += Section.getSize();
    Sizes.DebugSectionSizes[std::string(SectionName)] += Section.getSize();
  }
}

bool dwarfdump::collectObjectSectionSizes(ObjectFile &Obj,
                                          DWARFContext & /*DICtx*/,
                                          const Twine &Filename,
                                          raw_ostream &OS) {
  SectionSizes Sizes;

  // Get the section sizes.
  calculateSectionSizes(Obj, Sizes, Filename);

  OS << "----------------------------------------------------\n";
  OS << "file: " << Filename.str() << '\n';

  prettyPrintSectionSizes(Obj, Sizes, OS);

  // TODO: If the input file is an archive, print the cumulative summary of all
  // files from the archive.

  return true;
}

SectionSizes dwarfdump::collectObjectDiffSectionSizes(ObjectFile &Obj,
                                                      DWARFContext & /*DICtx*/,
                                                      const Twine &Filename,
                                                      raw_ostream &OS) {
  SectionSizes Sizes;

  calculateSectionSizes(Obj, Sizes, Filename);

  return Sizes;
}

uint64_t dwarfdump::findSectionSize(std::string sectionName,
                                    SectionSizes currentSection) {
  if (currentSection.DebugSectionSizes.find(sectionName) !=
      currentSection.DebugSectionSizes.end()) {
    return currentSection.DebugSectionSizes[sectionName];
  }
  return 0;
}

void dwarfdump::processAllSectionSizes(
    std::vector<SectionSizes> allFilesSectionSizes, raw_ostream &OS,
    bool baseFileExists) {

  if (!baseFileExists) {
    _Exit(-1);
  }

  std::set<std::string> foundSectionNames;
  SectionSizes baseFileSectionSizes;
  baseFileSectionSizes = allFilesSectionSizes[allFilesSectionSizes.size() - 1];

  for (auto fileSectionSizes : allFilesSectionSizes) {
    for (auto section : fileSectionSizes.DebugSectionSizes) {
      if (foundSectionNames.find(section.first) == foundSectionNames.end()) {
        foundSectionNames.insert(section.first);
      }
    }
  }

  std::unordered_map<std::string, Row> allRowsForPrint;
  int64_t totalDiffFileSize = 0;
  double totalDiffFileSizeInPercent = 0;

  for (auto sectionName : foundSectionNames) {
    Row newRow;
    newRow.sizeInPercents = 100;
    newRow.sizeOfSection = -findSectionSize(sectionName, baseFileSectionSizes);

    allRowsForPrint.insert(std::make_pair(sectionName, newRow));
    totalDiffFileSize = -baseFileSectionSizes.TotalObjectSize;
  }

  int64_t currIteration = 0;
  for (const auto &currSectionSizes : allFilesSectionSizes) {
    if (currIteration == (int64_t)(allFilesSectionSizes.size() - 1)) {
      continue;
    }
    for (auto sections : foundSectionNames) {
      auto currentSection = findSectionSize(sections, currSectionSizes);
      auto baseFileAmount = findSectionSize(sections, baseFileSectionSizes);

      allRowsForPrint[sections].sizeOfSection =
          currentSection + allRowsForPrint[sections].sizeOfSection;
      allRowsForPrint[sections].sizeInPercents =
          Percent(allRowsForPrint[sections].sizeOfSection, baseFileAmount);
    }
    totalDiffFileSize += currSectionSizes.TotalObjectSize;
    totalDiffFileSizeInPercent =
        Percent(totalDiffFileSize, baseFileSectionSizes.TotalObjectSize);
    currIteration++;
  }

  PrettyPrint(allRowsForPrint, OS, totalDiffFileSize,
              totalDiffFileSizeInPercent);
}

double dwarfdump::Percent(int64_t part, int64_t whole) {
  if (whole == 0) {
    if (part == 0) {
      return NAN;
    } else if (part > 0) {
      return INFINITY;
    } else {
      return -INFINITY;
    }
  } else {
    return static_cast<double>(part) / static_cast<double>(whole) * 100;
  }
}

std::string dwarfdump::PercentString(double percent) {
  if (percent == 0 || std::isnan(percent)) {
    return " [ = ]";
  } else if (percent == -100) {
    return " [DEL]";
  } else if (std::isinf(percent)) {
    return " [NEW]";
  } else {
    // We want to keep this fixed-width even if the percent is very large.
    std::string str;
    if (percent > 1000) {
      int digits = log10(percent) - 1;
      str = DoubleStringPrintf("%+2.0f", percent / pow(10, digits)) + "e" +
            std::to_string(digits) + "%";
    } else if (percent > 10) {
      str = DoubleStringPrintf("%+4.0f%%", percent);
    } else {
      str = DoubleStringPrintf("%+5.1F%%", percent);
    }

    return LeftPad(str, 6);
  }
}

std::string dwarfdump::DoubleStringPrintf(const char *fmt, double d) {
  char buf[1024];
  snprintf(buf, sizeof(buf), fmt, d);
  return std::string(buf);
}

std::string dwarfdump::LeftPad(const std::string &input, size_t size) {
  std::string ret = input;
  while (ret.size() < size) {
    ret = " " + ret;
  }

  return ret;
}

void dwarfdump::PrettyPrintRow(
    std::unordered_map<std::string, Row> allRowsForPrint, raw_ostream &OS,
    int indent, int64_t totalDiffFileSize, double totalDiffFileSizeInPercent) {
  for (auto row : allRowsForPrint) {
    OS << FixedWidthString("", indent) << " ";

    OS << PercentString(row.second.sizeInPercents) << " "
       << SiPrint(row.second.sizeOfSection, true) << " ";

    OS << "   " << row.first << "\n";
  }

  // print total
  OS << FixedWidthString("", indent) << " ";

  OS << PercentString(totalDiffFileSizeInPercent) << " "
     << SiPrint(totalDiffFileSize, true) << " ";

  OS << "   TOTAL\n";
}

std::string dwarfdump::FixedWidthString(const std::string &input, size_t size) {
  if (input.size() < size) {
    std::string ret = input;
    while (ret.size() < size) {
      ret += " ";
    }
    return ret;
  } else {
    return input.substr(0, size);
  }
}

std::string dwarfdump::SiPrint(int64_t size, bool force_sign) {
  const char *prefixes[] = {"", "Ki", "Mi", "Gi", "Ti"};
  size_t num_prefixes = 5;
  size_t n = 0;
  double size_d = size;
  while (fabs(size_d) > 1024 && n < num_prefixes - 2) {
    size_d /= 1024;
    n++;
  }

  std::string ret;

  if (fabs(size_d) > 100 || n == 0) {
    ret = std::to_string(static_cast<int64_t>(size_d)) + prefixes[n];
    if (force_sign && size > 0) {
      ret = "+" + ret;
    }
  } else if (fabs(size_d) > 10) {
    if (force_sign) {
      ret = DoubleStringPrintf("%+0.1f", size_d) + prefixes[n];
    } else {
      ret = DoubleStringPrintf("%0.1f", size_d) + prefixes[n];
    }
  } else {
    if (force_sign) {
      ret = DoubleStringPrintf("%+0.2f", size_d) + prefixes[n];
    } else {
      ret = DoubleStringPrintf("%0.2f", size_d) + prefixes[n];
    }
  }

  return LeftPad(ret, 7);
}

void dwarfdump::PrettyPrint(
    std::unordered_map<std::string, Row> allRowsForPrint, raw_ostream &OS,
    int64_t totalFileSizeDiff, double totalFileSizeInPercent) {
  OS << "    FILE SIZE   ";

  OS << "\n";

  OS << " -------------- ";

  OS << "\n";

  // The "TOTAL" row comes after all other rows.
  PrettyPrintRow(allRowsForPrint, OS, 0, totalFileSizeDiff,
                 totalFileSizeInPercent);
}
