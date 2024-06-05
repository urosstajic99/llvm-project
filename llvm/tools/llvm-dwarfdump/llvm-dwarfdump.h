//===-- llvm-dwarfdump - Debug info dumping utility -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TOOLS_LLVM_DWARFDUMP_LLVM_DWARFDUMP_H
#define LLVM_TOOLS_LLVM_DWARFDUMP_LLVM_DWARFDUMP_H

#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/Twine.h"
#include "llvm/DebugInfo/DWARF/DWARFContext.h"
#include "llvm/Object/ObjectFile.h"
#include "llvm/Support/raw_ostream.h"

namespace llvm {
namespace dwarfdump {

/// Holds cumulative section sizes for an object file.
struct SectionSizes {
  /// Map of .debug section names and their sizes across all such-named
  /// sections.
  MapVector<std::string, uint64_t, StringMap<uint64_t>> DebugSectionSizes;
  /// Total number of bytes of all sections.
  uint64_t TotalObjectSize = 0;
  /// Total number of bytes of all debug sections.
  uint64_t TotalDebugSectionsSize = 0;
};

struct Row {
  int64_t sizeOfSection;
  double sizeInPercents;
};

/// Calculate the section sizes.
void calculateSectionSizes(const object::ObjectFile &Obj, SectionSizes &Sizes,
                           const Twine &Filename);

bool collectStatsForObjectFile(object::ObjectFile &Obj, DWARFContext &DICtx,
                               const Twine &Filename, raw_ostream &OS);
bool collectObjectSectionSizes(object::ObjectFile &Obj, DWARFContext &DICtx,
                               const Twine &Filename, raw_ostream &OS);

SectionSizes collectObjectDiffSectionSizes(object::ObjectFile &Obj,
                                           DWARFContext &DICtx,
                                           const Twine &Filename,
                                           raw_ostream &OS);

void processAllSectionSizes(std::vector<SectionSizes> allSectionSizes,
                            raw_ostream &OS, bool baseFileExists);

uint64_t findSectionSize(std::string sectionName, SectionSizes currentSection);

std::string DoubleStringPrintf(const char *fmt, double d);

std::string PercentString(double percent);

std::string LeftPad(const std::string &input, size_t size);

std::string FixedWidthString(const std::string &input, size_t size);

std::string SiPrint(int64_t size, bool force_sign);

double Percent(int64_t part, int64_t whole);

void PrettyPrint(std::unordered_map<std::string, Row> allRowsForPrint,
                 raw_ostream &OS, int64_t totalFileSize,
                 double totalFileSizeInPercent);

void PrettyPrintRow(std::unordered_map<std::string, Row> allRowsForPrint,
                    raw_ostream &OS, int indent, int64_t totalDiffFileSize,
                    double totalDiffFileSizeInPercent);
} // namespace dwarfdump
} // namespace llvm

#endif
