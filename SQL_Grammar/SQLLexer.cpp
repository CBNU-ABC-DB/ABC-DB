
// Generated from SQL.g4 by ANTLR 4.13.2


#include "SQLLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct SQLLexerStaticData final {
  SQLLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  SQLLexerStaticData(const SQLLexerStaticData&) = delete;
  SQLLexerStaticData(SQLLexerStaticData&&) = delete;
  SQLLexerStaticData& operator=(const SQLLexerStaticData&) = delete;
  SQLLexerStaticData& operator=(SQLLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag sqllexerLexerOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<SQLLexerStaticData> sqllexerLexerStaticData = nullptr;

void sqllexerLexerInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (sqllexerLexerStaticData != nullptr) {
    return;
  }
#else
  assert(sqllexerLexerStaticData == nullptr);
#endif
  auto staticData = std::make_unique<SQLLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "T__9", "IDENTIFIER", "NUMBER", "STRING_LITERAL", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'SELECT'", "'FROM'", "','", "'WHERE'", "'='", "'<'", "'>'", "'<='", 
      "'>='", "'!='"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "IDENTIFIER", "NUMBER", 
      "STRING_LITERAL", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,14,92,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,1,0,
  	1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,3,1,3,1,3,1,3,1,
  	3,1,3,1,4,1,4,1,5,1,5,1,6,1,6,1,7,1,7,1,7,1,8,1,8,1,8,1,9,1,9,1,9,1,10,
  	1,10,5,10,67,8,10,10,10,12,10,70,9,10,1,11,4,11,73,8,11,11,11,12,11,74,
  	1,12,1,12,5,12,79,8,12,10,12,12,12,82,9,12,1,12,1,12,1,13,4,13,87,8,13,
  	11,13,12,13,88,1,13,1,13,0,0,14,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,
  	9,19,10,21,11,23,12,25,13,27,14,1,0,5,3,0,65,90,95,95,97,122,4,0,48,57,
  	65,90,95,95,97,122,1,0,48,57,3,0,10,10,13,13,39,39,3,0,9,10,13,13,32,
  	32,95,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,
  	1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,
  	0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,1,29,1,0,0,0,3,36,1,0,0,0,
  	5,41,1,0,0,0,7,43,1,0,0,0,9,49,1,0,0,0,11,51,1,0,0,0,13,53,1,0,0,0,15,
  	55,1,0,0,0,17,58,1,0,0,0,19,61,1,0,0,0,21,64,1,0,0,0,23,72,1,0,0,0,25,
  	76,1,0,0,0,27,86,1,0,0,0,29,30,5,83,0,0,30,31,5,69,0,0,31,32,5,76,0,0,
  	32,33,5,69,0,0,33,34,5,67,0,0,34,35,5,84,0,0,35,2,1,0,0,0,36,37,5,70,
  	0,0,37,38,5,82,0,0,38,39,5,79,0,0,39,40,5,77,0,0,40,4,1,0,0,0,41,42,5,
  	44,0,0,42,6,1,0,0,0,43,44,5,87,0,0,44,45,5,72,0,0,45,46,5,69,0,0,46,47,
  	5,82,0,0,47,48,5,69,0,0,48,8,1,0,0,0,49,50,5,61,0,0,50,10,1,0,0,0,51,
  	52,5,60,0,0,52,12,1,0,0,0,53,54,5,62,0,0,54,14,1,0,0,0,55,56,5,60,0,0,
  	56,57,5,61,0,0,57,16,1,0,0,0,58,59,5,62,0,0,59,60,5,61,0,0,60,18,1,0,
  	0,0,61,62,5,33,0,0,62,63,5,61,0,0,63,20,1,0,0,0,64,68,7,0,0,0,65,67,7,
  	1,0,0,66,65,1,0,0,0,67,70,1,0,0,0,68,66,1,0,0,0,68,69,1,0,0,0,69,22,1,
  	0,0,0,70,68,1,0,0,0,71,73,7,2,0,0,72,71,1,0,0,0,73,74,1,0,0,0,74,72,1,
  	0,0,0,74,75,1,0,0,0,75,24,1,0,0,0,76,80,5,39,0,0,77,79,8,3,0,0,78,77,
  	1,0,0,0,79,82,1,0,0,0,80,78,1,0,0,0,80,81,1,0,0,0,81,83,1,0,0,0,82,80,
  	1,0,0,0,83,84,5,39,0,0,84,26,1,0,0,0,85,87,7,4,0,0,86,85,1,0,0,0,87,88,
  	1,0,0,0,88,86,1,0,0,0,88,89,1,0,0,0,89,90,1,0,0,0,90,91,6,13,0,0,91,28,
  	1,0,0,0,5,0,68,74,80,88,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  sqllexerLexerStaticData = std::move(staticData);
}

}

SQLLexer::SQLLexer(CharStream *input) : Lexer(input) {
  SQLLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *sqllexerLexerStaticData->atn, sqllexerLexerStaticData->decisionToDFA, sqllexerLexerStaticData->sharedContextCache);
}

SQLLexer::~SQLLexer() {
  delete _interpreter;
}

std::string SQLLexer::getGrammarFileName() const {
  return "SQL.g4";
}

const std::vector<std::string>& SQLLexer::getRuleNames() const {
  return sqllexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& SQLLexer::getChannelNames() const {
  return sqllexerLexerStaticData->channelNames;
}

const std::vector<std::string>& SQLLexer::getModeNames() const {
  return sqllexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& SQLLexer::getVocabulary() const {
  return sqllexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView SQLLexer::getSerializedATN() const {
  return sqllexerLexerStaticData->serializedATN;
}

const atn::ATN& SQLLexer::getATN() const {
  return *sqllexerLexerStaticData->atn;
}




void SQLLexer::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  sqllexerLexerInitialize();
#else
  ::antlr4::internal::call_once(sqllexerLexerOnceFlag, sqllexerLexerInitialize);
#endif
}
