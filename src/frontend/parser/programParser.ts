// imports 
import { tokenize } from "../lexer/tokenize";
import ParserContext from "./parserContext";
import StatementParser from "./statementParser";
import { Program, ProgramKind } from "./types";

export default class ProgramParser {

  // an AST is built by parsing program 
  buildAST(sourceCode: string): Program {
    const program: Program = {
      kind: ProgramKind.Program,
      body: [],
    };
    
    const tokens = tokenize(sourceCode); 
    const parserContext = new ParserContext(tokens, 0); 
    const statementParser = new StatementParser(parserContext);

    while(!parserContext.endOf()){
      const stmt = statementParser.parseStatement();
      program.body.push(stmt);
    }

    return program;
  }
}

