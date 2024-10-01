
// Generated from SQL.g4 by ANTLR 4.13.2


#include "SQLListener.h"
#include "SQLVisitor.h"

#include "SQLParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct SQLParserStaticData final {
  SQLParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  SQLParserStaticData(const SQLParserStaticData&) = delete;
  SQLParserStaticData(SQLParserStaticData&&) = delete;
  SQLParserStaticData& operator=(const SQLParserStaticData&) = delete;
  SQLParserStaticData& operator=(SQLParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag sqlParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<SQLParserStaticData> sqlParserStaticData = nullptr;

void sqlParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (sqlParserStaticData != nullptr) {
    return;
  }
#else
  assert(sqlParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<SQLParserStaticData>(
    std::vector<std::string>{
      "sql_stmt", "select_stmt", "column_list", "where_clause", "condition", 
      "comparator", "literal_value"
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
  	4,1,14,43,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,1,0,
  	1,0,1,1,1,1,1,1,1,1,1,1,3,1,22,8,1,1,2,1,2,1,2,5,2,27,8,2,10,2,12,2,30,
  	9,2,1,3,1,3,1,3,1,4,1,4,1,4,1,4,1,5,1,5,1,6,1,6,1,6,0,0,7,0,2,4,6,8,10,
  	12,0,2,1,0,5,10,1,0,12,13,37,0,14,1,0,0,0,2,16,1,0,0,0,4,23,1,0,0,0,6,
  	31,1,0,0,0,8,34,1,0,0,0,10,38,1,0,0,0,12,40,1,0,0,0,14,15,3,2,1,0,15,
  	1,1,0,0,0,16,17,5,1,0,0,17,18,3,4,2,0,18,19,5,2,0,0,19,21,5,11,0,0,20,
  	22,3,6,3,0,21,20,1,0,0,0,21,22,1,0,0,0,22,3,1,0,0,0,23,28,5,11,0,0,24,
  	25,5,3,0,0,25,27,5,11,0,0,26,24,1,0,0,0,27,30,1,0,0,0,28,26,1,0,0,0,28,
  	29,1,0,0,0,29,5,1,0,0,0,30,28,1,0,0,0,31,32,5,4,0,0,32,33,3,8,4,0,33,
  	7,1,0,0,0,34,35,5,11,0,0,35,36,3,10,5,0,36,37,3,12,6,0,37,9,1,0,0,0,38,
  	39,7,0,0,0,39,11,1,0,0,0,40,41,7,1,0,0,41,13,1,0,0,0,2,21,28
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  sqlParserStaticData = std::move(staticData);
}

}

SQLParser::SQLParser(TokenStream *input) : SQLParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

SQLParser::SQLParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  SQLParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *sqlParserStaticData->atn, sqlParserStaticData->decisionToDFA, sqlParserStaticData->sharedContextCache, options);
}

SQLParser::~SQLParser() {
  delete _interpreter;
}

const atn::ATN& SQLParser::getATN() const {
  return *sqlParserStaticData->atn;
}

std::string SQLParser::getGrammarFileName() const {
  return "SQL.g4";
}

const std::vector<std::string>& SQLParser::getRuleNames() const {
  return sqlParserStaticData->ruleNames;
}

const dfa::Vocabulary& SQLParser::getVocabulary() const {
  return sqlParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView SQLParser::getSerializedATN() const {
  return sqlParserStaticData->serializedATN;
}


//----------------- Sql_stmtContext ------------------------------------------------------------------

SQLParser::Sql_stmtContext::Sql_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SQLParser::Select_stmtContext* SQLParser::Sql_stmtContext::select_stmt() {
  return getRuleContext<SQLParser::Select_stmtContext>(0);
}


size_t SQLParser::Sql_stmtContext::getRuleIndex() const {
  return SQLParser::RuleSql_stmt;
}

void SQLParser::Sql_stmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSql_stmt(this);
}

void SQLParser::Sql_stmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSql_stmt(this);
}


std::any SQLParser::Sql_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SQLVisitor*>(visitor))
    return parserVisitor->visitSql_stmt(this);
  else
    return visitor->visitChildren(this);
}

SQLParser::Sql_stmtContext* SQLParser::sql_stmt() {
  Sql_stmtContext *_localctx = _tracker.createInstance<Sql_stmtContext>(_ctx, getState());
  enterRule(_localctx, 0, SQLParser::RuleSql_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(14);
    select_stmt();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Select_stmtContext ------------------------------------------------------------------

SQLParser::Select_stmtContext::Select_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SQLParser::Column_listContext* SQLParser::Select_stmtContext::column_list() {
  return getRuleContext<SQLParser::Column_listContext>(0);
}

tree::TerminalNode* SQLParser::Select_stmtContext::IDENTIFIER() {
  return getToken(SQLParser::IDENTIFIER, 0);
}

SQLParser::Where_clauseContext* SQLParser::Select_stmtContext::where_clause() {
  return getRuleContext<SQLParser::Where_clauseContext>(0);
}


size_t SQLParser::Select_stmtContext::getRuleIndex() const {
  return SQLParser::RuleSelect_stmt;
}

void SQLParser::Select_stmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSelect_stmt(this);
}

void SQLParser::Select_stmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSelect_stmt(this);
}


std::any SQLParser::Select_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SQLVisitor*>(visitor))
    return parserVisitor->visitSelect_stmt(this);
  else
    return visitor->visitChildren(this);
}

SQLParser::Select_stmtContext* SQLParser::select_stmt() {
  Select_stmtContext *_localctx = _tracker.createInstance<Select_stmtContext>(_ctx, getState());
  enterRule(_localctx, 2, SQLParser::RuleSelect_stmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(16);
    match(SQLParser::T__0);
    setState(17);
    column_list();
    setState(18);
    match(SQLParser::T__1);
    setState(19);
    match(SQLParser::IDENTIFIER);
    setState(21);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == SQLParser::T__3) {
      setState(20);
      where_clause();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Column_listContext ------------------------------------------------------------------

SQLParser::Column_listContext::Column_listContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> SQLParser::Column_listContext::IDENTIFIER() {
  return getTokens(SQLParser::IDENTIFIER);
}

tree::TerminalNode* SQLParser::Column_listContext::IDENTIFIER(size_t i) {
  return getToken(SQLParser::IDENTIFIER, i);
}


size_t SQLParser::Column_listContext::getRuleIndex() const {
  return SQLParser::RuleColumn_list;
}

void SQLParser::Column_listContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterColumn_list(this);
}

void SQLParser::Column_listContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitColumn_list(this);
}


std::any SQLParser::Column_listContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SQLVisitor*>(visitor))
    return parserVisitor->visitColumn_list(this);
  else
    return visitor->visitChildren(this);
}

SQLParser::Column_listContext* SQLParser::column_list() {
  Column_listContext *_localctx = _tracker.createInstance<Column_listContext>(_ctx, getState());
  enterRule(_localctx, 4, SQLParser::RuleColumn_list);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(23);
    match(SQLParser::IDENTIFIER);
    setState(28);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == SQLParser::T__2) {
      setState(24);
      match(SQLParser::T__2);
      setState(25);
      match(SQLParser::IDENTIFIER);
      setState(30);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Where_clauseContext ------------------------------------------------------------------

SQLParser::Where_clauseContext::Where_clauseContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

SQLParser::ConditionContext* SQLParser::Where_clauseContext::condition() {
  return getRuleContext<SQLParser::ConditionContext>(0);
}


size_t SQLParser::Where_clauseContext::getRuleIndex() const {
  return SQLParser::RuleWhere_clause;
}

void SQLParser::Where_clauseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhere_clause(this);
}

void SQLParser::Where_clauseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhere_clause(this);
}


std::any SQLParser::Where_clauseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SQLVisitor*>(visitor))
    return parserVisitor->visitWhere_clause(this);
  else
    return visitor->visitChildren(this);
}

SQLParser::Where_clauseContext* SQLParser::where_clause() {
  Where_clauseContext *_localctx = _tracker.createInstance<Where_clauseContext>(_ctx, getState());
  enterRule(_localctx, 6, SQLParser::RuleWhere_clause);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(31);
    match(SQLParser::T__3);
    setState(32);
    condition();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionContext ------------------------------------------------------------------

SQLParser::ConditionContext::ConditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SQLParser::ConditionContext::IDENTIFIER() {
  return getToken(SQLParser::IDENTIFIER, 0);
}

SQLParser::ComparatorContext* SQLParser::ConditionContext::comparator() {
  return getRuleContext<SQLParser::ComparatorContext>(0);
}

SQLParser::Literal_valueContext* SQLParser::ConditionContext::literal_value() {
  return getRuleContext<SQLParser::Literal_valueContext>(0);
}


size_t SQLParser::ConditionContext::getRuleIndex() const {
  return SQLParser::RuleCondition;
}

void SQLParser::ConditionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCondition(this);
}

void SQLParser::ConditionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCondition(this);
}


std::any SQLParser::ConditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SQLVisitor*>(visitor))
    return parserVisitor->visitCondition(this);
  else
    return visitor->visitChildren(this);
}

SQLParser::ConditionContext* SQLParser::condition() {
  ConditionContext *_localctx = _tracker.createInstance<ConditionContext>(_ctx, getState());
  enterRule(_localctx, 8, SQLParser::RuleCondition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(34);
    match(SQLParser::IDENTIFIER);
    setState(35);
    comparator();
    setState(36);
    literal_value();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComparatorContext ------------------------------------------------------------------

SQLParser::ComparatorContext::ComparatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t SQLParser::ComparatorContext::getRuleIndex() const {
  return SQLParser::RuleComparator;
}

void SQLParser::ComparatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterComparator(this);
}

void SQLParser::ComparatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitComparator(this);
}


std::any SQLParser::ComparatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SQLVisitor*>(visitor))
    return parserVisitor->visitComparator(this);
  else
    return visitor->visitChildren(this);
}

SQLParser::ComparatorContext* SQLParser::comparator() {
  ComparatorContext *_localctx = _tracker.createInstance<ComparatorContext>(_ctx, getState());
  enterRule(_localctx, 10, SQLParser::RuleComparator);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(38);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 2016) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Literal_valueContext ------------------------------------------------------------------

SQLParser::Literal_valueContext::Literal_valueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* SQLParser::Literal_valueContext::NUMBER() {
  return getToken(SQLParser::NUMBER, 0);
}

tree::TerminalNode* SQLParser::Literal_valueContext::STRING_LITERAL() {
  return getToken(SQLParser::STRING_LITERAL, 0);
}


size_t SQLParser::Literal_valueContext::getRuleIndex() const {
  return SQLParser::RuleLiteral_value;
}

void SQLParser::Literal_valueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLiteral_value(this);
}

void SQLParser::Literal_valueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<SQLListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLiteral_value(this);
}


std::any SQLParser::Literal_valueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<SQLVisitor*>(visitor))
    return parserVisitor->visitLiteral_value(this);
  else
    return visitor->visitChildren(this);
}

SQLParser::Literal_valueContext* SQLParser::literal_value() {
  Literal_valueContext *_localctx = _tracker.createInstance<Literal_valueContext>(_ctx, getState());
  enterRule(_localctx, 12, SQLParser::RuleLiteral_value);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(40);
    _la = _input->LA(1);
    if (!(_la == SQLParser::NUMBER

    || _la == SQLParser::STRING_LITERAL)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void SQLParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  sqlParserInitialize();
#else
  ::antlr4::internal::call_once(sqlParserOnceFlag, sqlParserInitialize);
#endif
}
