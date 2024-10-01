#ifndef SEMANTICANALYZER_H
#define SEMANTICANALYZER_H

#include "ASTNodes.h"
#include "SystemCatalog.h"
#include <vector>
#include <string>

bool semanticAnalysis(const SelectStmtNode& selectStmt, const SystemCatalog& catalog, std::vector<std::string>& errors);

#endif // SEMANTICANALYZER_H
